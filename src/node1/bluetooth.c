#include "bluetooth.h"

ISR(USART1_RXC_vect)
{
    // Called when data received from USART

    // Read UDR register to reset flag
    unsigned char data = UDR1;

    // Check for error
    if ((UCSR1A & ((1 << FE1) | (1 << DOR1) | (1 << UPE1))) == 0)
    {
        printf("%d\n\r", data);
    }
}

void bluetooth_init(int ubrr )
{
    UBRR1H = (unsigned char)(ubrr >> 8);
    UBRR1L = (unsigned char)ubrr;
    UCSR1B = (1 << RXEN1) | (1 << TXEN1) |(1 << RXCIE1);
    UCSR1C |= (1 << UCSZ10) | (1 << UCSZ11) |(1<<URSEL1) | (1<<USBS1);
}

int bluetooth_available()
{
    return !(UCSR1A & (1 << RXC1));
}

int bluetooth_transmit(char data)
{
    /* Wait for empty transmit buffer */
    while (!(UCSR1A & (1 << UDRE1)))
        ;
    /* Put data into buffer, sends the data */
    UDR1 = data;

    return 0;
}

unsigned char bluetooth_receive()
{
    /* Wait for data to be received */
    while ((UCSR1A & (1 << RXC1)))
        ;
    return UDR1;
}