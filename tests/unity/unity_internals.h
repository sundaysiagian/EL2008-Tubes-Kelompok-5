#ifndef UNITY_INTERNALS_H
#define UNITY_INTERNALS_H

#include "unity.h"

typedef struct {
    int testCount;
    int failedCount;
} UnityTestResults;

void UnitySetUp(void);
void UnityTearDown(void);
void UnityRunTests(void);
void UnityTestResultsPrint(UnityTestResults results);

#endif 