#ifndef UNITY_H
#define UNITY_H

#include <stdio.h>
#include <stdlib.h>

typedef void (*TestFunction)(void);

void UnityBegin(const char* filename);
void UnityEnd(void);
void UnityAssertEqualInt(int expected, int actual, const char* message, int line);
void UnityAssertEqualString(const char* expected, const char* actual, const char* message, int line);
void UnityRunTests(TestFunction testFunction);

#endif // UNITY_H