/* adc.h
    * This part of the program reads
    * analog values from the I/O card
    * and converts them to digital values.
*/

#ifndef __ADC_H__
#define __ADC_H__

#include <stdint.h>

/* Channels to read from the ADC */
typedef enum
{
    JOY_X = 4,
    JOY_Y = 5,
    SLIDER_L = 6,
    SLIDER_R = 7
} adc_channel_t;

/* adc_read(uint8_t channel)
    * Reads a message from the external analog 
    * to digital converter on a given channel.
*/
uint8_t adc_read(uint8_t channel);

#endif