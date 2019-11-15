#ifndef __XMEM_H__
#define __XMEM_H__

#include <stdint.h>
#include <avr/io.h>

typedef enum
{
    OLED_COMMAND_ADDRESS = 0x1000,
    OLED_DATA_ADDRESS = 0x1200,
    ADC_ADDRESS = 0x1400,
    SRAM_ADDRESS = 0x1800
} base_address_t;

void xmem_init(void);
void xmem_write(uint8_t data, uint16_t addr, base_address_t base_address);
uint8_t xmem_read(uint16_t addr, base_address_t base_address);

#endif