#define BT_RX_QUEUE_SIZE 64

#include "bluetooth.h"
#include "q.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

volatile uint8_t bluetooth_msg_available_flag;
static queue_t bt_rx_queue;

/* ISR(USART_RXC_vect)
    * The USART1 receive flag will trigger
    * the interrupt. Registers are read
    * and put into a queue.
*/
ISR(USART1_RXC_vect)
{
    char c = UDR1;

    q_enqueue(&bt_rx_queue, c);

    if (c == '\n' && !q_is_empty(&bt_rx_queue))
        bluetooth_msg_available_flag = 1;
}

/* bluetooth_init(int ubrr)
    * Initialize the bluetooth with the 
    * given baud rate, and setup the
    * queue for messages.
*/
void bluetooth_init(int ubrr)
{
    UBRR1H = (unsigned char)(ubrr >> 8);
    UBRR1L = (unsigned char)ubrr;
    UCSR1B = (1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1);
    UCSR1C |= (1 << UCSZ10) | (1 << UCSZ11) | (1 << URSEL1) | (1 << USBS1);

    bluetooth_msg_available_flag = 0;

    q_init(&bt_rx_queue, BT_RX_QUEUE_SIZE);
}

/*bluetooth_msg_available()
    * Checks if the bluetooth available flag is set.
*/
int bluetooth_msg_available()
{
    return bluetooth_msg_available_flag;
}

/*bluetooth_TX_char(char data)
    * Sends a char to the output register of the USART
*/
int bluetooth_TX_char(char data)
{
    while (!(UCSR1A & (1 << UDRE1)))
        ;
    UDR1 = data;

    return 0;
}

/* bluetooth_send(char *str)
    * Sends the string on bluetooth
*/
void bluetooth_send(char *str)
{
    int a = 0;
    while (str[a] != '\0')
    {
        bluetooth_TX_char(str[a++]);
    }
}

/* *bluetooth_read()
    * Reads a bluetooth message from the queue
    * Checks for start ('\r') and end ('\n') char.
*/
const char *bluetooth_read()
{
    static char c[9];
    c[0] = '\0';
    char tp = q_dequeue(&bt_rx_queue);

    uint8_t i = 0;
    while (tp != '\r')
    {
        tp = q_dequeue(&bt_rx_queue);

        if (q_is_empty(&bt_rx_queue))
        {
            bluetooth_msg_available_flag = 0;
            return '\0';
        }
    }

    tp = q_dequeue(&bt_rx_queue);

    while (tp != '\n')
    {
        c[i++] = tp;
        tp = q_dequeue(&bt_rx_queue);

        if (i > 7)
        {
            bluetooth_msg_available_flag = 0;
            return '\0';
        }
    }
    c[i] = '\0';

    if (q_is_empty(&bt_rx_queue))
        bluetooth_msg_available_flag = 0;
    return c;
}
