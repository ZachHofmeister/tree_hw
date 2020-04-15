//A queue for tnode
#ifndef queue_h
#define queue_h

#include <stdlib.h>
#include <stdbool.h>

#include "tree.h"

typedef struct qnode qnode;
struct qnode {
    tnode * node;
    qnode * next;
};

typedef struct queue queue;
struct queue {
    qnode * front;
    qnode * back;
    size_t size;
};

qnode * qnode_create(tnode * node, qnode * next);
queue * queue_create(void);

void queue_push(queue * queue, tnode * node);
void queue_pop(queue * queue);
tnode * queue_front(queue * queue);

bool queue_empty(queue * queue);
size_t queue_size(queue * queue);

#endif /* queue_h */
