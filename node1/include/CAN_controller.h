#pragma once
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/*
 * CAN message for buffer n:       buffer start adress T0=0x30, T1=0x40,
 * T2=0x50, R0=0x60, R1=0x70 TXBnCTRL = +0        -> Control? TXBnSIDH = +1 ->
 * ID first 8bit TXBnSIDL = +2        -> ID last 3bit TXBnEID8 = +3        -> ID
 * extended high TXBnEID0 = +4        -> ID extended low TXBnDLC = +5         ->
 * data lenght TXBnD0 = +6          -> data TXBnD1 = +7          -> data TXBnD2
 * = +8          -> data TXBnD3 = +9          -> data TXBnD4 = +10         ->
 * data TXBnD5 = +11         -> data TXBnD6 = +12         -> data TXBnD7 = +13
 * -> data
 * */

typedef struct CanPack {
    uint16_t ID;
    uint8_t len;
    char data[8];
} canPack_t;

uint8_t get_send_buffer_adress(uint8_t buffer);
uint8_t get_recive_buffer_adress(uint8_t buffer);

// initalize MCP, and set baud rate on CAN comm
void CAN_init();

void CAN_send(canPack_t *can_pack, uint8_t buffer);

uint8_t CAN_revice(canPack_t *can_pack, uint8_t buffer);

void CAN_print(canPack_t *message);
