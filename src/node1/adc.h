#ifndef __ADC_H__
#define __ADC_H__

typedef enum
{
    JOY_X = 4,
    JOY_Y = 5,
    SLIDER_L = 6,
    SLIDER_R = 7
} adc_channel_t;

uint8_t adc_read(uint8_t channel);

#endif