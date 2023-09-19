#pragma once
#include "OLED_driver.h"
#include <avr/io.h>
#include "include/fonts.h"


void OLED_write_command(uint8_t cmd){
    volatile char *command = (char *) OLED_COMMAND_START_ADRESS;
    command[0] = cmd;


}
void OLED_write_data(uint8_t data){
    volatile char *d = (char * ) OLED_DATA_START_ADRESS;
    d[0] = data;
}


void OLED_init() {
 OLED_write_command(0xAE); // display off
 OLED_write_command(0xA1); //segment remap
 OLED_write_command(0xDA); //common pads hardware: alternative
 OLED_write_command(0x12);
 OLED_write_command(0xC8); //common output scan direction:com63~com0
 OLED_write_command(0xA8); //multiplex ration mode:63
 OLED_write_command(0x3F);
 OLED_write_command(0xD5); //display divide ratio/osc. freq. mode
 OLED_write_command(0x80);
 OLED_write_command(0x81); //contrast control
 OLED_write_command(0x50);
 OLED_write_command(0xD9); //set pre-charge period
 OLED_write_command(0x21);
 OLED_write_command(0x20); //Set Memory Addressing Mode
 OLED_write_command(0x02);
 OLED_write_command(0xDB); //VCOM deselect level mode
 OLED_write_command(0x30);
 OLED_write_command(0xAD); //master configuration
 OLED_write_command(0x00);
 OLED_write_command(0xA4); //out follows RAM content
 OLED_write_command(0xA6); //set normal display
 OLED_write_command(0xAf); // display on
}


void OLED_go_line(int line){
    OLED_write_command(0xB0 + line);
}

void OLED_go_col(int col){
    OLED_write_command(0x00 + (col & 16)); // Lower nibble
    OLED_write_command(0x10 + (col / 16)); // Higher nibble
}

void OLED_move(int line, int col){
    OLED_go_line(line);
    OLED_go_col(col);
}


void OLED_clear_screen(){
    for (int l = 0; l < 8; l++){
        OLED_go_line(l);
        OLED_go_col(0);
        for (int i = 0; i < 128; i++){
            OLED_write_data(0x00);
        }
    }
}

void OLED_printChar8(char c ){
    int c_with_offsett = c - 32;
    for (uint8_t i = 0; i < 7; i++){
        uint8_t font_byte = pgm_read_byte(&font8[c_with_offsett][i]);
        OLED_write_data(font_byte);
        printf(c);

    }
}

void OLED_print8(char *str){
    for (int i; i < strlen(str); i++){
        OLED_printChar8(str[i]);
    }
}
