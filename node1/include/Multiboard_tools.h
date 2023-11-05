#pragma once

#include "CAN_controller.h"
#include "ADC_driver.h"

//function to create can message for node 2
void send_Multiboard_to_CAN(canPack_t message);