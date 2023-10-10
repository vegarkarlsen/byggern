#include "CAN_controller.h"
#include "MCP2515_driver.h"
#include "mcp2515.h"
#include <stdint.h>
#include <stdio.h>

void CAN_init() {
    MCP_init();
    // loopback mode for testing
    MCP_modify_bit(MCP_CANCTRL, 0xE0, MODE_LOOPBACK);
}

uint8_t get_send_buffer_adress(uint8_t buffer){
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

void CAN_send(uint8_t buffer, canPack_ptr can_pack){
    uint8_t buffer_adress = get_send_buffer_adress(buffer);

    // ID
    uint8_t high = (can_pack->ID >> 3);
    uint8_t low = (can_pack->ID << 5);
    MCP_write(buffer_adress +1, high);
    MCP_write(buffer_adress + 2, low);
    printf("s heigh: 0x%x, low: 0x%x\r\n", high, low);

    // data lenght
    MCP_write(buffer_adress + 5, can_pack->len);

    // message max len = 8,
    for (uint8_t i = 0; i < can_pack->len; i++){
        MCP_write(buffer_adress + 6 + i, can_pack->data[i]);
    }

    MCP_request_to_send(buffer);
}

uint8_t get_recive_buffer_adress(uint8_t buffer){
    switch (buffer) {
        case 0:
            return MCP_RXB0CTRL;

        case 1:
            return MCP_RXB1CTRL;

        default:
            return MCP_RXB0CTRL; 
    }
}

canPack_t CAN_revice(uint8_t buffer){
    uint8_t buffer_adress = get_recive_buffer_adress(buffer);

    canPack_t can_pack;
    // ID = (high << 3) | low
    uint8_t high = MCP_read(buffer_adress + 1);
    uint8_t low = MCP_read(buffer_adress + 2);
    /* can_pack.ID = (high >> 3) | low; */
    can_pack.ID = 0;
    printf("r high 0x%x, low: 0x%x\r\n", high, low);

    can_pack.len = MCP_read(buffer_adress + 5);
    for (uint8_t i = 0; i < can_pack.len; i++){
        can_pack.data[i] = MCP_read(buffer_adress + 6 + i);
    }

    return can_pack;
}

