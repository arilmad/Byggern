#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <avr/io.h>
#include "twi_master.h"

typedef enum
{
    RIGHT,
    LEFT,
} motor_dir_t;

motor_get_max_value( void );
void motor_init( void );
void motor_drive( int16_t speed );
void motor_stop( void );
void motor_calibrate( void );
void pid( void );

#endif