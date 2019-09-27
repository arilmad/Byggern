#include "can_driver.h"

ISR(INT1_vect)
{
    can_interrupt_flag = 1;
}

void can_init(void)
{
    mcp2515_init(MODE_LOOPBACK);

    GICR |= (1 << INT1);
    MCUCR |= (1 << ISC11);
    MCUCR &= ~(1 << ISC10);

}

void can_setmode(char mode)
{

}

can_message_t* can_message_read(void)
{

}

void can_message_send(can_message_t* message)
{
    mcp2515_write(MCP_TXB0SIDH, message->id >> 3);
    mcp2515_write(MCP_TXB0SIDL, message->id << 5);
    mpc2515_write(MCP_TXB0DLC, 0x0F & message->data_length);

    for(uint8_t i = 0; i < message->data_length; i++)
    {
        mcp2515_write(MCP_TXB0D0 + i,message->data[i]);
    }

    mcp2515_rts(MCP_RTS_TX0);
}

