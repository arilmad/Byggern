#include <stdint.h>
#include <avr/interrupt.h>

#include "../../lib/can/can_driver.h"
#include "../../lib/uart/uart.h"

#include "servo.h"
#include "game.h"
#include "motor.h"
#include "pid.h"
#include "encoder.h"
#include "solenoid.h"

#define F_CPU 16000000

#define BAUD 9600
#define MYUBRR F_CPU / 16 / BAUD - 1

volatile uint8_t pid_flag = 0;

ISR(TIMER1_OVF_vect)
{
    pid_flag = 1;
}

void interrupt_init()
{
    TCCR1B |= (1<<CS10);
    TIMSK1 |= (1<<TOIE1);
    TCNT1 = 0;
}

int main()
{
    cli();

    interrupt_init();
    UART_init( MYUBRR );
    can_init( MODE_NORMAL );
    servo_init();
    game_init();
    motor_init();
    encoder_init();
    solenoid_init();

    can_message_t response;
    int16_t encoder_value;

    sei();
    uint16_t max_encoder_value = motor_calibrate();

    pid_init(35, 1 ,5);
    int16_t ref = max_encoder_value / 2;


    while(1)
    {
        if (!(can_message_read(&response))) // Returns 0 when successfully read
        {
            if (response.id == 1)
            {
                servo_set_pos(response.data[0]);
            }
            else if (response.id == 2)
            {
                int16_t temp = response.data[0];
                temp = abs( temp - 100 );
                ref = (int16_t)((double)(temp/100.0) * max_encoder_value);
            }
            else if (response.id == 3){
                solenoid_trigger();
            }
        }

        if(pid_flag)
        {
            encoder_value = encoder_read();
            int16_t u = pid_update(ref, encoder_value);
            motor_drive(u);
            pid_flag = 0;
        }
    }
    return 0;
}

