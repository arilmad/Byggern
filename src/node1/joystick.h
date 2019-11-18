/* Driver for the joystick on the io card */
#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

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

/* Init function for the joystick */
void joystick_init(void);


/* Check if the button flag is raised by the ISR */
uint8_t joystick_get_button_pressed_flag(void);

/* Resets the button flag */
void joystick_reset_button_pressed_flag(void);

/* Getter function for the relative position of the joystick */
joystick_pos_t joystick_get_relative_pos(void);

/* Getter functions for the x and y directions */
joystick_dir_t joystick_get_x_dir(void);
joystick_dir_t joystick_get_y_dir(void);

#endif