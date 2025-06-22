#include "unity.h"
#include "test_doctor_manager.h"
#include "test_scheduler.h"

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_doctor_manager);
    RUN_TEST(test_scheduler);
    return UNITY_END();
}