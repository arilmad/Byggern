#ifndef __PID_H__
#define __PID_H__

#define U_MAX 255
#define U_MIN -255

#define SCALING_FACTOR 1024
#define MAX_I_TERM INT32_MAX / 2

#include <stdint.h>

#include "../../lib/macros.h"

void pid_init(int16_t Kp_, int16_t Ki_, int16_t Kd_);
int16_t pid_calculate_u(int16_t ref, int16_t pos);

#endif