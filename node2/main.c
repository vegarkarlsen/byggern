#include "pio/pio_sam3x8e.h"
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

int main() {
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; // Disable Watchdog Timer

    configure_uart();

    // init_can();
    // pwm_init();

    // SysTick_Config(10500);

    ir_init();

    printf("Setup complete\n\r");

    turn_on_inboard_led();

    // CAN_MESSAGE can_pack;
    while (1) {
        // joy_test(50, 6);
        printf("ADC: %d\n\r", read_ir_raw());
        goals = detect_goal(goals);
        printf("Goals: %d\n\r", goals);

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
