
#include "can_handling.h"
#include <stdint.h>
#include "can_controller.h"

/*
  NOTE: The apporach with shared Variables may create a race condition.
*/

// Variables that are sent through can 
// is shared with main loop and can_handler
multiboard_t multiboard;
uint8_t goals;
uint8_t game_state;

CAN_MESSAGE goal_pack = { 
    .id = 2,
    .data_length = 2,
    .data[0] = 0, // goals
    .data[1] = 0,  // time alive high byte
    .data[2] = 0 // time alive low byte
};

void update_multiboard_vars(CAN_MESSAGE *message){

    multiboard_t *multiboard = get_global_multiboard_vars();

    multiboard->joystick_x = message->data[0];
    multiboard->joystick_y = message->data[1];
    multiboard->left_button = message->data[2];
    multiboard->right_button = message->data[3];
    multiboard->joystick_button = message->data[4];
    multiboard->slider_left = message->data[5];
    multiboard->slider_right = message->data[6];
}

multiboard_t *get_global_multiboard_vars(){
    return &multiboard;
}

uint8_t *get_goals_global(){
    return &goals;
}

CAN_MESSAGE prepere_goals_package(){
    uint8_t *g = get_game_state_global();
    // goal_pack.id = 2;
    // goal_pack.data_length = 1;
    goal_pack.data[0] = *g;
    return goal_pack;
}

void update_higscore(uint16_t game_alive){
    goal_pack.data[1] = (uint8_t)(game_alive >> 8); // high byte
    goal_pack.data[2] = (uint8_t)game_alive; // low byte
}

void update_game_state_vars(CAN_MESSAGE *message){ 
    uint8_t *gs = get_game_state_global();
    *gs = message->data[0];
}

uint8_t *get_game_state_global(){
    return &game_state;
}






