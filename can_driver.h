#ifndef __CAN_DRIVER_H__
#define __CAN_DRIVER_H__

#include <avr/io.h>
#include <avr/interrupt.h>

#include "spi.h"
#include "MCP2515.h"

volatile int can_interrupt_flag;

typedef struct can_message
{
    uint8_t id;
    uint8_t data_length;
    char data[8];
} can_message_t;

void can_init(void);
void can_message_write(can_message_t* message);
can_message_t* can_message_read(void);

#endif