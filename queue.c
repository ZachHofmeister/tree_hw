#include <stdlib.h>

#include "queue.h"
#include "tree.h"

qnode * qnode_create(tnode * node, qnode * next) {
    qnode * p = (qnode *)malloc(sizeof(qnode));
    p->node = node;
    p->next = next;
    return p;
}

queue * queue_create(void) {
    queue * p = (queue *)malloc(sizeof(queue));
    p->front = NULL;
    p->back = NULL;
    p->size = 0;
    return p;
}

void queue_push(queue * queue, tnode * node) {
    qnode * p = qnode_create(node, NULL);
    if (queue->size != 0) queue->back->next = p;
    else queue->front = p;
    queue->back = p;
    ++queue->size;
}

void queue_pop(queue * queue) {
    if (queue->size == 0) return;
    qnode * p = queue->front;
    queue->front = queue->front->next;
    if (queue->front == NULL) queue->back = NULL;
    --queue->size;
    free(p);
}

tnode * queue_front(queue * queue) {
    return queue->front->node;
}

bool queue_empty(queue * queue) {
    return queue->size == 0;
}

size_t queue_size(queue * queue) {
    return queue->size;
}