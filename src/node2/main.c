#include <stdint.h>

#include "../../lib/can/can_driver.h"
#include "../../lib/uart/uart.h"

#include "servo.h"
#include "game.h"
#include "motor.h"
#include "encoder.h"

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

    can_message_t response;
    int counter = 1000;
    int16_t encoder_number;

    printf("%s\r\n", "Welcome stranger");
    
    sei();

    motor_calibrate();
    motor_set_ref(1000);
    while(1)
    {

            //printf("%d\r\n", TCNT0);
        if (!(can_message_read(&response))) // Returns 0 when successfully read
        {
            //printf("id. %d", response.id);
            //printf(" DATA: %d\n\r", response.data[0]);
            if (response.id == 1)
            {
                servo_set_pos(response.data[0]);
            }
        }
        encoder_number = encoder_read();
        //printf("%8d\r", encoder_number);

        while (counter) { counter--; }
        counter = 1000;
        pid();
    }
    return 0;
}

