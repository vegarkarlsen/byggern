#pragma once
#include <avr/io.h>
// #include <stdlib.h>

#define OLED_COMMAND_START_ADRESS 0x1000
#define OLED_DATA_START_ADRESS 0x1200

void OLED_init();

void OLED_write_command(uint8_t cammand);
void OLED_write_data(uint8_t data);


void OLED_go_line(int line);

// TODO: this do not work as espected
void OLED_go_col(int col);

void OLED_clear_screen();


void OLED_move(int line, int col);

// font size is 8
void OLED_printChar8(char c);

void OLED_pritnt8(char * str);