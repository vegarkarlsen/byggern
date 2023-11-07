#pragma once
#include <stdio.h>
extern int option_select;
extern int menu_level;


void home_screen_print();

void option_change(int8_t joy_val);

void menu_level_select(int8_t joy_val_x, uint8_t *game_state);