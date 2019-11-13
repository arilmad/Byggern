#ifndef __CAN_DRIVER_H__
#define __CAN_DRIVER_H__

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/common.h>

#include "../spi/spi.h"
#include "MCP2515.h"

volatile int can_interrupt_flag;

typedef struct can_message
{
    int8_t id;
    uint8_t data_length;
    char data[8];
} can_message_t;

void can_init(char mode);
void can_message_send(can_message_t* message);
uint8_t can_message_read(can_message_t* message);

#endif