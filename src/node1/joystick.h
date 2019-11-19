/* joystick.h
    * This part of the program is used
    * to communicate with the joystick
    * found on the I/O card.
*/

#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include <stdint.h>

/* Type defining the position of the joystick */
typedef struct
{
    int16_t x;
    int16_t y;
} joystick_pos_t;

/* Type defining the direction of the joystick */
typedef enum
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NEUTRAL
} joystick_dir_t;

void joystick_init(void);

uint8_t joystick_get_button_pressed_flag(void);
void joystick_reset_button_pressed_flag(void);
joystick_pos_t joystick_get_relative_pos(void);
joystick_dir_t joystick_get_x_dir(void);
joystick_dir_t joystick_get_y_dir(void);

#endif