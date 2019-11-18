#include "xmem.h"

#include <stdint.h>
#include <avr/io.h>
/* void xmem_init()
    * Initialize the registers used 
    * by the external memory.
*/
void xmem_init()
{
    MCUCR |= (1 << SRE);
    SFIOR |= (1 << XMM2);
}

/* xmem_write(uint8_t data, uint16_t addr, base_address_t base_address)
    * Writes data to the address 
    * offset from the base address.
 */
void xmem_write(uint8_t data, uint16_t addr, base_address_t base_address)
{
    volatile char *ext_mem = (char *)base_address;
    ext_mem[addr] = data;
}

/* uint8_t xmem_read(uint16_t addr, base_address_t base_address)
    * Reads data from the address
    * offset from the base address.
*/
uint8_t xmem_read(uint16_t addr, base_address_t base_address)
{
    volatile char *ext_mem = (char *)base_address;
    uint8_t ret_val = ext_mem[addr];
    return ret_val;
}