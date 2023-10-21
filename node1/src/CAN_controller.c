#include "CAN_controller.h"
#include "MCP2515_driver.h"
#include "mcp2515.h"
#include <stdint.h>
#include <stdio.h>

void CAN_init() {
    MCP_init();

    // configuring baud rate:
    /* from node2: */
    /* CAN_BR_BRP(41) | CAN_BR_SJW(1) | CAN_BR_PROPAG(2) | CAN_BR_PHASE1(7) |
     * CAN_BR_PHASE2(6); */
    uint8_t BRP = 3;
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

    // normal mode
    MCP_modify_bit(MCP_CANCTRL, 0xE0, MODE_NORMAL);
}

uint8_t get_send_buffer_adress(uint8_t buffer) {
    switch (buffer) {
    case 0:
        return MCP_TXB0CTRL;

    case 1:
        return MCP_TXB1CTRL;

    case 2:
        return MCP_TXB2CTRL;

    default:
        return MCP_TXB0CTRL;
    }
}

void CAN_send(uint8_t buffer, canPack_t *can_pack) {
    uint8_t buffer_adress = get_send_buffer_adress(buffer);

    // ID
    uint8_t high = (can_pack->ID >> 3);
    uint8_t low = (can_pack->ID << 5);
    MCP_write(buffer_adress + 1, (uint8_t)high);
    MCP_write(buffer_adress + 2, (uint8_t)low);
    /* printf("s heigh: 0x%04x, low: 0x%04x\r\n", high, low); */

    // data lenght
    MCP_write(buffer_adress + 5, can_pack->len);

    // message max len = 8,
    for (uint8_t i = 0; i < can_pack->len; i++) {
        MCP_write(buffer_adress + 6 + i, can_pack->data[i]);
    }

    MCP_request_to_send(buffer);
}

uint8_t get_recive_buffer_adress(uint8_t buffer) {
    switch (buffer) {
    case 0:
        return MCP_RXB0CTRL;

    case 1:
        return MCP_RXB1CTRL;

    default:
        return MCP_RXB0CTRL;
    }
}

canPack_t CAN_revice(uint8_t buffer) {
    uint8_t buffer_adress = get_recive_buffer_adress(buffer);

    uint8_t high = MCP_read(buffer_adress + 1);
    uint8_t low = MCP_read(buffer_adress + 2);
    canPack_t can_pack;
    can_pack.ID = (high << 3) | (low >> 5);
    /* can_pack.ID = (high >> 3) | low; */
    /* printf("r high 0x%x, low: 0x%x\r\n", high, low); */

    can_pack.len = MCP_read(buffer_adress + 5);
    for (uint8_t i = 0; i < can_pack.len; i++) {
        can_pack.data[i] = MCP_read(buffer_adress + 6 + i);
    }

    return can_pack;
}

void CAN_print(canPack_t message) {

    printf("---------------------------------\r\n");
    printf("ID: %d\r\n", message.ID);
    printf("lenght: %d\r\n", message.len);

    uint8_t length = message.len;
    if (length > 8) {
        length = 8;
    }
    for (int i = 0; i < length; i++) {
        printf("data[%d]: %d\r\n", i, message.data[i]);
    }
}
