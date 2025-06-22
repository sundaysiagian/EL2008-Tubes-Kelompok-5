#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "../include/utils.h"

void clearScreen() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

void printLine(char sym, int length) {
    for (int i = 0; i < length; i++) {
        putchar(sym);
    }
    putchar('\n');
}

char* trimString(char* str) {
    char* end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return str;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    *(end+1) = 0;
    return str;
}

void showSpinner(int position) {
    const char spinner[] = {'|', '/', '-', '\\'};
    printf("\r%c ", spinner[position % 4]);
    fflush(stdout);
}

void updateProgressBar(int current, int total, int width) {
    float progress = (float)current / total;
    int pos = width * progress;
    printf("\r[");
    for (int i = 0; i < width; i++) {
        if (i < pos) printf("=");
        else if (i == pos) printf(">");
        else printf(" ");
    }
    printf("] %d%%", (int)(progress * 100));
    fflush(stdout);
}

void startProgress(const char* message) {
    printf("%s ", message);
    fflush(stdout);
}

void endProgress(const char* message) {
    printf("\r%s\n", message);
}

int confirmAction(const char* message) {
    char answer[10];
    printf("%s (y/n): ", message);
    if (fgets(answer, sizeof(answer), stdin) == NULL) return 0;
    answer[strcspn(answer, "\n")] = 0;
    return (answer[0] == 'y' || answer[0] == 'Y');
}

void printColored(const char* text) {
    printf("%s", text);
}