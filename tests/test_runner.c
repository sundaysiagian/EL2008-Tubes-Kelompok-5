#include "unity/unity.h"

void test_doctor_manager(void);
void test_scheduler_functionality(void);
void test_utils(void);
void test_display(void);

int main(void) {
    UNITY_BEGIN();
    
    printf("\n=== RUNNING ALL TESTS ===\n\n");

    RUN_TEST(test_doctor_manager);
    RUN_TEST(test_scheduler_functionality);
    RUN_TEST(test_utils);
    RUN_TEST(test_display);
    
    printf("\n=== ALL TESTS COMPLETED ===\n");
    
    return UNITY_END();
}