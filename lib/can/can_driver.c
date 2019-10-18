#include "can_driver.h"

ISR(INT1_vect)
{
    can_interrupt_flag = 1;
}

void can_init(char mode)
{
    mcp2515_init(mode);
    mcp2515_write(MCP_CANINTE, 0x05); // Enable Transmit and Receive buffer interrupt.
}

void can_setmode(char mode)
{
    mcp2515_modify(MCP_CANCTRL, MODE_MASK, mode);
}

uint8_t can_message_read(can_message_t* message)
{
    if (!(mcp2515_read(MCP_CANINTF) & 0x01))
    {
        return 1; // Nothing to read.
    }
    message->id = (uint8_t)(mcp2515_read(MCP_RXB0SIDH) << 3 | mcp2515_read(MCP_RXB0SIDL) >> 5);
    message->data_length = mcp2515_read(MCP_RXB0DLC);
    for (uint8_t i = 0; i < message->data_length; i++)
    {
        message->data[i] = mcp2515_read(MCP_RXB0D0 + i);
    }
    mcp2515_modify(MCP_CANINTF, 0x01, 0x00);
    return 0;
}

void can_message_send(can_message_t* message)
{
    mcp2515_write(MCP_TXB0SIDH, message->id >> 3);
    mcp2515_write(MCP_TXB0SIDL, message->id << 5);

    mcp2515_write(MCP_TXB0DLC, 0x0F & message->data_length);

    for (uint8_t i = 0; i < message->data_length; i++)
    {
        mcp2515_write(MCP_TXB0D0 + i, message->data[i]);
    }

    mcp2515_rts(MCP_RTS_TX0);
}
