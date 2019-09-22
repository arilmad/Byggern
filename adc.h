#ifndef _ADC_H_
#define _ADC_H_

#include <stdint.h>

typedef enum
{
    JOY_X = 4,
    JOY_Y = 5,
    SLIDER_L = 6,
    SLIDER_R = 7
} adc_channel_t;

uint8_t adc_read(uint8_t channel);
void adc_calibrate(void);
void pos_read(void);

#endif