#include "MCP2515_driver.h"
#include "SPI_driver.h"
#include "mcp2515.h"
#include "util/delay.h"
#include <stdint.h>

void MCP_reset() {
    SPI_reset_ss();
    SPI_transmit(MCP_RESET);
    SPI_set_ss();
}

uint8_t MCP_read(uint8_t adress) {
    uint8_t byte;
    SPI_reset_ss();
    SPI_transmit(MCP_READ);
    SPI_transmit(adress);
    byte = SPI_revice();
    SPI_set_ss();
    return byte;
}

void MCP_write(uint8_t adress, uint8_t byte) {
    SPI_reset_ss();
    SPI_transmit(MCP_WRITE);
    SPI_transmit(adress);
    SPI_transmit(byte);
    SPI_set_ss();
}

void MCP_modify_bit(uint8_t adress, uint8_t mask, uint8_t byte) {
    SPI_reset_ss();
    SPI_transmit(MCP_BITMOD);
    SPI_transmit(adress);
    SPI_transmit(mask);
    SPI_transmit(byte);
    SPI_set_ss();
}

/* void MCP_request_to_send(uint8_t buffer) { */
/*     // if try to choose invalid buffer, choose buffer 0  */
/*     // see MCP_RTX_* for which buffers is availible */
/*     if (buffer > 2){ */
/*         buffer = 0; */
/*     } */
/**/
/*     uint8_t buffer_adress = 0x80; */
/*     buffer_adress |= (1 << buffer); */
/**/
/*     SPI_reset_ss(); */
/*     SPI_transmit(buffer_adress); */
/*     SPI_reset_ss(); */
/* } */
void MCP_request_to_send(uint8_t buffer) {
    // if try to choose invalid buffer, choose buffer 0
    // see MCP_RTS_* for which buffers is availible
    uint8_t buffer_adress = MCP_RTS_TX0;

    switch (buffer) {
    case 0:
        buffer_adress = MCP_RTS_TX0;
        break;
    case 1:
        buffer_adress = MCP_RTS_TX1;
        break;
    case 2:
        buffer_adress = MCP_RTS_TX2;
        break;
    }

    SPI_reset_ss();
    SPI_transmit(buffer_adress);
    SPI_set_ss();
}

uint8_t MCP_read_status() {
    SPI_reset_ss();
    SPI_transmit(MCP_READ_STATUS);
    uint8_t byte = SPI_revice();
    SPI_set_ss();
    return byte;
}
void MCP_init_loopBack() {

    SPI_master_init();
    MCP_reset();

    _delay_ms(10);

    MCP_modify_bit(MCP_CANCTRL, 0xE0, MODE_LOOPBACK);
}
void MCP_init() {
    SPI_master_init();
    MCP_reset();

    _delay_ms(10);
}
