#include "can_handling.h"
#include "compiler.h"
#include "pio/pio_sam3x8e.h"
#include "sam3x8e.h"
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
    return (slider_val*100/255);
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

    // set PID values
    PID_t pid = {
        1, 1, 1, 30
    };

    CAN_MESSAGE can_pack;
    while (1) {
        // dac_write(4095);
        // joy_test(50, 6);
        // printf("%d\n\r", PIOC->PIO_PDSR);
        // printf("0x%x\n\r", PIOC->PIO_PSR);
        // printf("start\n\r");
        // _ms_delay(2000);

        // int16_t motor_pos = read_encoder();
        // printf("%d\r\n", motor_pos);
        // printf("Max pos: %d\n\r", max_pos_raw);

        printf("----------------------------------------\n\r");
        multiboard_t *multiboard = get_global_multiboard_vars();
        printf("slider_left %d\n\r", multiboard->slider_left);
        
        
        // detect_goal(&goals);
        // printf("Goals: %d\n\r", goals);



        //
        uint8_t referance = slider_to_persentage(multiboard->slider_left);
        printf("maped referance: %d\n\r", referance);
        uint16_t raw_pos = read_encoder();
        uint8_t pos = raw_pos_to_persentage(raw_pos, max_pos_raw);
        printf("pos (raw, real): (%d, %d)\n\r", raw_pos, pos);
        // //
        int16_t u = P(pid, referance, pos);
        printf("using p, u=%d\n\r", u);
        uint8_t motor_dir = 1;
        int8_t motor_voltage = u;
        if (u < 0){
            motor_voltage = u * -1;
            motor_dir = 0;
        }
        printf("u=%d, dir=%d\n\r", motor_voltage, motor_dir);
        move_motor(u, motor_dir);
        // printf("----------------------------------------\n\r");


        //
        // printf("u: %d\n\r", u);
        // printf("%d\n\r", can_pack.id);
        // printf("raw_r: %d\n\r", slider_left);
        // printf("pos: %d\n\r", pos);
        // printf("r: %d\n\r", referance);
        // printf("motor_dir: %d\n\r", motor_dir);
        // printf("intgeral: %d\n\r", get_integral());

    }
}
