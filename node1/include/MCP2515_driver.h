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

/*
 * Three availible buffers:
 * buffer = 0
 * buffer = 1
 * buffer = 2*/
void MCP_request_to_send(uint8_t buffer);

void MCP_modify_bit(uint8_t adress, uint8_t mask, uint8_t data);

uint8_t MCP_read_status();
