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
volatile uint8_t score_flag = 0;
volatile uint8_t solenoid_flag = 0;


ISR(TIMER1_OVF_vect)
{
    pid_flag = 1;
    
    static int i = 0;
    if (i < 20) { i++; }
    else 
    { 
        solenoid_flag = 1;
        i = 0;
    }

}

ISR(TIMER2_OVF_vect)
{
    static int i = 0;
    if (i < 350)
    {
        i++;
    } else
    {
        score_flag = 1;
        i = 0;
    }
    
}

void timer1_init()
{
    TCCR1B |= (1<<CS10);
    TIMSK1 |= (1<<TOIE1);
    TCNT1 = 0;
}

void timer2_init()
{
    TCCR2B |= (1 << CS20) | (1 << CS21);
    TIMSK2 |= (1 << TOIE2);
    TCNT2 = 0;
}

int main()
{
    cli();

    timer1_init();
    timer2_init();

    UART_init( MYUBRR );
    can_init( MODE_NORMAL );
    servo_init();
    game_init();
    motor_init();
    encoder_init();
    solenoid_init();

    can_message_t message_send = {0, 8, 0};
    can_message_t response;
    int16_t encoder_value;

    sei();
    uint16_t max_encoder_value = motor_calibrate();
// 35, 1, 5
    pid_init(35, 1, 6);
    int16_t ref = 0;
    int8_t active_game = 0;
    while(1)
    {
        if ((can_message_read(&response))) // Returns 0 when successfully read
        {
            if (response.id == 4) { 
                active_game = 1;
            }
            //printf("%d\r\n", response.id);
            if (active_game){  
                if (response.id == 1)
                {
                    servo_set_pos(response.data[0]);
                }
                else if (response.id == 2)
                {
                    int16_t temp = response.data[0];
                    temp -= 50;
                    ref = -(int16_t)(double)((temp/50.0) * max_encoder_value);    
                }
                else if (response.id == 3){
                    if (solenoid_flag)
                    {
                        solenoid_trigger();
                        solenoid_flag = 0;
                    }
                }

            }
        }

        encoder_value = encoder_read();


        //printf("%d\r\n", encoder_value);

        if(pid_flag)
        {
            int16_t u = pid_update(ref, encoder_value);
            motor_drive(u);
            pid_flag = 0;
        }

        if (score_flag)
        {
            message_send.id = 0;
            can_message_send(&message_send);
            score_flag = 0;
        }
        /*
        if (game_over_flag)
        {
            message_send.id = 1;
            can_message_send(&message_send);
            active_game = 0;
        }*/

        _delay_ms(10);

    }
    return 0;
}

