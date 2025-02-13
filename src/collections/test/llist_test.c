#include "collections/llist.h"

#include <unity.h>

void test_llist_create() {
    llist_t *list = llist_create();
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_NULL(list->head);
    TEST_ASSERT_EQUAL_INT(0, list->size);
    llist_destroy(list);
}

void test_llist_append() {
    llist_t *list = llist_create();
    int data1 = 1, data2 = 2, data3 = 3;

    llist_append(list, &data1);
    TEST_ASSERT_EQUAL_INT(1, list->size);
    TEST_ASSERT_EQUAL_INT(data1, *(int *)list->head->data);

    llist_append(list, &data2);
    TEST_ASSERT_EQUAL_INT(2, list->size);
    TEST_ASSERT_EQUAL_INT(data2, *(int *)list->head->next->data);

    llist_append(list, &data3);
    TEST_ASSERT_EQUAL_INT(3, list->size);
    TEST_ASSERT_EQUAL_INT(data3, *(int *)list->head->next->next->data);

    llist_destroy(list);
}

void test_llist_remove() {
    llist_t *list = llist_create();
    int data1 = 1, data2 = 2, data3 = 3;

    llist_append(list, &data1);
    llist_append(list, &data2);
    llist_append(list, &data3);

    llist_remove(list, &data2);
    TEST_ASSERT_EQUAL_INT(2, list->size);
    TEST_ASSERT_EQUAL_INT(data1, *(int *)list->head->data);
    TEST_ASSERT_EQUAL_INT(data3, *(int *)list->head->next->data);

    llist_remove(list, &data1);
    TEST_ASSERT_EQUAL_INT(1, list->size);
    TEST_ASSERT_EQUAL_INT(data3, *(int *)list->head->data);

    llist_remove(list, &data3);
    TEST_ASSERT_EQUAL_INT(0, list->size);
    TEST_ASSERT_NULL(list->head);

    llist_destroy(list);
}

void test_llist_remove_at() {
    llist_t *list = llist_create();
    int data1 = 1, data2 = 2, data3 = 3;

    llist_append(list, &data1);
    llist_append(list, &data2);
    llist_append(list, &data3);

    llist_remove_at(list, 1);
    TEST_ASSERT_EQUAL_INT(2, list->size);
    TEST_ASSERT_EQUAL_INT(data1, *(int *)list->head->data);
    TEST_ASSERT_EQUAL_INT(data3, *(int *)list->head->next->data);

    llist_remove_at(list, 0);
    TEST_ASSERT_EQUAL_INT(1, list->size);
    TEST_ASSERT_EQUAL_INT(data3, *(int *)list->head->data);

    llist_remove_at(list, 0);
    TEST_ASSERT_EQUAL_INT(0, list->size);
    TEST_ASSERT_NULL(list->head);

    llist_destroy(list);
}

void test_llist_get() {
    llist_t *list = llist_create();
    int data1 = 1, data2 = 2, data3 = 3;

    llist_append(list, &data1);
    llist_append(list, &data2);
    llist_append(list, &data3);

    TEST_ASSERT_EQUAL_INT(data1, *(int *)llist_get(list, 0));
    TEST_ASSERT_EQUAL_INT(data2, *(int *)llist_get(list, 1));
    TEST_ASSERT_EQUAL_INT(data3, *(int *)llist_get(list, 2));

    llist_destroy(list);
}

void test_llist_get_out_of_bounds() {
    llist_t *list = llist_create();
    int data1     = 1;

    llist_append(list, &data1);

    TEST_ASSERT_NULL(llist_get(list, 1));
    TEST_ASSERT_NULL(llist_get(list, 2));

    llist_destroy(list);
}

void register_llist_tests() {
    RUN_TEST(test_llist_create);
    RUN_TEST(test_llist_append);
    RUN_TEST(test_llist_remove);
    RUN_TEST(test_llist_remove_at);
    RUN_TEST(test_llist_get);
    RUN_TEST(test_llist_get_out_of_bounds);
}