#include "pio/pio_sam3x8e.h"
#include "timer.h"
#include "uart_and_printf/printf-stdarg.h"
#include "uart_and_printf/uart.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
/* #include <util/delay.h> */

#include "sam.h"
#include "can_controller.h"
#include "pwm_lib.h"
#include "timer.h"

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

void joy_test(int x){
    uint8_t pros;
    if (x < 0){
        pros = x * -1;
    }
    else {
        pros = x;
    }
    set_pwn_duty_cycle(pros);

}

int main() {
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; // Disable Watchdog Timer

    configure_uart();

    init_can();
    pwm_init();
    
    SysTick_Config(10500);


    printf("Setup complete\n\r");

    turn_on_inboard_led();
    CAN_MESSAGE can_pack;

    while (1) {
        /* can_send(&can_pack, 0); */
        /* uint8_t status = can_receive(&can_pack, 0); */
        /* can_print(&can_pack); */
        // joystick values
        uint8_t status = can_receive(&can_pack, 0);
        int8_t x = can_pack.data[0];
        int8_t y = can_pack.data[1];
        printf("(x,y): (%d, %d)\n\r", x,y);

        /* set_pwn_duty_cycle(100); */
        joy_test(y);

        /* printf("%d\n\r", getTimeMs()); */
 
    }
}
