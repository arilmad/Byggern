#include <avr/io.h>
#include <stdint.h>

#include "pwm.h"

void servo_init();

void servo_set_pos(uint8_t pos);