#define F_CPU 4915200
#define ext_adc ((volatile char *)0x1400)

#include "adc.h"

#include <avr/io.h>
#include <util/delay.h>

/* adc_read(uint8_t channel)
    * Reads the external memory address
    * of the analog to digital converter.
*/
uint8_t adc_read(uint8_t channel)
{
    *ext_adc = channel;
    _delay_ms(40);
    return ext_adc[0];
}