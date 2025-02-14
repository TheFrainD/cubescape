#pragma once

#include <stddef.h>
#include <stdint.h>

#include "core/bool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t (*htable_hash_fn)(void *key);
typedef bool_t (*htable_compare_fn)(void *key1, void *key2);
typedef void (*htable_key_free_fn)(void *key);
typedef void (*htable_data_free_fn)(void *value);

typedef struct {
    void *key;
    void *value;
} htable_entry_t;

typedef struct {
    htable_entry_t *entries;
    size_t capacity;
    size_t size;
    size_t key_size;

    htable_hash_fn hash_fn;
    htable_compare_fn compare_fn;
    htable_key_free_fn key_free_fn;
    htable_data_free_fn data_free_fn;
} htable_t;

typedef struct {
    void *key;
    void *value;

    htable_t *__table;
    size_t __index;
} htable_iter_t;

/**
 * @brief Creates a new hash table.
 *
 * This function creates a new hash table with the specified capacity, key size, hash function, compare function, key
 * free function, and value free function.
 *
 * @param capacity The capacity of the hash table.
 * @param key_size The size of the keys in the hash table.
 * @param hash_fn The hash function to use.
 * @param compare_fn The compare function to use.
 * @param key_free_fn The key free function to use.
 * @param data_free_fn The value free function to use.
 * @return htable_t* A pointer to the newly created hash table.
 */
htable_t *htable_create(size_t capacity, size_t key_size, htable_hash_fn hash_fn, htable_compare_fn compare_fn,
                        htable_key_free_fn key_free_fn, htable_data_free_fn data_free_fn);

/**
 * @brief Destroys the specified hash table.
 *
 * This function frees the memory allocated for the specified hash table.
 *
 * @param table A pointer to the hash table to destroy.
 */
void htable_destroy(htable_t *table);

/**
 * @brief Sets the specified key and value in the hash table.
 *
 * This function sets the specified key and value in the hash table.
 *
 * @param table A pointer to the hash table.
 * @param key The key to set.
 * @param value The value to set.
 */
void htable_set(htable_t *table, void *key, void *value);

/**
 * @brief Retrieves the value associated with the specified key in the hash table.
 *
 * This function retrieves the value associated with the specified key in the hash table.
 *
 * @param table A pointer to the hash table.
 * @param key The key to retrieve the value for.
 * @return void* The value associated with the specified key.
 */
void *htable_get(htable_t *table, void *key);

/**
 * @brief Removes the specified key and associated value from the hash table.
 *
 * This function removes the specified key and associated value from the hash table.
 *
 * @param table A pointer to the hash table.
 * @param key The key to remove.
 */
void htable_remove(htable_t *table, void *key);

/**
 * @brief Creates an iterator for the hash table.
 *
 * This function creates an iterator for the hash table.
 *
 * @param table A pointer to the hash table.
 * @return htable_iter_t The created iterator.
 */
htable_iter_t htable_iter(htable_t *table);

/**
 * @brief Advances the iterator to the next entry in the hash table.
 *
 * This function advances the iterator to the next entry in the hash table.
 *
 * @param iter A pointer to the iterator.
 * @return bool_t Whether the iterator was advanced.
 */
bool_t htable_next(htable_iter_t *iter);

#ifdef __cplusplus
}
#endif