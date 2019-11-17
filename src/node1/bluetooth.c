#include "bluetooth.h"

#define BT_RX_QUEUE_SIZE 64

volatile uint8_t bluetooth_available_flag;
volatile static queue_t bt_rx_queue;

ISR(USART1_RXC_vect)
{
    char c = UDR1;

    q_enqueue(&bt_rx_queue, c);

    if (c == '\n' && !q_is_empty(&bt_rx_queue))
        bluetooth_available_flag = 1;
}

void bluetooth_init(int ubrr)
{
    UBRR1H = (unsigned char)(ubrr >> 8);
    UBRR1L = (unsigned char)ubrr;
    UCSR1B = (1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1);
    UCSR1C |= (1 << UCSZ10) | (1 << UCSZ11) | (1 << URSEL1) | (1 << USBS1);

    bluetooth_available_flag = 0;

    q_init(&bt_rx_queue, BT_RX_QUEUE_SIZE);
}

int bluetooth_available()
{
    return (bluetooth_available_flag);
}

int bluetooth_TX_char(char data)
{
    while (!(UCSR1A & (1 << UDRE1)))
        ;
    UDR1 = data;

    return 0;
}

void bluetooth_send(char *str)
{
    int a = 0;
    while (str[a] != '\0')
    {
        bluetooth_TX_char(str[a++]);
    }
}

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
            bluetooth_available_flag = 0;
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
            bluetooth_available_flag = 0;
            return '\0';
        }
    }
    c[i] = '\0';

    if (q_is_empty(&bt_rx_queue))
        bluetooth_available_flag = 0;
    return c;
}
