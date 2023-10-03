
#include "util.h"
#include <stdint.h>

long map(long input, long i_min, long i_max, long o_max, long o_min) {
    return (input - i_min) * (o_max - o_min) / (i_max - i_min) + o_min;
}


void set_bit(uint8_t *byte, uint8_t bit){
   *byte = (1 << bit);
}
