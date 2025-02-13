#include "unity.h"

void setUp(void) {}

void tearDown(void) {}

extern void register_htable_tests();
extern void register_llist_tests();
extern void register_queue_tests();

int main(int argc, char **argv) {
    UNITY_BEGIN();
    register_htable_tests();
    register_llist_tests();
    register_queue_tests();
    return UNITY_END();
}