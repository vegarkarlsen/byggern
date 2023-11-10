#include "can_handling.h"
#include "compiler.h"
#include "pio/pio_sam3x8e.h"
#include "sam3x8e.h"
#include "solenoid.h"
#include "timer.h"
#include "uart_and_printf/printf-stdarg.h"
#include "uart_and_printf/uart.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <wchar.h>

#include "can_controller.h"
#include "pwm_lib.h"
#include "sam.h"
#include "timer.h"
#include "IR_driver.h"
#include "motor_driver.h"
#include "pid_controller.h"

#include "can_handling.h"
#include "can_interrupt.h"


//GLOBAL VARIABLES
// uint8_t goals = 0;
uint32_t start_time = 0;

// int8_t x;
// int8_t y; 
// uint8_t button_1;
// uint8_t button_2;
// uint8_t button_3;
// uint8_t slider_left;
// uint8_t slider_right;


void turn_on_inboard_led() {
    // enbale IO
    PIOA->PIO_PDR |= (PIO_PA19 | PIO_PA20);

    // set direction as output
    PIOA->PIO_ODR |= (PIO_PA19 | PIO_PA20);

    // dissable internal pullup
    PIOA->PIO_PUDR |= (PIO_PA19 | PIO_PA20);

    // write directly to pin
    PIOA->PIO_OWER = (PIO_PA19 | PIO_PA20);

    PIOA->PIO_ODSR |= (PIO_PA19);
    PIOA->PIO_ODSR |= (PIO_PA20);
}


uint8_t slider_to_persentage(uint8_t slider_val){
    uint16_t scaled = (slider_val*100)/255;

    return (uint8_t)scaled;
}

uint8_t raw_pos_to_persentage(uint16_t raw_pos, uint16_t max_pos){
    return (raw_pos * 100)/max_pos;
}

long map(long input, long i_min, long i_max, long o_max, long o_min) {
    return (input - i_min) * (o_max - o_min) / (i_max - i_min) + o_min;
}

uint8_t map_joystick_to_servo_voltage(int8_t joy){
    uint8_t servo_output = 50;
    if (joy < 3){
        servo_output = map(joy, 0, -100, 0, 50);
    }
    else if (joy > 3){
        servo_output = map(joy, 0, 100, 100, 50);
    }
    // else {
    //     servo_output = 50;
    // }
    printf("servo: %d\r\n", servo_output);
    return servo_output;

}


int main() {
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; // Disable Watchdog Timer

    configure_uart();

    init_can();
    pwm_init();

    SysTick_Config(10500);

    ir_init();
    motor_init();

    turn_on_inboard_led();

    uint16_t max_pos_raw = calibrate_encoder();
    enable_motor();

    solenoid_init();

    // set PID values
    PID_t pid = {
        0.8, 0.7, 1, 0.1
    };
    uint8_t *gs = get_game_state_global();
    *gs = 0;

    printf("Setup complete\n\r");
    // CAN_MESSAGE can_pack;
    while (1) {

        stop_motor();
        if (*gs == 1){
            start_time = getTimeMs(); 
        }
        // printf("Game off\n\r");
        while (*gs == 1){
            
            enable_motor();
            /* ------------------ GOAL DETECTION ------------------*/        
            detect_goal(get_goals_global());
            // printf("Goals: %d\n\r", *get_goals_global());
            /* ------------------ GOAL DETECTION ------------------*/
            
            /* ----------------------- PID ------------------------*/
            uint32_t t_be = getTimeMs();
            // printf("----------------------------------------\n\r");
            multiboard_t *multiboard = get_global_multiboard_vars();
            // printf("slider_left %d\n\r", multiboard->slider_left);

            uint8_t referance = slider_to_persentage(multiboard->slider_left);
            // uint8_t referance = Abs(multiboard->joystick_x);
            // printf("referance: (raw, real) (%d, %d)\n\r", multiboard->slider_left, referance);
            uint16_t raw_pos = read_encoder();
            uint8_t pos = raw_pos_to_persentage(raw_pos, max_pos_raw);
            // printf("pos (raw, real): (%d, %d)\n\r", raw_pos, pos);
            // //
            int16_t u = PI(pid, referance, pos);
            // printf("using p, u=%d\n\r", u);
            uint8_t motor_dir = 1;
            int8_t motor_voltage = u;
            if (u < 0){
                motor_voltage = u * -1;
                motor_dir = 0;
            }
            // printf("u=%d, dir=%d\n\r", motor_voltage, motor_dir);
            move_motor(motor_voltage, motor_dir);
            uint32_t t_af = getTimeMs();
            uint32_t t_tot = t_af-t_be;
            // printf("time: %d\n\r", t_tot);
            /* ----------------------- PID ------------------------*/

            /* -------------------- SOLENOID ----------------------*/
            if (multiboard->left_button){
                solenoid_on();
            } 
            else {
                solenoid_off();
            }
            /* -------------------- SOLENOID ----------------------*/

            /* ---------------------- SERVO -----------------------*/
            // servo handeling goes here
            
            // printf("raw_joystick %d\n\r", multiboard->joystick_x);
            uint8_t servo_output = map_joystick_to_servo_voltage(multiboard->joystick_x);
            // uint8_t servo_output = map_joystick_to_servo_voltage(slider_to_persentage(multiboard->slider_right));
            set_pwn_duty_cycle(servo_output, 6);

            /* ---------------------- SERVO -----------------------*/
            // _ms_delay(200);
        }

        uint32_t highscore = getTimeMs() - start_time;
        update_higscore(highscore/1000); // in seconds
    }
}
