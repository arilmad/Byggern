#include "can_driver.h"
#include "../spi/spi.h"
#include "MCP2515.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/common.h>

void can_init(char mode)
{
    mcp2515_init(mode);
}

void can_setmode(char mode)
{
    mcp2515_modify(MCP_CANCTRL, MODE_MASK, mode);
}

uint8_t can_message_read(can_message_t* message)
{
    if (!(mcp2515_read(MCP_CANINTF) & 0x01))
        return 0; // Nothing to read.

    int8_t tp = ( mcp2515_read(MCP_RXB0SIDH) );

    message->id = (uint8_t)tp;
    message->data_length = mcp2515_read(MCP_RXB0DLC);

    for (uint8_t i = 0; i < message->data_length; i++)
        message->data[i] = mcp2515_read(MCP_RXB0D0 + i);

    mcp2515_modify(MCP_CANINTF, 0x01, 0x00);

    return 1;
}

void can_message_send(can_message_t* message)
{
    mcp2515_write(MCP_TXB0SIDH, message->id);
    mcp2515_write(MCP_TXB0SIDL, 0x00);

    mcp2515_write(MCP_TXB0DLC, 0x0F & message->data_length);

    for (uint8_t i = 0; i < message->data_length; i++)
        mcp2515_write(MCP_TXB0D0 + i, message->data[i]);

    mcp2515_rts(MCP_RTS_TX0);
}
