#pragma once

#include <stddef.h>
#include <stdint.h>

#define LLIST_FOREACH(list, node) for (llist_node_t *node = list->head; node != NULL; node = node->next)

typedef struct llist_node {
    struct llist_node *next;
    void *data;
} llist_node_t;

typedef struct {
    llist_node_t *head;
    size_t size;
} llist_t;

/**
 * @brief Creates a new linked list.
 *
 * This function creates a new linked list.
 *
 * @return llist_t* A pointer to the newly created linked list.
 */
llist_t *llist_create();

/**
 * @brief Destroys the specified linked list.
 *
 * This function frees the memory allocated for the specified linked list.
 *
 * @param list A pointer to the linked list to destroy.
 */
void llist_destroy(llist_t *list);

/**
 * @brief Appends the specified data to the linked list.
 *
 * This function appends the specified data to the linked list.
 *
 * @param list A pointer to the linked list to append the data to.
 * @param data The data to append to the linked list.
 */
void llist_append(llist_t *list, void *data);

/**
 * @brief Removes the specified data from the linked list.
 *
 * This function removes the specified data from the linked list.
 *
 * @param list A pointer to the linked list to remove the data from.
 * @param data The data to remove from the linked list.
 */
void llist_remove(llist_t *list, void *data);

/**
 * @brief Removes the data at the specified index from the linked list.
 *
 * This function removes the data at the specified index from the linked list.
 *
 * @param list A pointer to the linked list to remove the data from.
 * @param index The index of the data to remove from the linked list.
 */
void llist_remove_at(llist_t *list, size_t index);

/**
 * @brief Retrieves the data at the specified index from the linked list.
 *
 * This function retrieves the data at the specified index from the linked list.
 *
 * @param list A pointer to the linked list to retrieve the data from.
 * @param index The index of the data to retrieve from the linked list.
 *
 * @return void* The data at the specified index.
 */
void *llist_get(llist_t *list, size_t index);
