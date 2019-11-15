#include <avr/io.h>
#include <avr/interrupt.h>


void bluetooth_init(int ubrr);

int bluetooth_available();

int bluetooth_transmit(char data);

unsigned char bluetooth_receive();