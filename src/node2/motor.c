#include "motor.h"
#include "twi_master.h"
#include "encoder.h"

#include <avr/io.h>
#include <stdlib.h>

static int16_t max_encoder_value;

/* motor_dir_t
    * Type used to increase module readability.
    * Defines left and rightwise racket movement.
*/
typedef enum
{
    RIGHT,
    LEFT,
} motor_dir_t;

/* motor_init()
    * Initializes TWI to communicate with the
    * motor box. Enable motor and set dir.
*/
void motor_init()
{
    TWI_Master_Initialise();
    DDRH |= (1 << PH1) | (1 << PH4);
    PORTH |= (1 << PH4);
}

/* motor_send()
    * Generates TWI message to motor box
    * containing slave address, write bit and
    * the speed to be written to the motor.
*/
void motor_send(char speed)
{
    uint8_t twi_msg[3] = {0x50, 0x00, 0xFF & speed};
    TWI_Start_Transceiver_With_Data(twi_msg, 3);
}

/* motor_drive()
    * Set motorbox dir based on sign of speed.
    * Then initiate motor movement. 
*/
void motor_drive(int16_t speed)
{
    motor_dir_t dir;

    if (speed > 0)
        dir = RIGHT;
    else
        dir = LEFT;

    if (dir == LEFT)
        PORTH |= (1 << PH1);

    else
        PORTH &= ~(1 << PH1);

    motor_send(abs(speed));
}

/* motor_stop()
    * Stop racket by writing speed=0
    * to the motor box.
*/
void motor_stop()
{
    motor_send(0);
}

/* motor_calibrate()
    * Map the racket range of movement.
    * First, drive to the right until obstructed.
    * Then, drive left until obstructed. Define
    * x = 0 in the middle. Positions to the left
    * will be negative and positions to the right
    * positive. This to make the controller more
    * robust.
*/

uint16_t motor_calibrate()
{
    uint16_t counter = 10000;
    int16_t pos = encoder_read();

    motor_drive(100);

    while (1)
    {
        while (counter)
            counter--;

        if (pos == encoder_read())
            break;

        counter = 10000;
        pos = encoder_read();
    }

    motor_stop();
    encoder_reset();
    motor_drive(-100);

    counter = 10000;

    while (1)
    {
        while (counter)
            counter--;

        if (pos == encoder_read())
            break;

        counter = 10000;
        pos = encoder_read();
    }

    max_encoder_value = pos;

    motor_drive(100);
    counter = 10000;

    while (encoder_read() > max_encoder_value / 2)
    {
        while (counter)
            counter--;

        counter = 10000;
    }

    motor_stop();
    encoder_reset();

    return max_encoder_value / 2;
}