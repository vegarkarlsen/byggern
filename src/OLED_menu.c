#pragma once
#include "OLED_menu.h"
#include "OLED_driver.h"
#include "ADC_driver.h"
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

const char *page_1[] = {"OPTION 1","OPTION 2","OPTION 3"};
const char *page_2[] = {"TEST","RETURN"};
const char **menu[] = {page_1, page_2};

int menu_size;

void home_screen_print(){
    OLED_clear_screen();
    OLED_move(0,0);
    if(menu_level==0){
        menu_size = sizeof(page_1) / sizeof(page_1[0]);
        if (option_select<0){
            option_select = 2;
        }
         else if (option_select>2){
         option_select = 0;
         }
    }
    else if (menu_level==1){
        menu_size = sizeof(page_2) / sizeof(page_2[0]);
        if (option_select<0){
            option_select = 1;
        }
    else if (option_select>1){
         option_select = 0;
         }
    }
    for(uint8_t i = 0; i<menu_size; i++){
        OLED_move(0+i,0);
        if(i==option_select){
            OLED_print8(menu[menu_level][i]);
            OLED_print8(" <-");
        }
        else{
            OLED_print8(menu[menu_level][i]);
        }
    };
}

void option_change(int joy_val){
    if (joy_val>0){
        option_select -= 1;
        //home_screen_print();
        _delay_ms(200);
    }
    else if (joy_val<0){
        option_select += 1;
        //home_screen_print();
        _delay_ms(200);
    }
}

void menu_level_select(){
    if (option_select==0 || menu_level == 0){
        menu_level = 1;
    }

    else if(option_select == 1 || menu_level == 1){
        menu_level = 0;
    }
}