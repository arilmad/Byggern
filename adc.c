#include <avr/io.h>
#include <util/delay.h>
#include "adc.h"
#include "uart.h"

#define ext_adc ((volatile char*) 0x1400)


void adc_init( void )
{
    DDRB &= ~(1<<DDB3);
}

uint8_t adc_read(uint8_t channel)
{
    *ext_adc = channel;
    _delay_ms(40);
    uint8_t adc_read = ext_adc[0];
}