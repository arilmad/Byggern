#include "servo.h"
#include "pwm.h"
#include "../../lib/macros.h"

#include <avr/io.h>
#include <stdint.h>

void servo_init()
{
    pwm_init(2950);
}

void servo_set_pos(uint8_t pos)
{
    pos = max(pos, 0);
    pos = min(pos, 255);

    uint16_t pwm_value = (int)(2950 + (pos * 15));

    pwm_set_duty_cycle(pwm_value);
}