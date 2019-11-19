/* xmem.h
    * This part of the program is used
    * to enable and utilize address space
    * outside the internal SRAM of the 
    * MCU.
*/

#ifndef __XMEM_H__
#define __XMEM_H__

#include <stdint.h>

/* 
    * Defines the base addresses 
    * for the different external 
    * memory locations.
*/
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