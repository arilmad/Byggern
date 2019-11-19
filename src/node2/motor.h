/* motor.h
    * This part of the program communicates
    * with the motor controlling the racket's
    * horizontal position.
*/

#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <stdint.h>

void motor_init(void);
void motor_drive(int16_t speed);
void motor_stop(void);
uint16_t motor_calibrate(void);

#endif