/* pid.h
    * This part of the program calculates
    * control signal u for the horizontal
    * position of the racket based on a
    * reference and current position.
*/

#ifndef __PID_H__
#define __PID_H__

#include <stdint.h>

void pid_init(int16_t Kp_, int16_t Ki_, int16_t Kd_);
int16_t pid_calculate_u(int16_t ref, int16_t pos);

#endif