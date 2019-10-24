#include <stdint.h>
#include <avr/io.h>

#include "pwm.h"

void servo_init();

void servo_set_pos(uint8_t pos);