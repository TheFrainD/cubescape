#include "collections/htable.h"

#include <unity.h>

static uint64_t hash_fn(void *key) { return *(uint64_t *)key; }

static bool_t compare_fn(void *a, void *b) { return *(uint64_t *)a == *(uint64_t *)b; }

void test_htable_create(void) {
    htable_t *table = htable_create(10, sizeof(uint64_t), hash_fn, compare_fn, NULL, NULL);
    TEST_ASSERT_NOT_NULL(table);
    TEST_ASSERT_EQUAL(10, table->capacity);
    TEST_ASSERT_EQUAL(0, table->size);
    htable_destroy(table);
}

void test_htable_set_get(void) {
    htable_t *table = htable_create(10, sizeof(uint64_t), hash_fn, compare_fn, NULL, NULL);
    uint64_t key    = 1;
    uint64_t value  = 42;
    htable_set(table, &key, &value);
    uint64_t *retrieved_value = htable_get(table, &key);
    TEST_ASSERT_NOT_NULL(retrieved_value);
    TEST_ASSERT_EQUAL(42, *retrieved_value);
    htable_destroy(table);
}

void test_htable_remove(void) {
    htable_t *table = htable_create(10, sizeof(uint64_t), hash_fn, compare_fn, NULL, NULL);
    uint64_t key    = 1;
    uint64_t value  = 42;
    htable_set(table, &key, &value);
    htable_remove(table, &key);
    uint64_t *retrieved_value = htable_get(table, &key);
    TEST_ASSERT_NULL(retrieved_value);
    htable_destroy(table);
}

void test_htable_expand(void) {
    htable_t *table = htable_create(2, sizeof(uint64_t), hash_fn, compare_fn, NULL, NULL);
    uint64_t key1 = 1, key2 = 2, key3 = 3;
    uint64_t value1 = 42, value2 = 43, value3 = 44;
    htable_set(table, &key1, &value1);
    htable_set(table, &key2, &value2);
    htable_set(table, &key3, &value3);
    TEST_ASSERT_EQUAL(3, table->size);
    htable_destroy(table);
}

void test_htable_collision(void) {
    htable_t *table = htable_create(2, sizeof(uint64_t), hash_fn, compare_fn, NULL, NULL);
    uint64_t key1 = 1, key2 = 3;  // These keys will collide in a table of size 2
    uint64_t value1 = 42, value2 = 43;
    htable_set(table, &key1, &value1);
    htable_set(table, &key2, &value2);
    uint64_t *retrieved_value1 = htable_get(table, &key1);
    uint64_t *retrieved_value2 = htable_get(table, &key2);
    TEST_ASSERT_NOT_NULL(retrieved_value1);
    TEST_ASSERT_NOT_NULL(retrieved_value2);
    TEST_ASSERT_EQUAL(42, *retrieved_value1);
    TEST_ASSERT_EQUAL(43, *retrieved_value2);
    htable_destroy(table);
}

void test_htable_iter(void) {
    htable_t *table = htable_create(10, sizeof(uint64_t), hash_fn, compare_fn, NULL, NULL);
    uint64_t key1 = 1, key2 = 2;
    uint64_t value1 = 42, value2 = 43;
    htable_set(table, &key1, &value1);
    htable_set(table, &key2, &value2);

    htable_iter_t iter = htable_iter(table);
    uint64_t iterated_keys[2];
    uint64_t iterated_values[2];
    size_t index = 0;

    while (htable_next(&iter)) {
        iterated_keys[index]   = *(uint64_t *)iter.key;
        iterated_values[index] = *(uint64_t *)iter.value;
        index++;
    }

    TEST_ASSERT_EQUAL(2, index);
    TEST_ASSERT_TRUE((iterated_keys[0] == 1 && iterated_values[0] == 42) ||
                     (iterated_keys[0] == 2 && iterated_values[0] == 43));
    TEST_ASSERT_TRUE((iterated_keys[1] == 1 && iterated_values[1] == 42) ||
                     (iterated_keys[1] == 2 && iterated_values[1] == 43));

    htable_destroy(table);
}

void register_htable_tests() {
    RUN_TEST(test_htable_create);
    RUN_TEST(test_htable_set_get);
    RUN_TEST(test_htable_remove);
    RUN_TEST(test_htable_expand);
    RUN_TEST(test_htable_collision);
    RUN_TEST(test_htable_iter);
}
