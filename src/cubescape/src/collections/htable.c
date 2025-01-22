#include "collections/htable.h"

#include <stdlib.h>
#include <string.h>

htable_t *htable_create(size_t capacity, size_t key_size, htable_hash_fn hash_fn, htable_compare_fn compare_fn,
                        htable_key_free_fn key_free_fn, htable_data_free_fn data_free_fn) {
    htable_t *table   = malloc(sizeof(htable_t));
    table->entries    = malloc(sizeof(htable_entry_t) * capacity);
    table->capacity   = capacity;
    table->size       = 0;
    table->key_size   = key_size;
    table->hash_fn    = hash_fn;
    table->compare_fn = compare_fn;

    if (key_free_fn == NULL) {
        table->key_free_fn = free;
    } else {
        table->key_free_fn = key_free_fn;
    }

    if (data_free_fn == NULL) {
        table->data_free_fn = free;
    } else {
        table->data_free_fn = data_free_fn;
    }

    memset(table->entries, 0, sizeof(htable_entry_t) * capacity);

    return table;
}

void htable_destroy(htable_t *table) {
    for (size_t i = 0; i < table->size; i++) {
        if (table->entries[i].key) {
            table->key_free_fn(table->entries[i].key);
        }

        if (table->entries[i].value) {
            table->data_free_fn(table->entries[i].value);
        }
    }
    free(table->entries);
    free(table);
}

static void htable_set_entry(htable_entry_t *entries, size_t capacity, size_t key_size, htable_hash_fn hash_fn,
                             htable_compare_fn compare_fn, htable_data_free_fn data_free_fn, void *key, void *value,
                             size_t *size) {
    uint64_t hash = hash_fn(key);
    size_t index  = (size_t)(hash & (uint64_t)(capacity - 1));

    while (entries[index].key != NULL) {
        if (compare_fn(entries[index].key, key)) {
            data_free_fn(entries[index].value);
            entries[index].value = value;
            return;
        }

        ++index;
        if (index >= capacity) {
            index = 0;
        }
    }

    if (size) {
        entries[index].key = malloc(key_size);
        memcpy(entries[index].key, key, key_size);
        ++(*size);
    }

    entries[index].value = value;
}

static void htable_expand(htable_t *table) {
    htable_entry_t *old_entries = table->entries;
    size_t old_capacity         = table->capacity;

    table->capacity *= 2;
    table->entries = malloc(sizeof(htable_entry_t) * table->capacity);
    memset(table->entries, 0, sizeof(htable_entry_t) * table->capacity);
    table->size = 0;

    for (size_t i = 0; i < old_capacity; i++) {
        if (old_entries[i].key) {
            htable_set_entry(table->entries, table->capacity, table->key_size, table->hash_fn, table->compare_fn,
                             table->data_free_fn, old_entries[i].key, old_entries[i].value, NULL);
        }
    }

    free(old_entries);
}

void htable_set(htable_t *table, void *key, void *value) {
    if (table == NULL) {
        return;
    }

    if (table->size >= table->capacity / 2) {
        htable_expand(table);
    }

    htable_set_entry(table->entries, table->capacity, table->key_size, table->hash_fn, table->compare_fn,
                     table->data_free_fn, key, value, &table->size);
}

void *htable_get(htable_t *table, void *key) {
    if (table == NULL) {
        return NULL;
    }

    uint64_t hash = table->hash_fn(key);
    size_t index  = (size_t)(hash & (uint64_t)(table->capacity - 1));

    while (table->entries[index].key != NULL) {
        if (table->compare_fn(table->entries[index].key, key)) {
            return table->entries[index].value;
        }

        ++index;
        if (index >= table->capacity) {
            index = 0;
        }
    }

    return NULL;
}

void htable_remove(htable_t *table, void *key) {
    if (table == NULL) {
        return;
    }

    uint64_t hash = table->hash_fn(key);
    size_t index  = (size_t)(hash & (uint64_t)(table->capacity - 1));

    while (table->entries[index].key != NULL) {
        if (table->compare_fn(table->entries[index].key, key)) {
            table->key_free_fn(table->entries[index].key);
            table->data_free_fn(table->entries[index].value);
            table->entries[index].key  = NULL;
            table->entries[index].value = NULL;
            --table->size;
            return;
        }

        index = (index + 1) & (table->capacity - 1);
    }
}

htable_iter_t htable_iter(htable_t *table) {
    htable_iter_t iter = {0};
    iter.__table       = table;
    iter.__index       = 0;
    return iter;
}

bool htable_next(htable_iter_t *iter) {
    while (iter->__index < iter->__table->capacity) {
        if (iter->__table->entries[iter->__index].key != NULL) {
            iter->key  = iter->__table->entries[iter->__index].key;
            iter->value = iter->__table->entries[iter->__index].value;
            ++iter->__index;
            return true;
        }
        ++iter->__index;
    }
    return false;
}
