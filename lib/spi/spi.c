#include "spi.h"

#include <avr/interrupt.h>
#include <avr/io.h>

/* spi_init()
    * Initialize SPI on either node.
    * Configure MCU as master. f_osc / 16
    * SCK frequency.
*/
void spi_init()
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

/* spi_activate_chipselect()
    * Set chip select pin low.
*/
void spi_activate_chipselect()
{
#if defined(__AVR_ATmega162__)
    PORTB &= ~(1 << PB4);
#elif defined(__AVR_ATmega2560__)
    PORTB &= ~(1 << PB7);
#endif
}

/* spi_deactivate_chipselect()
    * Set chip select pin high.
*/
void spi_deactivate_chipselect()
{
#if defined(__AVR_ATmega162__)
    PORTB |= (1 << PB4);
#elif defined(__AVR_ATmega2560__)
    PORTB |= (1 << PB7);
#endif
}

/* spi_transmit()
    * Sends one byte of data on
    * the serial line.
*/
void spi_transmit(char data)
{
    SPDR = data;
    while (!(SPSR & (1 << SPIF)))
        ;
}

/* spi_receive()
    * Receives one byte of data on
    * the serial line.
*/
char spi_receive(void)
{
    spi_transmit(0xFF);
    while (!(SPSR & (1 << SPIF)))
        ;
    return SPDR;
}
