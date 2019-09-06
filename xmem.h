#include <stdint.h>
#include <avr/io.h>

#define BASE_ADDRESS 0x1000

#ifndef _XMEM_H_
#define _XMEM_H_

void xmem_init( void );
void xmxm_write(uint8_t data, uint16_t addr);
uint8_t xmem_read(uint16_t addr);

#endif