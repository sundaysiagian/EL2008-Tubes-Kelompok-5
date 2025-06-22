#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "../include/types.h"
#include "../include/doctor_manager.h"
#include "../include/utils.h"

void tambahDokter(Dokter daftar[], int *jumlah, const char *nama, int maks_shift, int pref_pagi, int pref_siang, int pref_malam) {
    if (nama == NULL || strlen(nama) == 0) {
        printf("Error: Nama dokter tidak boleh kosong.\n");
        return;
    }

    if (maks_shift < 1) {
        printf("Error: Maksimal shift harus minimal 1.\n");
        return;
    }

    if (*jumlah >= MAX_DOCTORS) {
        printf("Error: Kapasitas dokter penuh!\n");
        return;
    }

    for (int i = 0; i < *jumlah; i++) {
        if (strcmp(daftar[i].nama, nama) == 0) {
            printf("Error: Dokter dengan nama %s sudah ada.\n", nama);
            return;
        }
    }

    strncpy(daftar[*jumlah].nama, nama, 49);
    daftar[*jumlah].nama[49] = '\0';
    daftar[*jumlah].maks_shift_mingguan = maks_shift;
    daftar[*jumlah].pref_pagi = pref_pagi ? 1 : 0;
    daftar[*jumlah].pref_siang = pref_siang ? 1 : 0;
    daftar[*jumlah].pref_malam = pref_malam ? 1 : 0;

    (*jumlah)++;
    printf("Dokter %s berhasil ditambahkan.\n", nama);
}

void hapusDokter(Dokter daftar[], int *jumlah, const char *nama) {
    if (nama == NULL || strlen(nama) == 0) {
        printf("Error: Nama dokter tidak boleh kosong.\n");
        return;
    }

    int found_index = -1;

    for (int i = 0; i < *jumlah; i++) {
        if (strcmp(daftar[i].nama, nama) == 0) {
            found_index = i;
            break;
        }
    }

    if (found_index != -1) {
        for (int i = found_index; i < *jumlah - 1; i++) {
            daftar[i] = daftar[i + 1];
        }
        (*jumlah)--;
        printf("Dokter %s berhasil dihapus.\n", nama);
    } else {
        printf("Error: Dokter %s tidak ditemukan.\n", nama);
    }
}

void tampilkanDaftarDokter(Dokter daftar[], int jumlah) {
    if (jumlah == 0) {
        printf("Belum ada data dokter.\n");
        return;
    }

    printf("\n======================================================================\n");
    printf("%-20s | %-10s | %-10s | %-10s | %-10s\n", "Nama", "Maks Shift", "Pagi", "Siang", "Malam");
    printf("======================================================================\n");

    for (int i = 0; i < jumlah; i++) {
        printf("%-20s | %-10d | %-10s | %-10s | %-10s\n",
               daftar[i].nama,
               daftar[i].maks_shift_mingguan,
               daftar[i].pref_pagi ? "Ya" : "Tidak",
               daftar[i].pref_siang ? "Ya" : "Tidak",
               daftar[i].pref_malam ? "Ya" : "Tidak");
    }
    printf("======================================================================\n");
}

int bacaDokterDariCSV(const char *filename, Dokter daftar[], int *jumlah) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: File %s tidak ditemukan atau tidak bisa dibuka.\n", filename);
        return 0;
    }

    char line[256];
    if (!fgets(line, sizeof(line), file)) {
        printf("Error: Format file CSV tidak valid (header tidak ditemukan).\n");
        fclose(file);
        return 0;
    }

    *jumlah = 0;
    int line_num = 1;

    while (fgets(line, sizeof(line), file) && *jumlah < MAX_DOCTORS) {
        line_num++;
        line[strcspn(line, "\n")] = 0;

        if (strlen(trimString(line)) == 0) continue;

        char *token = strtok(line, ",");
        if (!token) {
            printf("Warning: Format data tidak valid pada baris %d, melewati...\n", line_num);
            continue;
        }

        strncpy(daftar[*jumlah].nama, token, 49);
        daftar[*jumlah].nama[49] = '\0';

        token = strtok(NULL, ",");
        if (!token) {
            printf("Warning: Data tidak lengkap pada baris %d, menggunakan nilai default.\n", line_num);
            daftar[*jumlah].maks_shift_mingguan = 5;
        } else {
            daftar[*jumlah].maks_shift_mingguan = atoi(token);
            if (daftar[*jumlah].maks_shift_mingguan < 1) {
                printf("Warning: Maks shift tidak valid pada baris %d, menggunakan nilai 5.\n", line_num);
                daftar[*jumlah].maks_shift_mingguan = 5;
            }
        }

        token = strtok(NULL, ",");
        daftar[*jumlah].pref_pagi = token ? atoi(token) : 1;
        token = strtok(NULL, ",");
        daftar[*jumlah].pref_siang = token ? atoi(token) : 1;
        token = strtok(NULL, ",");
        daftar[*jumlah].pref_malam = token ? atoi(token) : 1;

        (*jumlah)++;
    }

    if (*jumlah == MAX_DOCTORS && !feof(file)) {
        printf("Warning: Jumlah dokter melebihi kapasitas. Hanya %d dokter pertama yang dimuat.\n", MAX_DOCTORS);
    }

    fclose(file);
    return 1;
}

int simpanDokterKeCSV(const char *filename, Dokter daftar[], int jumlah) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error: Gagal membuka file %s untuk disimpan.\n", filename);
        return 0;
    }

    if (fprintf(file, "Nama,Maksimal Shift,Pref Pagi,Pref Siang,Pref Malam\n") < 0) {
        printf("Error: Gagal menulis data ke file.\n");
        fclose(file);
        return 0;
    }

    int success = 1;
    for (int i = 0; i < jumlah; i++) {
        if (fprintf(file, "%s,%d,%d,%d,%d\n",
                daftar[i].nama,
                daftar[i].maks_shift_mingguan,
                daftar[i].pref_pagi,
                daftar[i].pref_siang,
                daftar[i].pref_malam) < 0) {
            
            printf("Error: Gagal menulis data ke file.\n");
            success = 0;
            break;
        }
    }

    fclose(file);

    if (success) {
        printf("Data %d dokter berhasil disimpan ke %s\n", jumlah, filename);
    }

    return success;
}