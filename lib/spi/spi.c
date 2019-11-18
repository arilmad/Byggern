#include "spi.h"

#include <avr/interrupt.h>
#include <avr/io.h>

//Initializes the Atmega162 to be a master.
void spi_init(void)
{
#if defined(__AVR_ATmega162__)
    DDRB |= (1 << PB5) | (1 << PB7) | (1 << PB4);
    SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
    PORTB |= (1 << PB4);
#elif defined(__AVR_ATmega2560__)
    DDRB |= (1 << PB2) | (1 << PB1) | (1 << PB7) | (1 << PB0);
    SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
#endif
}

void spi_activate_chipselect(void)
{
#if defined(__AVR_ATmega162__)
    PORTB &= ~(1 << PB4);
#elif defined(__AVR_ATmega2560__)
    PORTB &= ~(1 << PB7);
#endif
}

void spi_deactivate_chipselect(void)
{
#if defined(__AVR_ATmega162__)
    PORTB |= (1 << PB4);
#elif defined(__AVR_ATmega2560__)
    PORTB |= (1 << PB7);
#endif
}

//Sends one byte of data on the serial line.
void spi_transmit(char data)
{
    SPDR = data;
    while (!(SPSR & (1 << SPIF)))
        ;
}

char spi_receive(void)
{
    spi_transmit(0xFF);
    while (!(SPSR & (1 << SPIF)))
        ;
    return SPDR;
}
