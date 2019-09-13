#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

typedef struct{
    int16_t x;
    int16_t y;
} joystick_pos_t;

void joystick_init( void );
void joystick_set_center( void );
joystick_pos_t read_joystick_pos( void );

#endif