#include "pio/pio_sam3x8e.h"
#include "uart_and_printf/printf-stdarg.h"
#include "uart_and_printf/uart.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
/* #include <util/delay.h> */

#include "can_controller.h"
#include "sam.h"
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

    init_can();

    printf("Setup complete\n\r");

    turn_on_inboard_led();
    CAN_MESSAGE can_pack;

    while (1) {
        /* can_send(&can_pack, 0); */
        uint8_t status = can_receive(&can_pack, 0);
        /* printf("id %d\n\r", can_pack.id); */
        /* int8_t x = can_pack.data[0]; */
        /* printf("%d\n\r", x); */

        can_print(&can_pack);
        /* printf("ID: %d\r\nlen %d\r\ndata: %c\r\n", can_pack.id, can_pack.data_length, can_pack.data[0]); */
    }
}
