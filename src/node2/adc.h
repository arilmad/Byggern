#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef __ADC_H__
#define __ADC_H__

void adc_init(void (*f)(uint8_t));

#endif