#pragma once
#include <avr/io.h>
/* #include <cstdint> */
/* #include <cstdint> */
#include <stdlib.h>

long map(long input, long i_min, long i_max, long o_max, long o_min);

// TODO: fix this functions
void set_bit(uint8_t *byte, uint8_t bit);

void clear_bit(char *byte, uint8_t bit);
