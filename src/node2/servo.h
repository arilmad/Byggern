#ifndef __SERVO_H__
#define __SERVO_H__

#include <avr/io.h>
#include <stdint.h>

#include "pwm.h"
#include "../../lib/macros.h"

void servo_init(void);
void servo_set_pos(uint8_t pos);

#endif