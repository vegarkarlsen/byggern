#pragma once

#include "pio/pio_sam3x8e.h"
#include "uart_and_printf/printf-stdarg.h"
#include "uart_and_printf/uart.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#include "sam.h"
#include "can_controller.h"

            /* int8_t x = message.data[0]; */
            /* int8_t y = message.data[1]; */
            /* uint8_t button_1 = message.data[2]; */
            /* uint8_t button_2 = message.data[3]; */
            /* uint8_t button_3 = message.data[4]; */
            /* uint8_t slider_left = message.data[5]; */
            /* uint8_t slider_right = message.data[6]; */

typedef struct multiboard_t{
    int8_t joystick_x;
    int8_t joystick_y;
    uint8_t left_button;
    uint8_t right_button;
    uint8_t joystick_button;
    uint8_t slider_left;
    uint8_t slider_right;
} multiboard_t;


void update_multiboard_vars(CAN_MESSAGE *message);
multiboard_t *get_global_multiboard_vars();
