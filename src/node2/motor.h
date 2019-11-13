#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <avr/io.h>

#include "encoder.h"
#include "twi_master.h"
#include "../../lib/uart/uart.h"

typedef enum
{
    RIGHT,
    LEFT,
} motor_dir_t;

void motor_init( void );
void motor_drive( int16_t speed );
void motor_stop( void );
uint16_t motor_calibrate( void );

#endif