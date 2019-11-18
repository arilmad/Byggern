#ifndef __ENCODER_H__
#define __ENCODER_H__

#include <stdint.h>

void encoder_reset();
void encoder_init(void);
int16_t encoder_read(void);

#endif