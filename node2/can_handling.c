
#include "can_handling.h"



// global multiboard-

multiboard_t multiboard;

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
