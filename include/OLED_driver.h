#pragma once
#include <avr/io.h>
// #include <stdlib.h>

#define OLED_COMMAND_START_ADRESS 0x1000
#define OLED_DATA_START_ADRESS 0x1200

void OLED_init();

void OLED_write_command(uint8_t cammand);
void OLED_write_data(uint8_t data);

// go to a line (0 - 7)
void OLED_go_line(int line);
// go to column (0 - 127)
void OLED_go_col(int col);
// clear all pixels
void OLED_clear_screen();

// go to line x and column y
void OLED_move(int line, int col);

// Print one char with font 8
void OLED_printChar8(char c);
// Print str whit font 8
void OLED_print8(char *str);

void OLED_printChar(char c, int font_size);
void OLED_print(char *str, int font_size);
