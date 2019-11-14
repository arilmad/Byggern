#include "motor.h"


static int16_t max_encoder_value;


void motor_init( void )
{
    TWI_Master_Initialise();
    DDRH |= (1 << PH1) | (1 << PH4);
    PORTH |= (1 << PH4);
}

void motor_send( char speed )
{
    uint8_t twi_msg[3] = {0x50, 0x00, 0xFF & speed};
    TWI_Start_Transceiver_With_Data( twi_msg, 3 );
}

void motor_drive( int16_t speed )
{
    motor_dir_t dir;

    if ( speed > 0 ) dir = RIGHT;
    else dir = LEFT;

    if (dir == LEFT) {
        PORTH |= (1 << PH1);
    }
    else {
        PORTH &= ~(1 << PH1);
    }
    motor_send(abs(speed));
}

void motor_stop( void )
{
    motor_send(0);
}

uint16_t motor_calibrate()
{   
    uint16_t counter = 10000;
    int16_t pos = encoder_read(); 
    motor_drive(100);

    while ( 1 ) 
    { 
        while ( counter ) { counter --; }
        if ( pos == encoder_read() ) { break; }
        counter = 10000;
        pos = encoder_read();
    }
    
    motor_stop();   
    encoder_reset();
    motor_drive(-100);

    counter = 10000;

    while ( 1 )
    {
        while ( counter ) { counter --; }
        if ( pos == encoder_read() ) { break; }
        counter = 10000;
        pos = encoder_read();
    }


    max_encoder_value = pos;

    motor_drive(100);
    counter = 10000;

    while (encoder_read() > max_encoder_value/2)
    {
        while ( counter ) { counter --; }
        counter = 10000;
    }

    motor_stop();
    encoder_reset();
    pos = encoder_read();

    printf("%d\n\r", pos);

    return max_encoder_value/2;
}