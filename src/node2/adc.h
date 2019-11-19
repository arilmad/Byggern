/* adc.h
    * This part of the program is used
    * by ir.c to interpret the analog
    * IR-sensor signal.
*/

#ifndef __ADC_H__
#define __ADC_H__

#include <stdint.h>

void adc_init(void (*f)(uint8_t));

#endif