#ifndef __SOLENOID_H__
#define __SOLENOID_H__

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

void solenoid_init(void);
void solenoid_trigger(void);

#endif