#ifndef __IO_BUTTON__
#define __IO_BUTTON__

/* Init function for the button */
void io_button_init(void);

/* Get the state of the button */
uint8_t io_button_get_left_button_press(void);

#endif