#if defined(__AVR_ATmega162__)
#define F_CPU 4915200
#elif defined(__AVR_ATmega2560__)
#define F_CPU 16000000
#endif

#include "MCP2515.h"
#include "../spi/spi.h"

#include <util/delay.h>

void mcp2515_reset()
{
    spi_activate_chipselect();
    spi_transmit(MCP_RESET);
    spi_deactivate_chipselect();
    _delay_ms(20);
}

void mcp2515_modify(char address, char mask, char mode)
{
    spi_activate_chipselect();
    spi_transmit(MCP_BITMOD);
    spi_transmit(address);
    spi_transmit(mask);
    spi_transmit(mode);
    spi_deactivate_chipselect();
}

void mcp2515_init(char mode)
{
    spi_init();
    mcp2515_reset();
    mcp2515_modify(MCP_CANCTRL, MODE_MASK, mode);
}

char mcp2515_read(char address)
{
    char data;
    spi_activate_chipselect();
    spi_transmit(MCP_READ);
    spi_transmit(address);
    data = spi_receive();
    spi_deactivate_chipselect();
    return data;
}

void mcp2515_write(char address, char data)
{
    spi_activate_chipselect();
    spi_transmit(MCP_WRITE);
    spi_transmit(address);
    spi_transmit(data);
    spi_deactivate_chipselect();
}

char mcp2515_status(void)
{
    char status;
    spi_activate_chipselect();
    spi_transmit(MCP_READ_STATUS);
    status = spi_receive();
    spi_deactivate_chipselect();
    return status;
}

void mcp2515_rts(char buffer)
{
    spi_activate_chipselect();
    spi_transmit(buffer);
    spi_deactivate_chipselect();
}