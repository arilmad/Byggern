#include "io_button.h"

#include <stdlib.h>
#include <avr/io.h>

void io_button_init()
{
    DDRB &= (~(1 << PB1));
}

uint8_t io_button_get_left_button_press()
{
    return (PINB &(1 << PB1));
}
