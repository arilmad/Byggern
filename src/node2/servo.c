#include "servo.h"

void servo_init()
{
    pwm_init(2950);
}

void servo_set_pos(uint8_t pos)
{
    if (pos < 0){pos = 0;}
    else if (pos > 100){pos = 100;}

    uint16_t pwm_value = (int)(2950 + (pos*39));

    pwm_set_duty_cycle(pwm_value);
}