#include "spi.h"

ISR(INT2_vect)
{
    spi_interrupt_flag = 1;
}

void spi_interrupt_init(void)
{
#if defined(__AVR_ATmega162__)
    GICR |= (1 << INT1);
    MCUCR |= (1 << ISC11);
    MCUCR &= ~(1 << ISC10);
#elif defined(__AVR_ATmega2560__)
    //Code for spi interrupts here
#endif
}

//Initializes the Atmega162 to be a master.
void spi_init(void)
{
#if defined(__AVR_ATmega162__)
    DDRB |= (1 << PB5) | (1 << PB7) | (1 << PB4);
    SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
    //SPCR &= ~(1 << SPR1) | (1<<SPR0);
    PORTB |= (1 << PB4);
    spi_interrupt_init();
#elif defined(__AVR_ATmega2560__)
    DDR_SPI |= (1 << DD_MOSI) | (1 << DD_SCK);
    SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
#endif
}

void spi_activate_chipselect(void)
{
#if defined(__AVR_ATmega162__)
    PORTB &= ~(1 << PB4);
#elif defined(__AVR_ATmega2560__)
    PORTB &= ~(1 << PB0);
#endif
}

void spi_deactivate_chipselect(void)
{
#if defined(__AVR_ATmega162__)
    PORTB |= (1 << PB4);
#elif defined(__AVR_ATmega2560__)
    PORTB |= (1 << PB0);
#endif
}

//Sends one byte of data on the serial line.
void spi_transmit(char data)
{
    //Maybe configure ~SS as an output to make sure this device is always a master.
    //Otherwise it might become a slave when ~SS is driven low by another device.
    SPDR = data;
    while (!(SPSR & (1 << SPIF)))
        ;
}

//Receives one byte of data at the time.
char spi_receive(void)
{
    spi_transmit(0xFF);
    while (!(SPSR & (1 << SPIF)))
        ;
    return SPDR;
}
