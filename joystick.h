#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

#define max(a, b) \
    ({ __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b); \
        _a > _b ? _a : _b; })

#define min(a, b) \
    ({ __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b); \
        _a < _b ? _a : _b; })

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

void joystick_init(void);
joystick_pos_t get_relative_joystick_pos(void);
joystick_dir_t get_x_dir(void);
joystick_dir_t get_y_dir(void);

#endif