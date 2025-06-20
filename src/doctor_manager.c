#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NAMA 50
#define MAX_DOKTER 100

// biar sama
typedef struct {
    char nama[MAX_NAMA];    /* nama dokter */
    int max_shift;          
    int pref_pagi;  // 1 = mau, 0 = ga mau
    int pref_siang;  
    int pref_malam;       
} Dokter;

Dokter dokter[MAX_DOKTER];
int sum_dokter = 0;

// baca file daftar_dokter.csv
void read_csv(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("File %s tidak ditemukan. Data dokter kosong.\n", filename);
        return;
    }
    char line[256];
    fgets(line, sizeof(line), file); // skip header

    while (fgets(line, sizeof(line), file) && sum_dokter < MAX_DOKTER) {
        char *token;
        
        token = strtok(line, ",");
        if (token) strncpy(dokter[sum_dokter].nama, token, MAX_NAMA - 1);
        
        token = strtok(NULL, ",");
        if (token) dokter[sum_dokter].max_shift = atoi(token);
        
        token = strtok(NULL, ",");
        if (token) dokter[sum_dokter].pref_pagi = atoi(token);
        
        token = strtok(NULL, ",");
        if (token) dokter[sum_dokter].pref_siang = atoi(token);

        token = strtok(NULL, ",");
        if (token) dokter[sum_dokter].pref_malam = atoi(token);
        
        sum_dokter++;
    }
    fclose(file);
    printf("Data dokter dari %s berhasil dimuat.\n", filename);
}

// Simpan 5 kolom ke CSV
void simpanCSV(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Gagal membuka file %s\n", filename);
        return;
    }

    // ini buat header
    fprintf(file, "Nama,Maksimal Shift,Pref Pagi,Pref Siang,Pref Malam\n");

    for (int i = 0; i < sum_dokter; i++) {
        // buat tulis perkolomm
        fprintf(file, "%s,%d,%d,%d,%d\n",
                dokter[i].nama,
                dokter[i].max_shift,
                dokter[i].pref_pagi,
                dokter[i].pref_siang,
                dokter[i].pref_malam);
    }
    fclose(file);
    printf("Data berhasil disimpan ke %s\n", filename);
}

// input
void tambahDokter() {
    int a;
    if (sum_dokter >= MAX_DOKTER) {
        printf("Kapasitas dokter penuh!\n");
        return;
    }

    printf("Masukkan nama dokter: ");
    getchar(); // Membersihkan buffer stdin
    fgets(dokter[sum_dokter].nama, MAX_NAMA, stdin);
    dokter[sum_dokter].nama[strcspn(dokter[sum_dokter].nama, "\n")] = 0;

    printf("Masukkan maksimal shift per minggu: ");
    scanf("%d", &dokter[sum_dokter].max_shift);

    printf("Preferensi shift Pagi (1=Ya, 0=Tidak): ");
    scanf("%d", &dokter[sum_dokter].pref_pagi);
    
    printf("Preferensi shift Siang (1=Ya, 0=Tidak): ");
    scanf("%d", &dokter[sum_dokter].pref_siang);

    printf("Preferensi shift Malam (1=Ya, 0=Tidak): ");
    scanf("%d", &dokter[sum_dokter].pref_malam);

    sum_dokter++;
    printf("Dokter berhasil ditambahkan.\n");
}

void hapusDokter() {
    char nama[MAX_NAMA];
    printf("Masukkan nama dokter yang akan dihapus: ");
    getchar();
    fgets(nama, MAX_NAMA, stdin);
    nama[strcspn(nama, "\n")] = 0;

    int found_index = -1;
    for (int i = 0; i < sum_dokter; i++) {
        if (strcmp(dokter[i].nama, nama) == 0) {
            found_index = i;
            break;
        }
    }

    if (found_index != -1) {
        for (int i = found_index; i < sum_dokter - 1; i++) {
            dokter[i] = dokter[i + 1];
        }
        sum_dokter--;
        printf("Dokter %s berhasil dihapus.\n", nama);
    } else {
        printf("Dokter %s tidak ditemukan.\n", nama);
    }
}

// Buat nampilin
void tampilkanDokter() {
    if (sum_dokter == 0) {
        printf("Belum ada data dokter.\n");
        return;
    }

    printf("\n======================================================================\n");
    printf("%-20s | %-10s | %-10s | %-10s | %-10s\n", "Nama", "Maks Shift", "Pagi", "Siang", "Malam");
    printf("======================================================================\n");

    for (int i = 0; i < sum_dokter; i++) {
        printf("%-20s | %-10d | %-10s | %-10s | %-10s\n",
               dokter[i].nama,
               dokter[i].max_shift,
               dokter[i].pref_pagi ? "Ya" : "Tidak",  // Menggunakan ternary operator
               dokter[i].pref_siang ? "Ya" : "Tidak", // untuk tampilan yang lebih baik
               dokter[i].pref_malam ? "Ya" : "Tidak");
    }
    printf("======================================================================\n");
}

int main() {
    // Ganti nama file agar tidak bentrok dengan file jadwal nanti
    const char *filename = "../../data/sample/daftar_dokter.csv";
    read_csv(filename);

    int pilihan;
    do {
        printf("\n--- Manajemen Data Dokter ---\n");
        printf("1. Tambah Dokter\n");
        printf("2. Hapus Dokter\n");
        printf("3. Tampilkan Dokter\n");
        printf("4. Simpan dan Keluar\n");
        printf("Pilih opsi: ");

        int hasil_scan = scanf("%d", &pilihan);

        // biar cuman integer ajah
        if (hasil_scan != 1) {
            while (getchar() != '\n'); 
            pilihan = 0;
            printf("Pilihan tidak valid.\n");
        }
        else {
            switch (pilihan) {
            case 1: tambahDokter(); break;
            case 2: hapusDokter(); break;
            case 3: tampilkanDokter(); break;
            case 4: 
                simpanCSV(filename);
                return 0; // Keluar dari program
            default: 
                printf("Pilihan tidak valid.\n");
        }

        }

        
    } while (1);

    return 0;
}