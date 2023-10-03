#pragma once
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// Reset should be part of initalization prosess
void MCP_reset();

// set MCP in read mode?
uint8_t MCP_read(uint8_t adress);

void MCP_init();

void MCP_write(uint8_t adress, uint8_t byte);

void MCP2515_loopBackMode_init();


