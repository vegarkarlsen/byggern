#pragma once
#include <stdio.h>
extern int option_select;
extern int menu_level;


void home_screen_print();

void option_change(int joy_val_y);

void menu_level_select(int joy_val_x);