#include "collections/queue.h"

#include <stdlib.h>
#include <string.h>

#include "core/assert.h"

queue_t *queue_create(size_t capacity) {
    queue_t *queue  = malloc(sizeof(queue_t));
    queue->list     = llist_create();

    return queue;
}

void queue_destroy(queue_t *queue) {
    llist_destroy(queue->list);
    free(queue);
}

void *queue_front(queue_t *queue) {
    ASSERT(queue != NULL);
    ASSERT(queue->list != NULL);

    if (queue->list->size == 0) {
        return NULL;
    }

    return queue->list->head->data;
}

void *queue_back(queue_t *queue) {
    ASSERT(queue != NULL);
    ASSERT(queue->size != NULL);

    if (queue->list->size == 0) {
        return NULL;
    }

    LLIST_FOREACH(queue->list, node) {
        if (node->next == NULL) {
            return node->data;
        }
    }
}

void queue_push(queue_t *queue, void *value) {
    ASSERT(queue != NULL);
    ASSERT(queue->list != NULL);

    llist_append(queue->list, value);
}

void *queue_pop(queue_t *queue) {
    ASSERT(queue != NULL);
    ASSERT(queue->data != NULL);

    if (queue->list->size == 0) {
        return NULL;
    }

    return llist_pop(queue->list);
}

size_t queue_size(queue_t *queue) {
    ASSERT(queue != NULL);
    ASSERT(queue->list != NULL);

    return queue->list->size;
}
