#pragma once

#include <stddef.h>

typedef struct {
    void **data;
    size_t size;
    size_t capacity;
    size_t front;
} queue_t;

/**
 * @brief Creates a new queue.
 *
 * This function creates a new queue with the specified capacity.
 *
 * @param capacity The capacity of the queue.
 * @return queue_t* A pointer to the newly created queue.
 */
queue_t *queue_create(size_t capacity);

/**
 * @brief Destroys the specified queue.
 *
 * This function frees the memory allocated for the specified queue.
 *
 * @param queue A pointer to the queue to destroy.
 */
void queue_destroy(queue_t *queue);

/**
 * @brief Access next element in queue.
 *
 * This function returns the next element in the queue without removing it.
 *
 * @param queue A pointer to the queue.
 * @return void* A pointer to the next element in the queue.
 */
void *queue_front(queue_t *queue);

/**
 * @brief Access last element in queue.
 *
 * This function returns the last element in the queue without removing it.
 *
 * @param queue A pointer to the queue.
 * @return void* A pointer to the last element in the queue.
 */
void *queue_back(queue_t *queue);

/**
 * @brief Insert element into queue.
 *
 * This function inserts an element into the queue.
 *
 * @param queue A pointer to the queue.
 * @param value A pointer to the element to insert.
 */
void queue_push(queue_t *queue, void *value);

/**
 * @brief Remove element from queue.
 *
 * This function removes an element from the queue.
 *
 * @param queue A pointer to the queue.
 * @return void* A pointer to the removed element.
 */
void *queue_pop(queue_t *queue);
