#ifndef QUEUE
#define QUEUE

#include "freeE.h"

typedef struct Queue
{
    EncodeTree_t *val;
    struct Queue *next;
} Queue_t;

Queue_t *makeQueue(Node_t *Nodes, int countDiffSymbol);
EncodeTree_t *pop(Queue_t **head);
Queue_t *unite(EncodeTree_t *arg1, EncodeTree_t *arg2);
Queue_t *insert(Queue_t *head, Queue_t *unitedNodes);

#endif
