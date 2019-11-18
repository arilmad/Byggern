#ifndef __PID_H__
#define __PID_H__

void pid_init(int16_t Kp_, int16_t Ki_, int16_t Kd_);
int16_t pid_calculate_u(int16_t ref, int16_t pos);

#endif