#include "spi.h"

//Initializes the Atmega162 to be a master.
void spi_master_init(void)
{
    DDR_SPI = (1<<DD_MOSI) | (1<<DD_SCK);
    SPCR = (1<<SPE) | (1<<MSTR)  (1<<SPR0);
}
//Initializes the Atmega162 to be a slave.
void spi_slave_init(void)
{
    DDR_SPI = (1<<DD_MISO);
    SPCR = (1<<SPE);
}

//TODO: Make a timeer interrupt routine.

//TODO: Enable interrupt on SPIF

//Sends one byte of data on the serial line.
void spi_master_transmit(char data)
{
    //Maybe configure ~SS as an output to make sure this device is always a master.
    //Otherwise it might become a slave when ~SS is driven low by another device.
    SPDR = data;
    while(!(SPSR & (1<<SPIF)));
}

//Receives one byte of data at the time.
char spi_slave_receive(void)
{
    //Configure ~SS as an input?
    while(!(SPSR & (1<<SPIF)));
    return SPDR;
}