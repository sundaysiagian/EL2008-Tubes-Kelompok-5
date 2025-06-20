
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
    // Jika dokter pref, cari yg lain
    if (strcmp(jadwal[index_shift].tipe_shift, "pagi") == 0 && dokter->pref_pagi == 0){
        return false;
    }
    if (strcmp(jadwal[index_shift].tipe_shift, "siang") == 0 && dokter->pref_siang == 0){
        return false;
    }
    if (strcmp(jadwal[index_shift].tipe_shift, "malam") == 0 && dokter->pref_malam == 0){
        return false;
    }
    return true;
}


bool solveJadwal(Shift jadwal[], int idx) {
    if (idx == TOTAL_SHIFT) {
        return true;
    }

    for (int j = 0; j < jumlah_dokter; j++) {
        Dokter* d = &daftar_dokter[j];

        if (apakahAlokasiValid(jadwal, d, idx)) {
            jadwal[idx].dokter_bertugas = d;

            if (solveJadwal(jadwal, idx + 1)) {
                return true;
            }

            // gagal, balik lagi
            jadwal[idx].dokter_bertugas = NULL;
        }
    }

    return false; // tidak ada yang cocok
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
    FILE *f = fopen(nama_file, "w");
    if (!f) {
        printf("Gagal buka file untuk tulis: %s\n", nama_file);
        return;
    }

    fprintf(f, "Hari,Shift,NamaDokter\n");

    for (int i = 0; i < TOTAL_SHIFT; i++) {
        const char* nama = "KOSONG";
        if (jadwal[i].dokter_bertugas != NULL) {
            nama = jadwal[i].dokter_bertugas->nama;
        }

        fprintf(f, "%d,%s,%s\n", jadwal[i].hari_ke + 1, jadwal[i].tipe_shift, nama);
    }

    fclose(f);
    printf("File %s berhasil disimpan.\n", nama_file);
}


int muatDataDokter(const char* path_file) {
    FILE *fp = fopen(path_file, "r");
    if (!fp) {
        printf("Gagal membuka file: %s\n", path_file);
        return -1;
    }

    char baris[MAX_LINE_LEN];
    fgets(baris, sizeof(baris), fp); // skip header

    int i = 0;
    while (fgets(baris, sizeof(baris), fp) && i < MAX_DOKTER) {
        baris[strcspn(baris, "\n")] = 0; // hapus newline

        // parsing csv (satu-satu aja)
        char *token = strtok(baris, ",");
        if (token) strcpy(daftar_dokter[i].nama, token);

        token = strtok(NULL, ",");
        if (token) daftar_dokter[i].maks_shift_mingguan = atoi(token);

        token = strtok(NULL, ",");
        if (token) daftar_dokter[i].pref_pagi = atoi(token);

        token = strtok(NULL, ",");
        if (token) daftar_dokter[i].pref_siang = atoi(token);

        token = strtok(NULL, ",");
        if (token) daftar_dokter[i].pref_malam = atoi(token);

        i++;
    }

    fclose(fp);
    return i;
}





// INI MAINNYA========================================
int main() {
    jumlah_dokter = muatDataDokter("../../data/sample/daftar_dokter.csv");

    if (jumlah_dokter == -1) {
        return 1;
    }
    printf("Data %d dokter berhasil dimuat.\n\n", jumlah_dokter);

    Shift jadwal[TOTAL_SHIFT];

    // Inisialisasi shift (manual per 3 shift per hari)
    for (int hari = 0, i = 0; hari < 30; hari++) {
        //  pagi
        jadwal[i].hari_ke = hari;
        strcpy(jadwal[i].tipe_shift, "pagi");
        jadwal[i].dokter_bertugas = NULL;
        i++;

        //  siang
        jadwal[i].hari_ke = hari;
        strcpy(jadwal[i].tipe_shift, "siang");
        jadwal[i].dokter_bertugas = NULL;
        i++;

        //  malam
        jadwal[i].hari_ke = hari;
        strcpy(jadwal[i].tipe_shift, "malam");
        jadwal[i].dokter_bertugas = NULL;
        i++;
    }

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












