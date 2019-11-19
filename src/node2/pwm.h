/* pwm.h
    * This part of the program is used
    * to generate a PWM signal used
    * to control the servo controlling 
    * the racket's angle.
*/

#ifndef __PWM_H__
#define __PWM_H__

#include <stdint.h>

void pwm_init(uint16_t duty_cycle);
void pwm_set_duty_cycle(uint16_t duty_cycle);

#endif