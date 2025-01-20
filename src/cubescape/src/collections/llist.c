#include "collections/llist.h"

#include <stdlib.h>

llist_t *llist_create() {
    llist_t *list = malloc(sizeof(llist_t));
    if (list == NULL) {
        return NULL;
    }

    list->head = NULL;
    list->size = 0;

    return list;
}

void llist_destroy(llist_t *list) {
    if (list == NULL) {
        return;
    }

    llist_node_t *node = list->head;
    while (node != NULL) {
        llist_node_t *next = node->next;
        free(node);
        node = next;
    }

    free(list);
}

void llist_append(llist_t *list, void *data) {
    if (list == NULL) {
        return;
    }

    llist_node_t *node = malloc(sizeof(llist_node_t));
    if (node == NULL) {
        return;
    }

    node->data = data;
    node->next = NULL;

    if (list->head == NULL) {
        list->head = node;
    } else {
        llist_node_t *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }

        current->next = node;
    }

    list->size++;
}

void llist_remove(llist_t *list, void *data) {
    if (list == NULL) {
        return;
    }

    llist_node_t *prev    = NULL;
    llist_node_t *current = list->head;
    while (current != NULL) {
        if (current->data == data) {
            if (prev == NULL) {
                list->head = current->next;
            } else {
                prev->next = current->next;
            }

            free(current);
            list->size--;
            return;
        }

        prev    = current;
        current = current->next;
    }
}

void llist_remove_at(llist_t *list, size_t index) {
    if (list == NULL || index >= list->size) {
        return;
    }

    llist_node_t *prev    = NULL;
    llist_node_t *current = list->head;
    for (size_t i = 0; i < index; ++i) {
        prev    = current;
        current = current->next;
    }

    if (prev == NULL) {
        list->head = current->next;
    } else {
        prev->next = current->next;
    }

    free(current);
    list->size--;
}

void *llist_get(llist_t *list, size_t index) {
    if (list == NULL || index >= list->size) {
        return NULL;
    }

    llist_node_t *current = list->head;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }

    return current->data;
}
