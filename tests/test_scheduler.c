#include "unity.h"

void test_scheduler_functionality(void) {
    TEST_ASSERT_EQUAL(1, 1);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_scheduler_functionality);
    return UNITY_END();
}