
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> // Untuk tipe data bool

#define MAX_DOKTER 50 
#define MAX_LINE_LEN 256
#define TOTAL_SHIFT 90

//struct
typedef struct {
    char nama[50];
    int maks_shift_mingguan;
    int pref_pagi;
    int pref_siang;
    int pref_malam;
} Dokter;

// shift
typedef struct {
    int hari_ke;
    char tipe_shift[10];
    Dokter* dokter_bertugas;
} Shift;


Dokter daftar_dokter[MAX_DOKTER];
int jumlah_dokter = 0;



// dklarasi fungsi-fungsi
bool apakahAlokasiValid(Shift jadwal[], Dokter* dokter, int index_shift) {
    int hari_ini = index_shift / 3;

    // Cek apa ada jadwal ganda di hari sama
    int shift_awal_hari = hari_ini * 3;
    for (int i = shift_awal_hari; i < index_shift; i++) {
        if (jadwal[i].dokter_bertugas != NULL && jadwal[i].dokter_bertugas == dokter) {
            return false;
        }
    }

    // cek batas maks shift per minggu
    int minggu_ini = hari_ini / 7;
    int shift_awal_minggu = minggu_ini * (7 * 3);
    int jumlah_shift_minggu_ini = 0;
    for (int i = shift_awal_minggu; i < index_shift; i++) {
        if (jadwal[i].dokter_bertugas != NULL && jadwal[i].dokter_bertugas == dokter) {
            jumlah_shift_minggu_ini++;
        }
    }
    if (jumlah_shift_minggu_ini >= dokter->maks_shift_mingguan) {
        return false;
    }
    
    // terakhir liat preferensi
    // Jika dokter tidak preferensi shift ini, anggap tidak valid agar algoritma
    // mencari dokter lain yang lebih cocok. Ini adalah cara sederhana untuk optimasi.
    if (strcmp(jadwal[index_shift].tipe_shift, "pagi") == 0 && dokter->pref_pagi == 0) return false;
    if (strcmp(jadwal[index_shift].tipe_shift, "siang") == 0 && dokter->pref_siang == 0) return false;
    if (strcmp(jadwal[index_shift].tipe_shift, "malam") == 0 && dokter->pref_malam == 0) return false;


    return true;
}


bool solveJadwal(Shift jadwal[], int index_shift) {
    if (index_shift == TOTAL_SHIFT) {
        return true;
    }

    for (int i = 0; i < jumlah_dokter; i++) {
        if (apakahAlokasiValid(jadwal, &daftar_dokter[i], index_shift)) {
            jadwal[index_shift].dokter_bertugas = &daftar_dokter[i];

            if (solveJadwal(jadwal, index_shift + 1)) {
                return true;
            }

            jadwal[index_shift].dokter_bertugas = NULL; // BACKTRACK
        }
    }
    return false;
}

void inisialisasiJadwal(Shift jadwal[]) {
    char tipe[3][10] = {"pagi", "siang", "malam"};
    for (int i = 0; i < TOTAL_SHIFT; i++) {
        jadwal[i].hari_ke = i / 3;
        strcpy(jadwal[i].tipe_shift, tipe[i % 3]);
        jadwal[i].dokter_bertugas = NULL;
    }
}

void tampilkanJadwal(Shift jadwal[]) {
    printf("====================================================\n");
    printf("| %-5s | %-10s | %-10s | %-20s |\n", "Hari", "Shift", "Tipe", "Dokter Bertugas");
    printf("====================================================\n");
    for (int i = 0; i < TOTAL_SHIFT; i++) {
        printf("| %-5d | Shift %-5d | %-10s | %-20s |\n",
               jadwal[i].hari_ke + 1,
               i + 1,
               jadwal[i].tipe_shift,
               jadwal[i].dokter_bertugas ? jadwal[i].dokter_bertugas->nama : "KOSONG");
        if ((i + 1) % 3 == 0) {
            printf("----------------------------------------------------\n");
        }
    }
}

void simpanJadwalKeCSV(Shift jadwal[], const char* nama_file) {
    printf("\nMenyimpan jadwal ke file %s...\n", nama_file);

    // write file
    FILE *file = fopen(nama_file, "w");
    if (file == NULL) {
        perror("Error: Gagal membuka file untuk ditulis");
        return; // Keluar dari fungsi jika gagal
    }

    // // buat header
    fprintf(file, "Hari,Tipe Shift,Nama Dokter\n");

    // tulis data
    for (int i = 0; i < TOTAL_SHIFT; i++) {
        // Ambil nama dokter 
        const char* nama_dokter = "KOSONG";
        if (jadwal[i].dokter_bertugas != NULL) {
            nama_dokter = jadwal[i].dokter_bertugas->nama;
        }

        fprintf(file, "%d,%s,%s\n",
                jadwal[i].hari_ke + 1, 
                jadwal[i].tipe_shift,
                nama_dokter);
    }

    //close
    fclose(file);
    printf("Jadwal berhasil dibuat dan disimpan.\n");
}

int muatDataDokter(const char* nama_file) {
    FILE *file = fopen(nama_file, "r");
    if (file == NULL) {
        perror("Error: Tidak bisa membuka file");
        return -1;
    }
    char line[MAX_LINE_LEN];
    fgets(line, sizeof(line), file); // Skip header

    int count = 0;
    while (fgets(line, sizeof(line), file) && count < MAX_DOKTER) {
        line[strcspn(line, "\n")] = 0;
        char *token;
        token = strtok(line, ","); 
        if (token) {
            strcpy(daftar_dokter[count].nama, token);
        }
        token = strtok(NULL, ","); 
        if (token) {
            daftar_dokter[count].maks_shift_mingguan = atoi(token);
        }
        token = strtok(NULL, ",");
        
        if (token) {
            daftar_dokter[count].pref_pagi = atoi(token);
        }
        token = strtok(NULL, ","); 
        if (token) {
            daftar_dokter[count].pref_siang = atoi(token);
        }
        token = strtok(NULL, ","); 
        if (token) {
            daftar_dokter[count].pref_malam = atoi(token);
        }
        count++;
    }
    fclose(file);
    return count;
}




// INI MAINNYA========================================
int main() {
    jumlah_dokter = muatDataDokter("../../data/sample/daftar_dokter.csv");

    if (jumlah_dokter == -1) {
        return 1;
    }
    printf("Data %d dokter berhasil dimuat.\n\n", jumlah_dokter);

    Shift jadwal[TOTAL_SHIFT];
    inisialisasiJadwal(jadwal);

    if (solveJadwal(jadwal, 0)) {
        printf("Jadwal berhasil dibuat!\n\n");
        simpanJadwalKeCSV(jadwal, "../../data/sample/jadwal_dokter.csv");
        // tampilkanJadwal(jadwal);
    } else {
        printf("Gagal membuat jadwal. Tidak ada solusi yang memungkinkan dengan batasan yang ada.\n");
    }

    return 0;
}

//====================================================












