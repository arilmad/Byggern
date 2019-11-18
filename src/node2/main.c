#define F_CPU 16000000

#define BAUD 9600
#define UBRR F_CPU / 16 / BAUD - 1

#include "../../lib/can/can_driver.h"
#include "../../lib/can/MCP2515.h"
#include "../../lib/uart/uart.h"

#include "servo.h"
#include "ir.h"
#include "motor.h"
#include "pid.h"
#include "encoder.h"
#include "solenoid.h"

#include <stdint.h>
#include <avr/interrupt.h>

/* Various binary flags used by main loop.
    * pid_flag signalizes control signal update. Set periodically.
    * score_flag sets periodically and indicates that node1 should
    * increment its score. CAN msg sent only whilst game is active.
    * solenoid_flag sets periodically and ensures some time slack
    * in between solenoid_trigger() calls.
*/
volatile uint8_t pid_flag = 0;
volatile uint8_t score_flag = 0;
volatile uint8_t solenoid_flag = 0;


/* ISR(TIMER1_OVF_vect)
    * Upon each TIMER1 overflow, signalize pid control update.
    * Upon every 20th TIMER1 overflow, signalize that
    * solenoid is ready for use if need be.
 */
ISR(TIMER1_OVF_vect)
{
    pid_flag = 1;

    static int i = 0;
    if (i < 20)
        i++;

    else
    {
        solenoid_flag = 1;
        i = 0;
    }
}

/* ISR(TIMER2_OVF_vect)
    * Set score_flag periodically. Score held by node 1 
    * will increment every time score_flag is set.
 */
ISR(TIMER2_OVF_vect)
{
    static int i = 0;
    if (i < 350)
        i++;

    else
    {
        score_flag = 1;
        i = 0;
    }
}

/* timer1_init()
    * No prescaling
    * Enable input capture interrupt
    * Initialize counter at 0
 */
void timer1_init()
{
    TCCR1B |= (1 << CS10);
    TIMSK1 |= (1 << TOIE1);
    TCNT1 = 0;
}

/* timer2_init()
    * clk_io / 64 prescaling
    * Enable input capture interrupt
    * Initialize counter at 0
 */
void timer2_init()
{
    TCCR2B |= (1 << CS20) | (1 << CS21);
    TIMSK2 |= (1 << TOIE2);
    TCNT2 = 0;
}

/* main()
    * The node2 main loop is the heart of the game,
    * and sets up all sensors and motors for the 
    * game board. Before receiving a CAN msg from
    * node1 calling for a new game to start, it will
    * refuse to take any external commands. 
    *
    * Whilst in active_game mode it will receive motor
    * and position references from node1, and report
    * back periodically to node1 from the solenoid is
    * triggered for the first time (start_score=1) 
    * until the IR-sensor is triggered, meaning game 
    * is over. When the IR-sensor is triggered, a CAN 
    * msg will be sent to node1 and the game resets.   
 */
int main()
{
    cli();

    can_message_t message_send = {0, 8, 0};
    can_message_t response;

    int16_t encoder_value;
    uint16_t max_encoder_value;

    int16_t u;
    int16_t ref;
    int16_t tp;

    int8_t active_game = 0;
    uint8_t start_score = 0;

    timer1_init();
    timer2_init();

    UART_init(UBRR);
    can_init(MODE_NORMAL);

    servo_init();
    ir_init();
    motor_init();
    encoder_init();
    solenoid_init();
    pid_init(35, 1, 6);

    sei();

    while (1)
    {
        if (can_message_read(&response))
        {
            if (response.id == 4)
            {
                active_game = 1;
                servo_set_pos(122);
                ir_reset_ir_triggered_flag();
                max_encoder_value = motor_calibrate();
                ref = 0;
            }

            else if (active_game)
            {
                if (response.id == 1)
                    servo_set_pos((uint8_t)response.data[0]);

                else if (response.id == 2)
                {
                    tp = (uint8_t)response.data[0] - 122;
                    ref = -(int16_t)(double)((tp / 122.0) * max_encoder_value);
                }
                else if (response.id == 3)
                {
                    if (solenoid_flag)
                    {
                        solenoid_trigger();
                        solenoid_flag = 0;
                        start_score = 1;
                        ir_reset_ir_triggered_flag();
                    }
                }
            }
        }

        if (pid_flag)
        {
            encoder_value = encoder_read();
            u = pid_calculate_u(ref, encoder_value);
            motor_drive(u);
            pid_flag = 0;
        }

        if (score_flag && start_score)
        {
            score_flag = 0;
            message_send.id = 0;
            can_message_send(&message_send);
        }

        if (ir_get_ir_triggered_flag() && start_score)
        {
            active_game = 0;
            start_score = 0;

            message_send.id = 1;
            can_message_send(&message_send);
        }
    }
    return 0;
}
