#ifndef __SERVO_H__
#define __SERVO_H__

#include <stdint.h>

void servo_init(void);
void servo_set_pos(uint8_t pos);

#endif