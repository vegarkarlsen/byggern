#include "ADC_driver.h"
#include "util.h"
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

void ADC_timer_init() {
    DDRD |= (1 << PD5);
    TCCR1A = (1 << COM1A0);              // Toggle-mode
    TCCR1B = (1 << WGM12) | (1 << CS10); // CTSmode, No scaler
    OCR0 = 1;
}

uint8_t read_channel(uint8_t channel) {

    // check if channel is valid
    if (channel > 3 | channel < 0) {
        // TODO: mabye provoke an error
        return -1;
    }

    volatile char *adc = (char *)ADC_START_ADRESS;

    adc[0] = 0x00; // initialize reading
    _delay_ms(50); // delay need to be more than t_CONV (see adc datasheet)

    // store reading in array
    uint8_t readings[4];
    for (int i = 0; i < 4; i++) {
        readings[i] = adc[i];
    }
    return readings[channel];
}

struct Position calculate_direction_joy(int8_t percentage_x,
                                        int8_t percentage_y) {

    int8_t offset_x = 34;
    int8_t threshold_x = 5;
    int8_t offset_y = 34;
    int8_t threshold_y = 5;

    // return data
    struct Position pos;

    // left
    if (percentage_x > offset_x + threshold_x) {
        pos.x = RIGHT;
    }
    // right
    else if (percentage_x < (offset_x)-threshold_x) {
        pos.x = LEFT;
    } else {
        pos.x = NONE;
    }

    // up
    if (percentage_y > (offset_y) + threshold_y) {
        pos.y = UP;
    } else if (percentage_y < (offset_y)-threshold_y) {
        pos.y = DOWN;
    } else {
        pos.y = NONE;
    }

    return pos;
}

int8_t analog_to_persentage_joy(uint8_t input, int8_t offset) {

    int8_t output;

    // map up
    if (input > 173 + offset) {
        output = map(input, 173 + offset, 255, 100, 0);
    }
    // map down
    else if (input < 173 - offset) {
        output = map(input, 173 - offset, 3, -100, 0);
    } else {
        output = 0;
    }

    return output;
}
