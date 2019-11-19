#define F_CPU 16000000

#include "solenoid.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

/* solenoid_init()
    * Set PB4 as output to cotrol
    * the solenoid.
*/
void solenoid_init()
{
    DDRB |= (1 << PB4);
    PORTB |= (1 << PB4);
}

/* solenoid_trigger()
    * Pull PB4 low for 25ms to
    * trigger the relay supplying
    * the solenoid with +16V.
*/
void solenoid_trigger()
{
    PORTB &= (~(1 << PB4));
    _delay_ms(25);
    PORTB |= (1 << PB4);
}