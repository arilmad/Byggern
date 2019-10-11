#ifndef __SPI_H__
#define __SPI_H__

#include <avr/io.h>

void spi_master_init(void);
void spi_slave_init(void);

void spi_master_transmit(char data);
char spi_slave_receive(void);

#endif