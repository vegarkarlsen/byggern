#include "pio/pio_sam3x8e.h"
#include "uart_and_printf/printf-stdarg.h"
#include "uart_and_printf/uart.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

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

    // Set baudrate, Phase1, phase2 and propagation delay for can bus. Must
    // match on all nodes!
    uint32_t can_br = CAN_BR_BRP(41) | CAN_BR_SJW(1) | CAN_BR_PROPAG(2) | CAN_BR_PHASE1(7) | CAN_BR_PHASE2(6);
    can_init_def_tx_rx_mb(can_br);

    printf("Setup complete\n\r");

    turn_on_inboard_led();
    CAN_MESSAGE can_pack = {
        1,
        2,
        "ti"
    };

    while (1) {
        can_send(&can_pack, 0);
        /* uint8_t status = can_receive(&can_pack, 0); */
        /* printf("ID: %s\r\nlen %s\r\ndata: %c",can_pack.id, can_pack.data_length, can_pack.data[0]); */
    }
}
