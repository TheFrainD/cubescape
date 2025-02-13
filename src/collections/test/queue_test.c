#include "collections/queue.h"

#include <unity.h>

void test_queue_create_destroy(void) {
    queue_t *queue = queue_create();
    TEST_ASSERT_NOT_NULL(queue);
    TEST_ASSERT_EQUAL_UINT32(0, queue_size(queue));
    queue_destroy(queue);
}

void test_queue_push_pop(void) {
    queue_t *queue = queue_create();
    int values[]   = {10, 20, 30};

    queue_push(queue, &values[0]);
    queue_push(queue, &values[1]);
    queue_push(queue, &values[2]);

    TEST_ASSERT_EQUAL_PTR(&values[0], queue_pop(queue));
    TEST_ASSERT_EQUAL_PTR(&values[1], queue_pop(queue));
    TEST_ASSERT_EQUAL_PTR(&values[2], queue_pop(queue));

    queue_destroy(queue);
}

void test_queue_front_back(void) {
    queue_t *queue = queue_create();
    int values[]   = {100, 200, 300};

    queue_push(queue, &values[0]);
    queue_push(queue, &values[1]);
    queue_push(queue, &values[2]);

    TEST_ASSERT_EQUAL_PTR(&values[0], queue_front(queue));
    TEST_ASSERT_EQUAL_PTR(&values[2], queue_back(queue));

    queue_destroy(queue);
}

void test_queue_empty_behavior(void) {
    queue_t *queue = queue_create();

    TEST_ASSERT_NULL(queue_pop(queue));
    TEST_ASSERT_NULL(queue_front(queue));
    TEST_ASSERT_NULL(queue_back(queue));

    queue_destroy(queue);
}

void test_queue_overflow(void) {
    queue_t *queue = queue_create();
    int values[]   = {1, 2, 3};

    queue_push(queue, &values[0]);
    queue_push(queue, &values[1]);

    // This should fail silently or handle overflow properly
    queue_push(queue, &values[2]);

    TEST_ASSERT_EQUAL_PTR(&values[0], queue_pop(queue));
    TEST_ASSERT_EQUAL_PTR(&values[1], queue_pop(queue));

    queue_destroy(queue);
}

void register_queue_tests() {
    RUN_TEST(test_queue_create_destroy);
    RUN_TEST(test_queue_push_pop);
    RUN_TEST(test_queue_front_back);
    RUN_TEST(test_queue_empty_behavior);
    RUN_TEST(test_queue_overflow);
}
