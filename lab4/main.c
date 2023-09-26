
#include "ADC_driver.h"
#include "OLED_driver.h"
#include "SRAM_test.h"
#include "USART_driver.h"
#include "util.h"
#include "OLED_menu.h"
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

uint8_t joy_x = -1;
uint8_t joy_y = -1;

int option_select = 0;
int menu_level = 0;

uint8_t slider_left = -1;
uint8_t slider_right = -1;
bool p0 = 0;

void test_cs_nand_logic() {

    DDRC = 0b1111;
    printf("setting all cs bits as output\r\n");

    for (uint8_t i = 1; i < 16; i += 4) {
        PORTC = i;

        printf("setting PORTC  %2d \r\n", i);
        _delay_ms(50000);
    }
}

int main(void) {
    USART0_init(MYUBRR);
    SRAM_init(); // this is genreal for databus
    // SRAM_test();
    ADC_timer_init();
    OLED_init();
    // need to define printf here to use it in other functions
    fdevopen(USART0_transmit, USART0_receive);
    /* calibrate_zero_point(10); */


  // OLED_clear_screen();
  //OLED_go_line(0);
  //OLED_print8("Htest123!");
  home_screen_print();


  while (1) {
    _delay_ms(500);

    joy_x = read_channel(0);
    joy_y = read_channel(1);

    slider_left = read_channel(2);
    slider_right = read_channel(3);

        // int8_t joy_x_per = map(joy_x, 0, 255, 100, -100);
        // int8_t joy_y_per = map(joy_y, 0, 255, 100, -100);

        // struct Position pos = calculate_direction_joy(joy_x_per, joy_y_per);

    int8_t joy_x_per = analog_to_persentage_joy(joy_x, 3);
    option_change(analog_to_persentage_joy(joy_y,3));
    home_screen_print();
    int8_t joy_y_per = analog_to_persentage_joy(joy_y, 3);
    uint8_t p0 = PINB >> 0;
    if(p0==1){
      menu_level_select();
    }

    // printf("joysick (x, y): (%2d, %2d) \r\n",joy_x_per, joy_y_per);
    // // printf("joysick (x, y): (%2d, %2d) \r\n",joy_x, joy_y);
    // // printf("joysick (x, y): (%2d, %2d) \r\n",pos.x, pos.y);
    // printf("slider (LEFT, RIGHT): (%2d, %2d) \r\n", slider_left,
    // slider_right);
  }
}
