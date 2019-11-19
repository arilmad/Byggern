#define PWM_LOW_VALUE 2950

#include "servo.h"
#include "pwm.h"
#include "../../lib/macros.h"

#include <avr/io.h>

/* servo_init()
    * Initialize servo with leftmost
    * value.
*/
void servo_init()
{
    pwm_init(PWM_LOW_VALUE);
}

/* servo_set_pos()
    * Receives a relative servo position from
    * 0 to 255 and calculates a value fit
    * to set the pwm duty cycle.
*/
void servo_set_pos(uint8_t pos)
{
    pos = max(pos, 0);
    pos = min(pos, 255);

    uint16_t pwm_value = (int)(PWM_LOW_VALUE + (pos * 15));

    pwm_set_duty_cycle(pwm_value);
}