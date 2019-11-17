#ifndef __IO_BUTTON__
#define __IO_BUTTON__

#include <stdlib.h>
#include <avr/io.h>

void io_button_init(void);
uint8_t io_button_get_left_button_press(void);

#endif