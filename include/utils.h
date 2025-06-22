#ifndef UTILS_H
#define UTILS_H

void clearScreen();
void printLine(char sym, int length);
char* trimString(char* str);
void showSpinner(int position);
void updateProgressBar(int current, int total, int width);
void startProgress(const char* message);
void endProgress(const char* message);
int confirmAction(const char* message);
void printColored(const char* text);

#endif