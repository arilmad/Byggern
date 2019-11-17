#ifndef __ENCODER_H__
#define __ENCODER_H__

#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <stdint.h>
#include <avr/io.h>
#include <avr/delay.h>

void encoder_init(void);
int16_t encoder_read(void);

#endif