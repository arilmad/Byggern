#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

typedef struct
{
    int16_t x;
    int16_t y;
} joystick_pos_t;

typedef enum
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NEUTRAL
} joystick_dir_t;

uint8_t joystick_get_button_pressed_flag(void);
void joystick_reset_button_pressed_flag(void);

void joystick_init(void);

joystick_pos_t joystick_get_relative_pos(void);

joystick_dir_t joystick_get_x_dir(void);
joystick_dir_t joystick_get_y_dir(void);

#endif