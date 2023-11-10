#pragma once

#include "pio/pio_sam3x8e.h"
#include "uart_and_printf/printf-stdarg.h"
#include "uart_and_printf/uart.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#include "sam.h"
#include "can_controller.h"



typedef struct multiboard_t{
    int8_t joystick_x;
    int8_t joystick_y;
    uint8_t left_button;
    uint8_t right_button;
    uint8_t joystick_button;
    uint8_t slider_left;
    uint8_t slider_right;
    uint8_t game_state;
} multiboard_t;


void update_multiboard_vars(CAN_MESSAGE *message);
multiboard_t *get_global_multiboard_vars();


CAN_MESSAGE prepere_goals_package();
void update_game_state_vars(CAN_MESSAGE *message);
uint8_t *get_game_state_global();
uint8_t *get_goals_global();

void update_higscore(uint16_t game_alive);
