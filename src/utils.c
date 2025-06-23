/*
Program Utilitas
Fungsi:
1. Mengambil input integer dengan validasi rentang.
2. Membersihkan layar.
3. Mencetak garis pembatas.
4. Memotong spasi pada string.
5. Menampilkan spinner dan progress bar untuk operasi berjalan.
6. Memulai dan mengakhiri progress dengan pesan.
7. Meminta konfirmasi aksi dari pengguna.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "../include/utils.h"

int getIntInput(int min, int max, const char* prompt) {
    int value;
    char input[100];

    while (1) {
        printf("%s", prompt); // Tampilkan prompt ke user
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input. Please try again.\n");
            continue;
        }

        input[strcspn(input, "\n")] = 0; // Hapus karakter newline

        // Cek input kosong
        if (strlen(input) == 0) {
            printf("Input tidak boleh kosong. Silakan coba lagi.\n");
            continue;
        }

        // Konversi string ke integer
        char *endptr;
        value = strtol(input, &endptr, 10);

        // Cek input non-angka
        if (*endptr != '\0') {
            printf("Input harus berupa angka. Silakan coba lagi.\n");
            continue;
        }

        // Pastikan input berada dalam rentang yang ditentukan
        if (value < min || value > max) {
            printf("Input harus antara %d dan %d. Silakan coba lagi.\n", min, max);
            continue;
        }

        return value; 
    }
}

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
    printf("\r                                                                      ");
    printf("\r%s\n", message);
}

int confirmAction(const char* message) {
    char answer[10];
    printf("%s (y/n): ", message);
    if (fgets(answer, sizeof(answer), stdin) == NULL) return 0;
    answer[strcspn(answer, "\n")] = 0;
    return (answer[0] == 'y' || answer[0] == 'Y');
}
