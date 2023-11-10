
#include "ADC_driver.h"
#include "CAN_controller.h"
#include "Multiboard_tools.h"
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
/* #include "include/fonts.h" */

#define FOSC 4915200 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC / 16 / BAUD - 1
#define F_CPU 16000000

/**/

int option_select = 0;
int menu_level = 0;
int8_t joy_y;
int8_t joy_x;
uint8_t game_state = 0;
uint16_t highscore = 0;
bool button_left;

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

void test_buffer_choice(uint8_t buffer) {
    if (buffer > 2) {
        buffer = 0;
    }
    uint8_t buffer_adress = 0x80;
    buffer_adress |= (1 << buffer);
    printf("buffer: %2d adress: 0x%X\r\n", buffer, buffer_adress);
}

void test_11_bit() {
    uint8_t high = 0x11;
    uint8_t low = 0x2;
    uint16_t data = (high << 3) | low;
    printf("high: %x, low: %x, data: %2d \r\n", high, low, data);
}
void revice_and_print_node2(){

    canPack_t t;
    CAN_revice(&t, 1);
    printf("id: %d\n\r", t.ID);
    printf("len: %d\n\r", t.len);
    printf("data: %d\n\r", t.data[0]);
}

int main(void) {
    USART0_init(MYUBRR);
    SRAM_init(); // this is genreal for databus
    ADC_timer_init();
    OLED_init();
    // need to define printf here to use it in other functions
    fdevopen(USART0_transmit, USART0_receive);

    /* MCP_init(); */
    /* MCP_modify_bit(MCP_CANCTRL, 0xE0, MODE_LOOPBACK); */

    /* MCP_init_loopBack(); */
    CAN_init();

    calibrate_zero_point(10);

    // OLED_clear_screen();
    // OLED_go_line(0);
    /* home_screen_print(); */

    /* SPI_master_init(); */
    /* uint8_t data; */
    /* canPack_t message = { */
    /*     7, */
    /*     5, */
    /*     "halla" */
    /* }; */
    canPack_t message;
    canPack_t revice_msg;
    while (1) {
        /*----------BEFORE GAME START-----------*/
        _delay_ms(500);
        joy_x = read_joystick_channel_transformed(JOYSTICK_CHANNEL_X, 20);
        joy_y = read_joystick_channel_transformed(JOYSTICK_CHANNEL_y, 20);
        // printf(joy_y);
        option_change(joy_y);
        home_screen_print();
        menu_level_select(joy_x, &game_state);
        send_Multiboard_to_CAN(&message, game_state);

        // CAN_revice(&revice_msg, 0);
        // CAN_print(&revice_msg);

        // revice_and_print_node2();
        while(game_state){
            send_Multiboard_to_CAN(&message, game_state);
            _delay_ms(100);
            // exit game
            if (message.data[3] == 1){
                game_state = 0;
            }
        }

        /*------------AFTER GAME START---------*/
        //while game alive: do messages n stuff

        //send_Multiboard_to_CAN(message);
        
        //revice_and_print_nod1();
        

        /* message = CAN_revice(0); */
        /* printf("ID: %d\r\nlen %d\r\ndata: %c%c\r\n", message.ID, message.len,
         * message.data[0],message.data[1]); */
        /* message.ID = 5; */
        /* message.len = 2; */
        /* message.data[0] = 17; */
        /* message.data[1] = 57; */
    }
}
