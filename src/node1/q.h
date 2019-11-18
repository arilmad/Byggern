#ifndef __Q_H__
#define __Q_H__

/* Defines type structure for the queue */
typedef struct queue
{
    unsigned int size;
    int front;
    int end;
    unsigned int length;
    char *buffer;
} queue_t;

/* Initialize the queue */
void q_init(queue_t *q, unsigned int size);

/* Checks if the queue is full */
int q_is_full(queue_t *q);

/* Checks if the queue is empty */
int q_is_empty(queue_t *q);

/* Enques an element into the queue */
void q_enqueue(queue_t *q, char e);

/* Deques an element from the queue */
char q_dequeue(queue_t *q);

#endif