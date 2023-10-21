#include "JOYSTICK.h"
#include "CAN_controller.h"
#include "ADC_driver.h"


void send_JOYSTICK_to_CAN(uint8_t x, uint8_t y, canPack_t message){
    x = read_joystick_channel_transformed(JOYSTICK_CHANNEL_X, 3);
    y = read_joystick_channel_transformed(JOYSTICK_CHANNEL_y, 3);
    message.ID = 7;
    message.len = 2;
    message.data[0] = x;
    message.data[1] = y;
    CAN_print(message);
    CAN_send(0, &message);
}