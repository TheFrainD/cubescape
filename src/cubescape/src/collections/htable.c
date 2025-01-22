#include "collections/htable.h"

#include <stdlib.h>
#include <string.h>

#include <cubelog/cubelog.h>

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
        void *original_key = key;
        key = malloc(key_size);
        memcpy(key, original_key, key_size);
        ++(*size);
    }

    entries[index].key   = key;
    entries[index].value = value;
}

static void htable_expand(htable_t *table) {
    size_t new_capacity = table->capacity * 2;
    if (new_capacity < table->capacity) {
        CUBELOG_ERROR("Failed to expand hash table: capacity overflow");
        return;
    }

    htable_entry_t *new_entries = malloc(sizeof(htable_entry_t) * new_capacity);
    memset(new_entries, 0, sizeof(htable_entry_t) * new_capacity);

    for (size_t i = 0; i < table->capacity; ++i) {
        if (table->entries[i].key != NULL) {
            htable_set_entry(new_entries, new_capacity, table->key_size, table->hash_fn, table->compare_fn,
                             table->data_free_fn, table->entries[i].key, table->entries[i].value, NULL);
        }
    }

    free(table->entries);
    table->entries  = new_entries;
    table->capacity = new_capacity;
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

        ++index;
        if (index >= table->capacity) {
            index = 0;
        }
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
