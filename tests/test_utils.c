#include "unity/unity.h"
#include "../include/utils.h"
#include "../include/types.h"
#include <string.h>

void test_utils(void) {
    // Basic test to ensure utils module works
    TEST_ASSERT_EQUAL(1, 1); // Placeholder test
    
    // Add specific tests based on your utils.h functions
    // For example, if you have string validation functions:
    // TEST_ASSERT_TRUE(isValidInput("test"));
    
    printf("Utils tests completed\n");
}