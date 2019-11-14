#include "solenoid.h"
#include <util/delay.h>

void solenoid_init()
{
    DDRB |= (1 << PB4);
    PORTB |= (1 << PB4);
}

void solenoid_trigger()
{
    PORTB &= (~( 1 << PB4 ));
    _delay_ms(25);
    PORTB |= (1 << PB4);
}