#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NAMA 50
#define MAX_DOKTER 100

typedef struct {
    char nama[MAX_NAMA];   /* nama dokter */
    int max_shift;        
    char shift_pref[10];  /* berupa pagi/siang/malam */
} Dokter;

Dokter dokter[MAX_DOKTER];
int sum_dokter = 0;

void read_csv(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("File %s tidak ditemukan, membuat file baru.\n", filename);
        return;
    }
    char line[256];
    fgets(line, sizeof(line), file); // Skip header
    while (fgets(line, sizeof(line), file) && sum_dokter < MAX_DOKTER) {
        char *token = strtok(line, ",");
        if (token) strncpy(dokter[sum_dokter].nama, token, MAX_NAMA - 1);
        
        token = strtok(NULL, ",");
        if (token) dokter[sum_dokter].max_shift = atoi(token);
        
        token = strtok(NULL, ",");
        if (token) {
            token[strcspn(token, "\n")] = 0; // Hapus newline
            strncpy(dokter[sum_dokter].shift_pref, token, 9);
        }
        sum_dokter++;
    }
    fclose(file);
}

void simpanCSV(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Gagal membuka file", filename);
        return;
    }

    fprintf(file, "Nama,Maksimal Shift,Preferensi Shift\n");
    for (int i = 0; i < sum_dokter; i++) {
        fprintf(file, "%s,%d,%s\n", dokter[i].nama, dokter[i].max_shift, dokter[i].shift_pref);
    }
    fclose(file);
    printf("Data berhasil disimpan", filename);
}

void tambahDokter() {
    if (sum_dokter >= MAX_DOKTER) {
        printf("Kapasitas dokter penuh!\n");
        return;
    }

    printf("Masukkan nama dokter: ");
    getchar(); 
    fgets(dokter[sum_dokter].nama, MAX_NAMA, stdin);
    dokter[sum_dokter].nama[strcspn(dokter[sum_dokter].nama, "\n")] = 0;

    printf("Masukkan maksimal shift per minggu: ");
    scanf("%d", &dokter[sum_dokter].max_shift);

    printf("Masukkan preferensi shift (Pagi/Siang/Malam): ");
    getchar();
    fgets(dokter[sum_dokter].shift_pref, 10, stdin);
    dokter[sum_dokter].shift_pref[strcspn(dokter[sum_dokter].shift_pref, "\n")] = 0;

    sum_dokter++;
    printf("Dokter berhasil ditambahkan.\n");
}

void hapusDokter() {
    char nama[MAX_NAMA];
    printf("Masukkan nama dokter yang akan dihapus: ");
    getchar();
    fgets(nama, MAX_NAMA, stdin);
    nama[strcspn(nama, "\n")] = 0;

    int found = 0;
    for (int i = 0; i < sum_dokter; i++) {
        if (strcmp(dokter[i].nama, nama) == 0) {
            for (int j = i; j < sum_dokter - 1; j++) {
                dokter[j] = dokter[j + 1];
            }
            sum_dokter--;
            found = 1;
            printf("Dokter %s berhasil dihapus.\n", nama);
            break;
        }
    }
    if (!found) printf("Dokter %s tidak ditemukan.\n", nama);
}

void tampilkanDokter() {
    if (sum_dokter == 0) {
        printf("Belum ada data dokter.\n");
        return;
    }

    printf("\nDaftar Dokter:\n");
    printf("----------------------------------------\n");
    printf("Nama\t\tMax Shift\tPreferensi\n");
    printf("----------------------------------------\n");
    for (int i = 0; i < sum_dokter; i++) {
        printf("%s\t\t%d\t\t%s\n", dokter[i].nama, dokter[i].max_shift, dokter[i].shift_pref);
    }
    printf("----------------------------------------\n");
}

int main() {
    const char *filename = "dokter.csv";
    read_csv(filename);

    int pilihan;
    do {
        printf("\nManajemen Data Dokter\n");
        printf("1. Tambah Dokter\n");
        printf("2. Hapus Dokter\n");
        printf("3. Tampilkan Dokter\n");
        printf("4. Simpan dan Keluar\n");
        printf("Pilih opsi: ");
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1: tambahDokter(); break;
            case 2: hapusDokter(); break;
            case 3: tampilkanDokter(); break;
            case 4: simpanCSV(filename); return 0;
            default: printf("Pilihan tidak valid.\n");
        }
    } while (1);

    return 0;
}
