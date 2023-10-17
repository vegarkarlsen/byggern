
#include "ADC_driver.h"
#include "MCP2515_driver.h"
#include "OLED_driver.h"
#include "OLED_menu.h"
#include "SPI_driver.h"
#include "SRAM_test.h"
#include "USART_driver.h"
#include "mcp2515.h"
#include "util.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include "CAN_controller.h"
/* #include "include/fonts.h" */

#define FOSC 4915200 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC / 16 / BAUD - 1
#define F_CPU 16000000

/* int8_t joy_x; */
/* int8_t joy_y; */
/* uint8_t slider_left; */
/* uint8_t slider_right; */
/**/

int option_select = 0;
int menu_level = 0;
/**/
/* bool p0 = 0; */
/* bool p1 = 0; */
/* bool p2 = 0; */
/**/
/* uint8_t test; */

/* void testMCP(){ */
/*     uint8_t write = 59; */
/*     uint8_t read; */
/*     MCP_write(); */
/*     read = MCP_read(); */
/**/
/*     printf("sending %2d\r\n", write); */
/*     printf("reading %2d\r\n", read); */
/**/
/**/
/* } */

void test_buffer_choice(uint8_t buffer){
    if (buffer > 2){
        buffer = 0;
    }
    uint8_t buffer_adress = 0x80;
    buffer_adress |= (1 << buffer);
    printf("buffer: %2d adress: 0x%X\r\n", buffer, buffer_adress);
}

void test_11_bit(){
    uint8_t high = 0x11;
    uint8_t low = 0x2;
    uint16_t data = (high << 3) | low;
    printf("high: %x, low: %x, data: %2d \r\n", high, low, data);

}



int main(void) {
    USART0_init(MYUBRR);
    /* SRAM_init(); // this is genreal for databus  */
    // SRAM_test();
    /* ADC_timer_init(); */
    /* OLED_init(); */
    // need to define printf here to use it in other functions
    fdevopen(USART0_transmit, USART0_receive);

    /* MCP_init(); */
    CAN_init();

    /* calibrate_zero_point(10); */

    // OLED_clear_screen();
    // OLED_go_line(0);
    // OLED_print8("Htest123!");
    /* home_screen_print(); */

    /* SPI_master_init(); */
    /* uint8_t data; */
    while (1) {
        /* test_11_bit(); */

        canPack_t can_pack1 = {1, 6, "halla"};
        CAN_send(0, &can_pack1);
        canPack_t returned = CAN_revice(0);
        printf("-----------------------\r\n");
        /* printf("send_id 0x%x\r\n", can_pack.ID); */
        /* printf("returnd_id 0x%x\r\n", returned.ID); */
        /* printf("sending: %c%c%c\r\n", can_pack.data[0], can_pack.data[1], can_pack.data[2]); */
        /* printf("retunred: %c%c%c\r\n", returned.data[0], returned.data[1], returned.data[2]); */
        printf("ID: 0x%x\r\n", returned.ID);
        printf("len: 0x%x\r\n", returned.len);
        printf("Message: %c%c%c%c%c%c \r\n", returned.data[0], returned.data[1], returned.data[2], returned.data[3], returned.data[4], returned.data[5]);
        _delay_ms(50);

        CAN_send(0, &can_pack1);
        canPack_t returned2 = CAN_revice(0);
        printf("-----------------------\r\n");
        printf("ID: 0x%x\r\n", returned2.ID);
        printf("len: 0x%x\r\n", returned2.len);
        printf("Message: %c%c%c \r\n", returned2.data[0], returned2.data[1], returned2.data[2]);

        
        /* _delay_ms(500);  */
        /* MCP_write(0x41, 0xA7); */
        /* data = MCP_read(0x41); */
        /* printf("data= 0x%x\n\r", data); */
        /* SPI_transmit(0x69); */

        /* joy_x = read_joystick_channel_transformed(JOYSTICK_CHANNEL_X, 3); */
        /* joy_y = read_joystick_channel_transformed(JOYSTICK_CHANNEL_y, 3); */
        /* uint8_t raw_x = read_channel(JOYSTICK_CHANNEL_X); */
        /* uint8_t raw_y = read_channel(JOYSTICK_CHANNEL_y); */

        /* slider_left = read_channel(SLIDER_CHANNEL_LEFT); */
        /* slider_right = read_channel(SLIDER_CHANNEL_RIGHT); */

        // int8_t joy_x_per = map(joy_x, 0, 255, 100, -100);
        // int8_t joy_y_per = map(joy_y, 0, 255, 100, -100);

        // struct Position pos = calculate_direction_joy(joy_x_per, joy_y_per);

        // screen
        /* option_change(joy_y); */
        /* home_screen_print(); */

        // read button values
        /* p0 = PINB & (1 << PB0); */
        /* p1 = PINB & (1 << PB1); */
        /* p2 = !(PINB & (1 << PB2)); */
        /* if (p2 == 1) { */
        /*     menu_level_select(); */
        /* } */

        /* printf("3 : %2d\r\n", test); */
        /* printf("%2d \r\n", PINB); */
        /* printf("p0: %2d p1: %2d p2: %2d \r\n", p0, p1, p2); */
        /* printf("joysick (x, y): (%2d, %2d) \r\n", joy_x, joy_y); */
        /* printf("joysick (x, y): (%2d, %2d) \r\n", raw_x, raw_y); */
        // printf("slider (LEFT, RIGHT): (%2d, %2d) \r\n", slider_left,
        // slider_right);
    }
}
