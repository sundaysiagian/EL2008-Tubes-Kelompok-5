#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unity.h"

static int testsPassed = 0;
static int testsFailed = 0;
static const char* currentFileName;

void UnityBegin(const char* filename) {
    testsPassed = 0;
    testsFailed = 0;
    currentFileName = filename;
    printf("---- Testing: %s ----\n", filename);
}

int UnityEnd(void) {
    printf("--------------------\n");
    printf("Tests passed: %d\n", testsPassed);
    printf("Tests failed: %d\n", testsFailed);
    printf("--------------------\n");
    return testsFailed;
}

void UnityAssertEqualInt(int expected, int actual, const char* message, int line) {
    if (expected == actual) {
        testsPassed++;
        printf("PASS: %s (line %d)\n", message, line);
    } else {
        testsFailed++;
        printf("FAIL at line %d: %s\n", line, message);
        printf("  Expected: %d\n", expected);
        printf("  Actual:   %d\n", actual);
    }
}

void UnityAssertEqualString(const char* expected, const char* actual, const char* message, int line) {
    if (expected == NULL && actual == NULL) {
        testsPassed++;
        printf("PASS: %s (line %d)\n", message, line);
        return;
    }
    if (expected == NULL || actual == NULL) {
        testsFailed++;
        printf("FAIL at line %d: %s\n", line, message);
        printf("  One string is NULL\n");
        return;
    }
    if (strcmp(expected, actual) == 0) {
        testsPassed++;
        printf("PASS: %s (line %d)\n", message, line);
    } else {
        testsFailed++;
        printf("FAIL at line %d: %s\n", line, message);
        printf("  Expected: '%s'\n", expected);
        printf("  Actual:   '%s'\n", actual);
    }
}