#pragma once
#include "OLED_menu.h"
#include "ADC_driver.h"
#include "OLED_driver.h"
#include "Multiboard_tools.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

const char *page_1[] = {"START GAME", "HIGHSCORE", "RULES"};
const char *page_2[] = {"DO YOU WISH ", "TO START?", "YES", "NO"};
const char *page_3[] = {"HIGHSCORE: ", /*show goals*/ "YES", "RETURN"};
const char *page_4[] = {"THERE", "ARE", "NO", "RULES", "EXIT"};
const char **menu[] = {page_1, page_2, page_3, page_4};

int menu_size;

// FIXME: option_select and menu_level should be input

void home_screen_print() {
    OLED_clear_screen();
    OLED_move(0, 0);
    if (menu_level == 0) {
        menu_size = sizeof(page_1) / sizeof(page_1[0]);
        if (option_select < 0) {
            option_select = 2;
        } else if (option_select > 2) {
            option_select = 0;
        }
    } else if (menu_level == 1) {
        menu_size = sizeof(page_2) / sizeof(page_2[0]);
        if (option_select < 2) {
            option_select = 3;
        } else if (option_select > 3) {
            option_select = 2;
        }
    }
    else if(menu_level == 2){
        menu_size = sizeof(page_3) / sizeof(page_3[0]);
        if (option_select < 2) {
            option_select = 2;
        } else if (option_select > 2) {
            option_select = 2;
        }
    }
    else if(menu_level == 3){
        menu_size = sizeof(page_4) / sizeof(page_4[0]);
        if (option_select < 4) {
            option_select = 4;
        } else if (option_select > 4) {
            option_select = 4;
        }
    }
    for (uint8_t i = 0; i < menu_size; i++) {
        OLED_move(0 + i, 0);
        if (i == option_select) {
            OLED_print8(menu[menu_level][i]);
            OLED_print8(" <-");
        } else {
            OLED_print8(menu[menu_level][i]);
        }
    };
}

void option_change(int8_t joy_val) {
    if (joy_val > 0) {
        option_select -= 1;
        // home_screen_print();
        _delay_ms(200);
    } else if (joy_val < 0) {
        option_select += 1;
        // home_screen_print();
        _delay_ms(200);
    }
}

void menu_level_select(int8_t joy_val_x, uint8_t *gamestate){
    /*---------------MENU LEVEL 0---------------*/
    if (option_select==0 && menu_level == 0 && joy_val_x > 0){
        menu_level = 1;
        option_select = 2;
    }
    else if(option_select == 1 && menu_level == 0 && joy_val_x > 0){
        menu_level = 2;
        option_select = 2;
    }
    else if(option_select == 2 && menu_level == 0 && joy_val_x > 0){
        menu_level = 3;
        option_select = 4;
    }
    /*----------------MENU LEVEL 1----------------*/
    else if(option_select == 3 && menu_level == 1 && joy_val_x < 0){
        menu_level = 0;
        option_select = 0;
    }
    else if(option_select == 2 && menu_level == 1 && joy_val_x > 0){
        //implement lives and time
        menu_level = 0;
        option_select = 0;
        OLED_clear_screen();
        OLED_print8("PLAYING");
        *gamestate = 1;
        //send_game_state(*game_state);
    }
    /*----------------MENU LEVEL 2----------------*/
    else if(option_select == 2 && menu_level == 2 && joy_val_x < 0){
        menu_level = 0;
        option_select = 0;
    }
    /*----------------MENU LEVEL 3----------------*/
    else if(option_select == 4 && menu_level == 3 && joy_val_x < 0){
        menu_level = 0;
        option_select = 0;
    }

}
