#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define RX_QUEUE_SIZE 32

typedef struct queue
{
    unsigned int size;
    int front;
    int end;
    unsigned int length;
    char *buffer;
} queue_t;

void q_init(queue_t *q, unsigned int size);
int q_is_full(queue_t *q);
int q_is_empty(queue_t *q);
void q_enqueue(queue_t *q, char e);
char q_dequeue(queue_t *q);

void bluetooth_init(int ubrr);

int bluetooth_available();

int bluetooth_TX_char(char data);

void bluetooth_send(char *str);

const char *bluetooth_read();

unsigned char bluetooth_get_data(void);
