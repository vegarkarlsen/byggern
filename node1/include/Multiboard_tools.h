#pragma once

#include "CAN_controller.h"
#include "ADC_driver.h"
#include <stdint.h>

//function to create can message for node 2
void send_Multiboard_to_CAN(canPack_t message);

typedef struct goal_pack {
    uint8_t goals;
    uint16_t higscore;
    
} goal_pack_t;

goal_pack_t unpack_goal_pack(canPack_t canpack);
void send_game_state(uint8_t game_state);
