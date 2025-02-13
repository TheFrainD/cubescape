#include "collections/queue.h"

#include <stdlib.h>
#include <string.h>

#include "core/assert.h"

queue_t *queue_create(size_t capacity) {
    queue_t *queue  = malloc(sizeof(queue_t));
    queue->data     = malloc(sizeof(void *) * capacity);
    queue->size     = 0;
    queue->capacity = capacity;
    queue->front    = 0;

    return queue;
}

void queue_destroy(queue_t *queue) {
    free(queue->data);
    free(queue);
}

void *queue_front(queue_t *queue) {
    ASSERT(queue != NULL);
    ASSERT(queue->data != NULL);

    if (queue->size == 0) {
        return NULL;
    }

    return queue->data[queue->front];
}

void *queue_back(queue_t *queue) {
    ASSERT(queue != NULL);
    ASSERT(queue->data != NULL);

    if (queue->size == 0) {
        return NULL;
    }

    return queue->data[queue->size - 1];
}

void queue_push(queue_t *queue, void *value) {
    ASSERT(queue != NULL);
    ASSERT(queue->data != NULL);

    if (queue->size == queue->capacity) {
        return;
    }

    queue->data[queue->front + queue->size] = value;
    ++queue->size;
}

void *queue_pop(queue_t *queue) {
    ASSERT(queue != NULL);
    ASSERT(queue->data != NULL);

    if (queue->size == 0) {
        return NULL;
    }

    void *value = queue->data[queue->front];
    for (size_t i = 1; i < queue->size; ++i) {
        queue->data[i - 1] = queue->data[i];
    }
    --queue->size;

    return value;
}
