#include "motor.h"
#include "encoder.h"
#include "../../lib/uart/uart.h"

#define umax 256
#define umin -256

static uint16_t max_encoder_value;

static int r = 2000;

/*
static double e2, e1, e0, u2, u1, u0;
double y;

double Kp = 5; 
double Ki = 0;
double Kd = 0;

int N = 20;
double Ts = 0.01;

#define a0 (1 + N*Ts)
#define a1 -(2 + N*Ts)
#define a2 1

#define b0 Kp*(1+N*Ts) + Ki*Ts*(1+N*Ts) + Kd*N
#define b1 -(Kp*(2+N*Ts) + Ki*Ts + 2*Kd*N)
#define b2 Kp + Kd*N

#define ku1 a1/a0
#define ku2 a2/a0
#define ke0 b0/a0
#define ke1 b1/a0
#define ke2 b2/a0

void pid( void )
{
    e2 = e1;
    e1 = e0;
    u2 = u1;
    u1 = u0;

    y = encoder_read();

    e0 = r - y;
    u0 = -ku1*u1 - ku2*u2 + ke0*e0 + ke1*e1 + ke2*e2;

    //if ( u0 > umax ) { u0 = 256.0; }
    //else if ( u0 < umin ) { u0 = -256.0; }

    motor_drive(u0);
}
*/

static float e0 = 0;
static float integral = 0;
#define Kp 1
#define Ki 0
#define Kd 0
#define Ts 0.01

void pid()
{
    float e = (float)(r - encoder_read());
    integral += e*Ts;
    float d = (e - e0)/Ts;
    float u = Kp*e + Ki*integral + Kd*d;
    if ( u < -256 )
    {
        motor_drive(-256);
    } 
    else if ( u > 256 )
    {
        motor_drive(256);
    }

    else
    {
        motor_drive(u);
    }
    e0 = e;
}

void motor_set_ref(int ref)
{
    r = ref;
}

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

void motor_calibrate()
{   
    int16_t pos = encoder_read(); 
    motor_drive(100);
    uint16_t counter = 1000;

    while ( 1 )
    {
        if ( !counter )
        {
            if ( pos == encoder_read())
            {
                break;
            } else 
            {
                pos = encoder_read();
                counter = 1000;
            }
        }
        counter --;
        encoder_read();
    }

    motor_stop();   
    encoder_reset();
    
    motor_drive(-100);

    counter = 1000;

    while ( 1 )
    {
        if ( !counter )
        {
            if ( pos == encoder_read())
            {
                break;
            } else 
            {
                pos = encoder_read();
                counter = 1000;
            }
        }
        counter --;
        encoder_read();
    }

    max_encoder_value = encoder_read();
    motor_drive(100);
    while (encoder_read() > max_encoder_value/2);
    motor_stop();
    pos = encoder_read();
}