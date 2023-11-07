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
uint8_t goals = 0;

int8_t x;
int8_t y; 
uint8_t button_1;
uint8_t button_2;
uint8_t button_3;
uint8_t slider_left;
uint8_t slider_right;

// #include "can_interrupt.h"

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

void motor_test(){
    printf("Start motor\n\r");
    enable_motor();
    uint32_t time_now = getTimeMs();
    printf("waiting\n\r");
    _ms_delay(200);
    move_motor(1, 1);
    stop_motor();
    printf("stopping motor\n\r");
}

uint8_t slider_to_persentage(uint8_t slider_val){
    uint16_t scaled = (slider_val*100)/255;

    return (uint8_t)scaled;
}

    // return (input - i_min) * (o_max - o_min) / (i_max - i_min) + o_min;
uint8_t raw_pos_to_persentage(uint16_t raw_pos, uint16_t max_pos){
    return (raw_pos * 100)/max_pos;
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

    printf("Setup complete\n\r");
    uint16_t max_pos_raw = calibrate_encoder();
    // move_motor(25, 1);
    enable_motor();

    solenoid_init();
    solenoid_off();

    // set PID values
    PID_t pid = {
        0.8, 1.2, 1, 0.1
    };

    CAN_MESSAGE can_pack;
    while (1) {

        detect_goal(&goals);
        printf("Goals: %d\n\r", goals);

        
        /* ----------------------- PID ------------------------*/
        uint32_t t_be = getTimeMs();
        printf("----------------------------------------\n\r");
        multiboard_t *multiboard = get_global_multiboard_vars();
        printf("slider_left %d\n\r", multiboard->slider_left);

        uint8_t referance = slider_to_persentage(multiboard->slider_left);
        // uint8_t referance = Abs(multiboard->joystick_x);
        printf("referance: (raw, real) (%d, %d)\n\r", multiboard->slider_left, referance);
        uint16_t raw_pos = read_encoder();
        uint8_t pos = raw_pos_to_persentage(raw_pos, max_pos_raw);
        printf("pos (raw, real): (%d, %d)\n\r", raw_pos, pos);
        // //
        int16_t u = PI(pid, referance, pos);
        // printf("using p, u=%d\n\r", u);
        uint8_t motor_dir = 1;
        int8_t motor_voltage = u;
        if (u < 0){
            motor_voltage = u * -1;
            motor_dir = 0;
        }
        printf("u=%d, dir=%d\n\r", motor_voltage, motor_dir);
        move_motor(motor_voltage, motor_dir);
        uint32_t t_af = getTimeMs();
        uint32_t t_tot = t_af-t_be;
        printf("time: %d\n\r", t_tot);
        /* ----------------------- PID ------------------------*/

        /* -------------------- SOLENOID ----------------------*/
        if (multiboard->left_button){
            solenoid_off();
        } 
        else {
            solenoid_on();
        }
        /* -------------------- SOLENOID ----------------------*/

    }
}
