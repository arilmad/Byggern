#include "adc.h"

#define ext_adc ((volatile char *)0x1400)

uint8_t adc_read(uint8_t channel)
{
    *ext_adc = channel;
    _delay_ms(40);
    return ext_adc[0];
}