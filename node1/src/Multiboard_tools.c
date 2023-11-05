#include "Multiboard_tools.h"
#include "ADC_driver.h"
#include "CAN_controller.h"
#include "USART_driver.h"

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
