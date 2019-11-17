#include "q.h"

void q_init(queue_t *q, unsigned int size)
{
    q->size = size;
    q->end = -1;
    q->front = -1;
    q->length = 0;
    q->buffer = (char *)malloc(size + 1);
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
        return 0;
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