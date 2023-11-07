#include "Multiboard_tools.h"
#include "ADC_driver.h"
#include "CAN_controller.h"
#include "USART_driver.h"
#include <stdint.h>

void send_Multiboard_to_CAN(canPack_t message) {
    //Data 0 = x axis joystick
    //Data 1 = y axis joystick
    //Data 2 = 
    //Data 3 = 
    //Data 4 = 
    //Data 5 = slider left
    //Data 6 = slider right
    int8_t x_joy = read_joystick_channel_transformed(JOYSTICK_CHANNEL_X, 3);
    int8_t y_joy = read_joystick_channel_transformed(JOYSTICK_CHANNEL_y, 3);
    bool button_1 = PINB & (1 << PB0);
    bool button_2 = PINB & (1 << PB1);
    bool button_3 = !(PINB & (1 << PB2));
    uint8_t slider_left = (uint8_t)read_channel(SLIDER_CHANNEL_LEFT);
    uint8_t slider_right = (uint8_t)read_channel(SLIDER_CHANNEL_RIGHT);
    message.ID = 7;
    message.len = 7;
    message.data[0] = x_joy;
    message.data[1] = y_joy;
    message.data[2] = button_1;
    message.data[3] = button_2;
    message.data[4] = button_3;
    message.data[5] = slider_left;
    message.data[6] = slider_right;
    printf("%d\n\r", (int8_t)message.data[0]);
    CAN_print(&message);
    CAN_send(&message, 1);
}


goal_pack_t unpack_goal_pack(canPack_t canpack){
    goal_pack_t g;
    g.goals = canpack.data[0];
    g.higscore = (canpack.data[1] << 8) | canpack.data[2];
    return g;
}

void send_game_state(uint8_t game_state){
    canPack_t game_state_pack;
    game_state_pack.ID = 1;
    game_state_pack.len = 1;
    game_state_pack.data[0] = game_state;
    CAN_send(&game_state_pack, 2);
}



