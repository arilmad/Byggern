#ifndef __SPI_H__
#define __SPI_H__

#include <avr/interrupt.h>
#include <avr/io.h>

void spi_init(void);

void spi_transmit(char data);
char spi_receive(void);
void spi_activate_chipselect(void);
void spi_deactivate_chipselect(void);

#endif