#include "spi.h"

ISR(INT1_vect)
{
    spi_interrupt_flag = 1;
}

void spi_interrupt_init(void)
{
    GICR |= (1 << INT1);
    MCUCR |= (1 << ISC11);
    MCUCR &= ~(1 << ISC10);
}

//Initializes the Atmega162 to be a master.
void spi_init(void)
{
    DDRB |= (1 << PB5) | (1 << PB7) | (1 << PB4);
    SPCR |= (1 << SPE) | (1 << MSTR) | (1<<SPR0) | (1<<SPR1);
    //SPCR &= ~(1 << SPR1) | (1<<SPR0);
    spi_interrupt_init();
}

void spi_activate_chipselect(void)
{
    PORTB &= ~(1 << PB4);
}

void spi_deactivate_chipselect(void)
{
    PORTB |= (1 << PB4);
}
//TODO: Make a timeer interrupt routine.

//TODO: Enable interrupt on SPIF

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
    while (!(SPSR & (1 << SPIF)))
        ;
    return SPDR;
}

