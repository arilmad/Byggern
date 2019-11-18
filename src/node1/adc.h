#ifndef __ADC_H__
#define __ADC_H__

/* Channels to read from the ADC */
typedef enum
{
    JOY_X = 4,
    JOY_Y = 5,
    SLIDER_L = 6,
    SLIDER_R = 7
} adc_channel_t;

/* 
    * Reads a message from the external analog 
    * to digital converter on a given channel.
*/
uint8_t adc_read(uint8_t channel);

#endif