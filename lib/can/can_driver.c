#include "can_driver.h"
#include "../spi/spi.h"
#include "MCP2515.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/common.h>

/* can_init()
    * Initialize mcp2525 with mode
    * specified by main.
*/
void can_init(char mode)
{
    mcp2515_init(mode);
}

/* can_setmode()
    * Modify CAN mode.
*/
void can_setmode(char mode)
{
    mcp2515_modify(MCP_CANCTRL, MODE_MASK, mode);
}

/* can_message_read()
    * Return 0 if no message is received. Derive
    * message ID and data from message. Read only
    * from RXB0SIDH since few unique message IDs 
    * are used.
*/
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

/* can_message_send()
    * Send a pre-constructed can type message. High
    * resolution IDs are not necessary, only TXB0SIDH
    * is therefore used.
*/
void can_message_send(can_message_t* message)
{
    mcp2515_write(MCP_TXB0SIDH, message->id);
    mcp2515_write(MCP_TXB0SIDL, 0x00);

    mcp2515_write(MCP_TXB0DLC, 0x0F & message->data_length);

    for (uint8_t i = 0; i < message->data_length; i++)
        mcp2515_write(MCP_TXB0D0 + i, message->data[i]);

    mcp2515_rts(MCP_RTS_TX0);
}
