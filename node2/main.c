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

// #define DEBUG_MOTOR
#include "log.h"



//GLOBAL VARIABLES
uint8_t goals = 0;

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


int main() {
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; // Disable Watchdog Timer

    configure_uart();

    // init_can();
    pwm_init();

    SysTick_Config(10500);

    ir_init();
    motor_init();

    turn_on_inboard_led();

    printf("Setup complete\n\r");
    uint16_t max_pos_raw = calibrate_encoder();

    // CAN_MESSAGE can_pack;
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
        


        // uint16_t test = dac_write_percentage(120);
        
        // printf("DACC interput status register: %x\n\r", DACC->DACC_ISR);
        // printf("ADC: %d\n\r", read_ir_raw());
        // detect_goal(&goals);
        // printf("Goals: %d\n\r", goals);



        // can_send(&can_pack, 0);
        // uint8_t status = can_receive(&can_pack,00);
        //
        // // can_print(&can_pack);
        // // joystick values
        // if (can_pack.id == 7){
        //     int8_t x = can_pack.data[0];
        //     int8_t y = can_pack.data[1];
        //     printf("(x,y): (%d, %d)\n\r", x, y);
        //     joy_test(y, 6);
        // }

        /* printf("%d\n\r", getTimeMs()); */
    }
}
