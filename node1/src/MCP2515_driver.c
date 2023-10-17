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

void MCP_init() {
    SPI_master_init();
    MCP_reset();

    _delay_ms(10);

    // configuring baud rate:
    /* from node2: */
    /* CAN_BR_BRP(41) | CAN_BR_SJW(1) | CAN_BR_PROPAG(2) | CAN_BR_PHASE1(7) |
     * CAN_BR_PHASE2(6); */
    uint8_t BRP = 4;
    uint8_t BLTMODE = 1;
    uint8_t SJW = 1;
    uint8_t PROPAG = 2;
    uint8_t PS1 = 7;
    uint8_t PS2 = 6;

    // CNF1 bit 7-6: SJW, bit 5-0: BPR
    MCP_write(MCP_CNF1, (SJW << 6) | (BRP));
    // CNF2 bit7: BLTMODE, bit6: SAM, bit5-3: PHSEG1, bit2-0: PRSEG
    MCP_write(MCP_CNF2, (BLTMODE << 7) | (PS1 << 3) | (PROPAG));
    // CNF3 => bit7: SOF, bit6 WAKFIL, bit5-3: 0, bit2-0: PS2
    MCP_write(MCP_CNF3, PS2);
}
