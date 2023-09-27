
#include "ADC_driver.h"
#include "OLED_driver.h"
#include "OLED_menu.h"
#include "SRAM_test.h"
#include "USART_driver.h"
#include "util.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
// #include "include/fonts.h"

#define FOSC 4915200 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC / 16 / BAUD - 1
#define F_CPU 16000000

int8_t joy_x;
int8_t joy_y;
uint8_t slider_left;
uint8_t slider_right;

int option_select = 0;
int menu_level = 0;

bool p0 = 0;
bool p1 = 0;
bool p2 = 0;

int main(void) {
    USART0_init(MYUBRR);
    SRAM_init(); // this is genreal for databus
    // SRAM_test();
    ADC_timer_init();
    OLED_init();
    // need to define printf here to use it in other functions
    fdevopen(USART0_transmit, USART0_receive);
    calibrate_zero_point(10);

    // OLED_clear_screen();
    // OLED_go_line(0);
    // OLED_print8("Htest123!");
    home_screen_print();

    while (1) {
        _delay_ms(500);

        joy_x = read_joystick_channel_transformed(JOYSTICK_CHANNEL_X, 3);
        joy_y = read_joystick_channel_transformed(JOYSTICK_CHANNEL_y, 3);
        uint8_t raw_x = read_channel(JOYSTICK_CHANNEL_X);
        uint8_t raw_y = read_channel(JOYSTICK_CHANNEL_y);

        /* slider_left = read_channel(SLIDER_CHANNEL_LEFT); */
        /* slider_right = read_channel(SLIDER_CHANNEL_RIGHT); */

        // int8_t joy_x_per = map(joy_x, 0, 255, 100, -100);
        // int8_t joy_y_per = map(joy_y, 0, 255, 100, -100);

        // struct Position pos = calculate_direction_joy(joy_x_per, joy_y_per);

        // screen
        option_change(joy_y);
        home_screen_print();

        // read button values
        p0 = PINB & (1 << PB0);
        p1 = PINB & (1 << PB1);
        p2 = !(PINB & (1 << PB2));
        if (p2 == 1) {
            menu_level_select();
        }

        /* printf("3 : %2d\r\n", test); */
        printf("%2d \r\n", PINB);
        printf("p0: %2d p1: %2d p2: %2d \r\n", p0, p1, p2);
        /* printf("joysick (x, y): (%2d, %2d) \r\n", joy_x, joy_y); */
        /* printf("joysick (x, y): (%2d, %2d) \r\n", raw_x, raw_y); */
        // printf("slider (LEFT, RIGHT): (%2d, %2d) \r\n", slider_left,
        // slider_right);
    }
}
