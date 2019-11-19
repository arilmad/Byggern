/* servo.h
    * This part of the program is used
    * to convert a relative servo
    * position to a value fit for
    * setting the duty cycle and
    * controlling the specific 
    * servo used in this project.
*/

#ifndef __SERVO_H__
#define __SERVO_H__

#include <stdint.h>

void servo_init(void);
void servo_set_pos(uint8_t pos);

#endif