#pragma once

#include "CAN_controller.h"

void send_JOYSTICK_to_CAN(uint8_t x, uint8_t y, canPack_t message);