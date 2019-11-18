#ifndef __Q_H__
#define __Q_H__

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

#endif