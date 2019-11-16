#include "bluetooth.h"

unsigned char BT_read;
uint8_t bluetooth_available_flag;
static queue_t rx_queue;



unsigned char bluetooth_get_data(void)
{
    printf("Check\n\r");
    return BT_read;
}

void bluetooth_init(int ubrr)
{
    UBRR1H = (unsigned char)(ubrr >> 8);
    UBRR1L = (unsigned char)ubrr;
    UCSR1B = (1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1);
    UCSR1C |= (1 << UCSZ10) | (1 << UCSZ11) | (1 << URSEL1) | (1 << USBS1);

    bluetooth_available_flag = 0;
    q_init(&rx_queue, RX_QUEUE_SIZE);
}

int bluetooth_available()
{
    return (rx_queue.size && bluetooth_available_flag);
}

int bluetooth_TX_char(char data)
{
    /* Wait for empty transmit buffer */
    while (!(UCSR1A & (1 << UDRE1)))
        ;
    /* Put data into buffer, sends the data */
    UDR1 = data;

    return 0;
}

char bluetooth_receive()
{
    /* Wait for data to be received */
    while (!(UCSR1A & (1 << RXC1)))
        ;
    /* Get and return received data from buffer */
    return UDR1;
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
    char tp = q_dequeue(&rx_queue);
    int i = 0;
    while (tp != '\r')
    {
        tp = q_dequeue(&rx_queue);
        cli();
        if (q_is_empty(&rx_queue)) 
        {
            bluetooth_available_flag = 0;
            return -1;
        }
        sei();
    }
    tp = q_dequeue(&rx_queue);

    while (tp != '\n')
    {
        c[i++] = tp;
        tp = q_dequeue(&rx_queue);
    }
    c[i] = '\0';
    cli();
    if (q_is_empty(&rx_queue)) 
        bluetooth_available_flag = 0;
    sei();
    return c;
}

ISR(USART1_RXC_vect)
{
    char c = bluetooth_receive();
    q_enqueue(&rx_queue, c);
    if (c == '\n' && !q_is_empty(&rx_queue))
        bluetooth_available_flag = 1;
    //if ((UCSR1A & ((1 << FE1) | (1 << DOR1) | (1 << UPE1))) == 0)
}

void q_init(queue_t *q, unsigned int size)
{
    q->size = size;
    q->end = -1;
    q->front = -1;
    q->length = 0;
    q->buffer = (char *)malloc(size);
}

int q_is_full(queue_t *q)
{
    if ((q->front == q->end + 1) || (q->front == 0 && q->end == q->size - 1))
        return 1;
    return 0;
}

int q_is_empty(queue_t *q)
{
    if (q->front == -1)
        return 1;
    return 0;
}

void q_enqueue(queue_t *q, char e)
{
    if (!q_is_full(q))
    {
        if (q->front == -1)
            q->front = 0;
        q->end = (q->end + 1) % q->size;
        q->buffer[q->end] = e;
    }
}

char q_dequeue(queue_t *q)
{
    char e;
    if (q_is_empty(q))
        return -1;
    e = q->buffer[q->front];
    if (q->front == q->end)
    {
        q->front = -1;
        q->end = -1;
    }
    else
    {
        q->front = (q->front + 1) % q->size;
    }
    return e;
}