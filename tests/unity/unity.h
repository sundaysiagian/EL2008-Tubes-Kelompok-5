#ifndef UNITY_H
#define UNITY_H

#include <stdio.h>
#include <stdlib.h>

typedef void (*TestFunction)(void);

// Core Unity functions
void UnityBegin(const char* filename);
int UnityEnd(void);
void UnityAssertEqualInt(int expected, int actual, const char* message, int line);
void UnityAssertEqualString(const char* expected, const char* actual, const char* message, int line);

// Macros
#define UNITY_BEGIN() UnityBegin(__FILE__)
#define UNITY_END() UnityEnd()
#define TEST_ASSERT_EQUAL(expected, actual) UnityAssertEqualInt(expected, actual, "Value should be equal", __LINE__)
#define TEST_ASSERT_EQUAL_STRING(expected, actual) UnityAssertEqualString(expected, actual, "Strings should be equal", __LINE__)
#define RUN_TEST(test) do { printf("Running %s...\n", #test); test(); } while (0)

#endif // UNITY_H