#pragma once

#include <stddef.h>

#include "collections/llist.h"
typedef struct {
    llist_t *list;
} queue_t;

/**
 * @brief Creates a new queue.
 *
 * This function creates a new queue with the specified capacity.
 *
 * @return queue_t* A pointer to the newly created queue.
 */
queue_t *queue_create();

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

/**
 * @brief Get the size of the queue.
 * 
 * This function returns the number of elements in the queue.
 * 
 * @param queue A pointer to the queue.
 * @return size_t The number of elements in the queue.
 */
size_t queue_size(queue_t *queue);
