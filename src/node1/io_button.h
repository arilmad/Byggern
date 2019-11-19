/* io_button.h
    * This part of the program reads
    * the button(s) used by the I/O
    * card.
*/

#ifndef __IO_BUTTON__
#define __IO_BUTTON__

#include <stdint.h>

void io_button_init(void);
uint8_t io_button_get_left_button_press(void);

#endif