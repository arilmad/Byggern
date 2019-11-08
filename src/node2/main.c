#include <stdint.h>

#include "../../lib/can/can_driver.h"
#include "../../lib/uart/uart.h"

#include "servo.h"
#include "game.h"
#include "motor.h"
#include "encoder.h"
#include "solenoid.h"

#define F_CPU 16000000

#define BAUD 9600
#define MYUBRR F_CPU / 16 / BAUD - 1


int main()
{
    cli();
    UART_init( MYUBRR );
    can_init(MODE_NORMAL);
    servo_init();
    game_init();
    //TWI_Master_Initialise();
    motor_init();
    encoder_init();
    solenoid_init();

    can_message_t response;
    int counter = 10000;
    int16_t encoder_number;
    int32_t tuner_counter = 250;
    printf("%s\r\n", "Welcome stranger");
    
    sei();

    motor_calibrate();
    static uint8_t flip = 0;

    while(1)
    {
        /*
        if (!( tuner_counter))
        {
            if (!flip)
            {
                motor_set_ref(3000);
            } else
            {
                motor_set_ref(5000);
            }
            flip = !flip;
            tuner_counter = 250;   
        }

        tuner_counter --;

        */

            //printf("%d\r\n", TCNT0);
        if (!(can_message_read(&response))) // Returns 0 when successfully read
        {
            printf("id. %d", response.id);
            //printf(" DATA: %d\n\r", response.data[0]);
            if (response.id == 1)
            {
                servo_set_pos(response.data[0]);
            }
            else if (response.id == 2)
            {
                motor_set_ref(response.data[0]);
            }
            else if (response.id == 3){
                solenoid_trigger();
            }
        }
        encoder_number = encoder_read();
        //printf("%8d\r", encoder_number);

        while (counter) { counter--; }
        counter = 10000;
        pid();
    }
    return 0;
}

