/*
Program Pengelolaan Data Dokter
Fungsi:
1. Menampilkan menu manajemen dokter
2. Menambah dokter
3. Menghapus dokter
4. Menampilkan daftar dokter
5. Membaca dan load data dokter dari CSV
6. Menyimpan data dokter ke CSV
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "../include/types.h"
#include "../include/doctor_manager.h"
#include "../include/utils.h"

#define DOCTORS_FILE "data/sample/daftar_dokter.csv"
#define LOAD_DOCTORS_FILE "data/doctors.csv"

void doctorManagementMenu(Dokter daftar[], int *jumlah) {
    int pilihan;
    char nama[50];
    int maks_shift, pref_pagi, pref_siang, pref_malam;

    while(1) {
        clearScreen();
        printf("\n=== MANAJEMEN DATA DOKTER ===\n");
        printLine('-', 60);
        printf("[1] Tampilkan Daftar Dokter\n");
        printf("[2] Tambah Dokter Baru\n");
        printf("[3] Hapus Dokter\n");
        printf("[4] Muat Data Dokter dari File CSV\n");
        printf("[5] Simpan Data Dokter ke File CSV\n");
        printf("[0] Kembali ke Menu Utama\n");
        printLine('-', 60);
        printf("Jumlah dokter saat ini: %d\n", *jumlah);
        printLine('-', 60);

        pilihan = getIntInput(0, 5, "Masukkan pilihan Anda (0-5): ");

        switch(pilihan) {
            case 1: // Tampilkan daftar dokter
                clearScreen();
                printf("\n=== DAFTAR DOKTER ===\n");
                tampilkanDaftarDokter(daftar, *jumlah);
                printf("\nTekan Enter untuk kembali ke menu manajemen dokter...");
                getchar();
                break;

            case 2: // Tambah dokter baru
                clearScreen();
                printf("\n=== TAMBAH DOKTER BARU ===\n");
                printf("Masukkan data dokter baru:\n");
                printf("Nama dokter: ");
                if (fgets(nama, 50, stdin) == NULL) {
                    printf("Error membaca input\n");
                    break;
                }
                nama[strcspn(nama, "\n")] = 0;
                if (strlen(nama) == 0) {
                    printf("Nama tidak boleh kosong.\n");
                    printf("\nTekan Enter untuk melanjutkan...");
                    getchar();
                    break;
                }

                maks_shift = getIntInput(1, 21, "Maksimal shift per minggu (1-21): ");
                printLine('-', 40);
                printf("Preferensi shift (1 = Ya, 0 = Tidak):\n");
                pref_pagi = getIntInput(0, 1, "Shift pagi (1 = Ya, 0 = Tidak): ");
                pref_siang = getIntInput(0, 1, "Shift siang (1 = Ya, 0 = Tidak): ");
                pref_malam = getIntInput(0, 1, "Shift malam (1 = Ya, 0 = Tidak): ");

                // Validasi minimal satu preferensi shift
                if (pref_pagi == 0 && pref_siang == 0 && pref_malam == 0) {
                    printf("Error: Dokter harus memiliki minimal satu preferensi shift.\n");
                    printf("\nTekan Enter untuk melanjutkan...");
                    getchar();
                    break;
                }

                tambahDokter(daftar, jumlah, nama, maks_shift, pref_pagi, pref_siang, pref_malam);
                printf("\nTekan Enter untuk kembali ke menu manajemen dokter...");
                getchar();
                break;

            case 3: // Hapus dokter
                clearScreen();
                printf("\n=== HAPUS DOKTER ===\n");
                if (*jumlah == 0) {
                    printf("Tidak ada dokter untuk dihapus.\n");
                    printf("\nTekan Enter untuk kembali ke menu manajemen dokter...");
                    getchar();
                    break;
                }
                
                tampilkanDaftarDokter(daftar, *jumlah);
                printf("\nMasukkan nama dokter yang akan dihapus: ");
                if (fgets(nama, 50, stdin) == NULL) {
                    printf("Error membaca input\n");
                    break;
                }
                nama[strcspn(nama, "\n")] = 0;

                // Konfirmasi penghapusan
                if (confirmAction("Yakin ingin menghapus dokter ini")) {
                    hapusDokter(daftar, jumlah, nama);
                } else {
                    printf("Penghapusan dibatalkan.\n");
                }
                printf("\nTekan Enter untuk kembali ke menu manajemen dokter...");
                getchar();
                break;

            case 4: // Muat data dari CSV
                clearScreen();
                printf("\n=== MUAT DATA DARI CSV ===\n");
                printf("Data akan dimuat dari file: %s\n", LOAD_DOCTORS_FILE);
                if (confirmAction("Lanjutkan? Data dokter saat ini akan diganti")) {
                    startProgress("Memuat data dari CSV...");
                    if (bacaDokterDariCSV(LOAD_DOCTORS_FILE, daftar, jumlah)) {
                        endProgress("Berhasil memuat data dari CSV");
                    } else {
                        endProgress("Gagal memuat data dari CSV");
                    }
                } else {
                    printf("Operasi dibatalkan.\n");
                }
                printf("\nTekan Enter untuk kembali ke menu manajemen dokter...");
                getchar();
                break;

            case 5: // Simpan data ke CSV
                clearScreen();
                printf("\n=== SIMPAN DATA KE CSV ===\n");
                if (*jumlah == 0) {
                    printf("Tidak ada dokter untuk disimpan.\n");
                    printf("\nTekan Enter untuk kembali ke menu manajemen dokter...");
                    getchar();
                    break;
                }
                
                printf("Data akan disimpan ke file: %s\n", DOCTORS_FILE);
                if (confirmAction("Lanjutkan? File yang sudah ada akan ditimpa")) {
                    startProgress("Menyimpan data ke CSV...");
                    if (simpanDokterKeCSV(DOCTORS_FILE, daftar, *jumlah)) {
                        endProgress("Berhasil menyimpan data ke CSV");
                    } else {
                        endProgress("Gagal menyimpan data ke CSV");
                    }
                } else {
                    printf("Operasi dibatalkan.\n");
                }
                printf("\nTekan Enter untuk kembali ke menu manajemen dokter...");
                getchar();
                break;

            case 0: // Kembali ke menu utama
                return;

            default:
                printf("Pilihan tidak valid.\n");
                printf("\nTekan Enter untuk melanjutkan...");
                getchar();
        }
    }
}


void tampilkanDaftarDokter(Dokter daftar[], int jumlah) {
    if (jumlah == 0) { // Jika daftar dokter kosong
        printf("Belum ada data dokter.\n");
        return;
    }

    // Header tabel
    printf("\n======================================================================\n");
    printf("%-20s | %-10s | %-10s | %-10s | %-10s\n", "Nama", "Maks Shift", "Pagi", "Siang", "Malam");
    printf("======================================================================\n");

    for (int i = 0; i < jumlah; i++) { // Handle nama yang terlalu panjang (>20 karakter)
        char truncated_name[21];
        strncpy(truncated_name, daftar[i].nama, 20);
        truncated_name[20] = '\0';
        if (strlen(daftar[i].nama) > 20) {
            strcpy(truncated_name + 17, "...");
        }
        
        // Menampilkan data tiap dokter
        printf("%-20s | %-10d | %-10s | %-10s | %-10s\n",
               truncated_name,
               daftar[i].maks_shift_mingguan,
               daftar[i].pref_pagi ? "Ya" : "Tidak", // Convert dari 1/0 jadi Ya/Tidak
               daftar[i].pref_siang ? "Ya" : "Tidak",
               daftar[i].pref_malam ? "Ya" : "Tidak");
    }
    printf("======================================================================\n");
}

void tambahDokter(Dokter daftar[], int *jumlah, const char *nama, int maks_shift, int pref_pagi, int pref_siang, int pref_malam) {
    if (nama == NULL || strlen(nama) == 0) { // Cek apakah input nama kosong
        printf("Error: Nama dokter tidak boleh kosong.\n");
        return;
    }

    if (maks_shift < 1) { // Cek supaya shift minimal 1 (kurang dari itu, artinya dokter tidak bertugas)
        printf("Error: Jumlah shift minimal 1.\n");
        return;
    }

    if (*jumlah >= MAX_DOCTORS) { // Cek apakah jumlah dokter terdaftar sudah maksimum
        printf("Error: Kapasitas dokter penuh!\n");
        return;
    }

    for (int i = 0; i < *jumlah; i++) { // Cek apakah dokter baru sudah terdaftar
        if (strcmp(daftar[i].nama, nama) == 0) {
            printf("Error: Dokter dengan nama %s sudah ada.\n", nama);
            return;
        }
    }

    // Jika nama dokter valid
    strncpy(daftar[*jumlah].nama, nama, 49); // Salin, batasi sampai 49 karakter, akhiri dengan \0
    daftar[*jumlah].nama[49] = '\0'; 
    daftar[*jumlah].maks_shift_mingguan = maks_shift; // Set properti dokter
    daftar[*jumlah].pref_pagi = pref_pagi ? 1 : 0; // Semua input nonzero dinormalisasi ke 1
    daftar[*jumlah].pref_siang = pref_siang ? 1 : 0;
    daftar[*jumlah].pref_malam = pref_malam ? 1 : 0;

    (*jumlah)++; // Counter jumlah dokter terdaftar
    printf("Dokter %s berhasil ditambahkan.\n", nama);
}

void hapusDokter(Dokter daftar[], int *jumlah, const char *nama) {
    if (nama == NULL || strlen(nama) == 0) { // Cek apakah input nama kosong
        printf("Error: Nama dokter tidak boleh kosong.\n");
        return;
    }

    int found_index = -1;

    for (int i = 0; i < *jumlah; i++) { // Cari dokter berdasarkan nama
        if (strcmp(daftar[i].nama, nama) == 0) {
            found_index = i;
            break;
        }
    }

    if (found_index != -1) { // Jika dokter ditemukan
        for (int i = found_index; i < *jumlah - 1; i++) { // Geser semua dokter setelahnya ke kiri
            daftar[i] = daftar[i + 1];
        }
        (*jumlah)--; // Kurangi jumlah dokter terdaftar
        printf("Dokter %s berhasil dihapus.\n", nama);
    } else { // Jika dokter tidak ditemukan
        printf("Error: Dokter %s tidak ditemukan.\n", nama);
    }
}

int simpanDokterKeCSV(const char *filename, Dokter daftar[], int jumlah) {
    FILE *file = fopen(filename, "w"); // Buka file untuk ditulis (mode write)
    if (!file) { // Cek apakah file berhasil dibuka
        printf("Error: Gagal membuka file %s untuk disimpan.\n", filename);
        return 0;
    }

    // Tulis header CSV di baris pertama
    if (fprintf(file, "Nama,Maksimal Shift,Pref Pagi,Pref Siang,Pref Malam\n") < 0) { // Cek apakah berhasil menulis header
        printf("Error: Gagal menulis data ke file.\n");
        fclose(file);
        return 0;
    }

    int success = 1; // Flag untuk melacak apakah semua data berhasil ditulis
    for (int i = 0; i < jumlah; i++) { // Loop untuk menulis data setiap dokter
        if (fprintf(file, "%s,%d,%d,%d,%d\n",
                daftar[i].nama,
                daftar[i].maks_shift_mingguan,
                daftar[i].pref_pagi,
                daftar[i].pref_siang,
                daftar[i].pref_malam) < 0) { // Cek apakah berhasil menulis data dokter
            
            printf("Error: Gagal menulis data ke file.\n");
            success = 0; // Set flag gagal
            break; // Hentikan loop jika ada error
        }
    }

    fclose(file); // Tutup file setelah selesai

    if (success) { // Jika semua data berhasil disimpan
        printf("Data %d dokter berhasil disimpan ke %s\n", jumlah, filename);
    }

    return success; // Return 1 jika berhasil, 0 jika gagal
}

int bacaDokterDariCSV(const char *filename, Dokter daftar[], int *jumlah) {
    FILE *file = fopen(filename, "r");
    if (!file) { 
        printf("Error: File %s tidak ditemukan atau tidak bisa dibuka.\n", filename);
        return 0;
    }

    char line[256]; // Ukuran file CSV
    if (!fgets(line, sizeof(line), file)) { // Baca header lalu abaikan
        printf("Error: Format file CSV tidak valid (header tidak ditemukan).\n");
        fclose(file);
        return 0;
    }

    *jumlah = 0; // Reset counter
    int line_num = 1; // Lacak baris yang sedang dibaca

    // Baca file CSV baris per baris
    while (fgets(line, sizeof(line), file) && *jumlah < MAX_DOCTORS) {
        line_num++;
        line[strcspn(line, "\n")] = 0; // Hapus newline di akhir string

        if (strlen(trimString(line)) == 0) continue; // Lewati baris kosong 

        char *token = strtok(line, ","); // CSV parsing; ambil nama dokter
        if (!token) { 
            printf("Warning: Format data tidak valid pada baris %d, melewati...\n", line_num);
            continue;
        }

        strncpy(daftar[*jumlah].nama, token, 49); // Salin nama dokter (safety), akhiri \0
        daftar[*jumlah].nama[49] = '\0'; 

        token = strtok(NULL, ","); 
        if (!token) { // Handle data tidak lengkap (normalisasi ke 5 shift)
            printf("Warning: Data tidak lengkap pada baris %d, menggunakan nilai default.\n", line_num);
            daftar[*jumlah].maks_shift_mingguan = 5;
        } else {
            int temp_shift = atoi(token);
            if (temp_shift < 1) { // Handle data invalid (normalisasi ke 5 shift)
                printf("Warning: Maks shift tidak valid pada baris %d (%d), menggunakan nilai 5.\n", line_num, temp_shift);
                daftar[*jumlah].maks_shift_mingguan = 5;
            } else if (temp_shift > 21) { // Handle bacaan shift melebihi batas (normalisasi ke jml shift maksimum 21 shift)
                printf("Warning: Maks shift terlalu tinggi pada baris %d (%d), dinormalisasi ke 21.\n", line_num, temp_shift);
                daftar[*jumlah].maks_shift_mingguan = 21;
            } else { 
                daftar[*jumlah].maks_shift_mingguan = temp_shift;
            }
        }

        // Handle data invalid untuk preferensi pagi
        token = strtok(NULL, ",");
        if (!token) { 
            printf("Warning: Preferensi pagi tidak ada pada baris %d, menggunakan nilai 0.\n", line_num);
            daftar[*jumlah].pref_pagi = 0;
        } else {
            char *endptr;
            long pref_val = strtol(token, &endptr, 10);
            if (*endptr != '\0') {
                printf("Warning: Preferensi pagi tidak valid pada baris %d (%s), menggunakan nilai 0.\n", line_num, token);
                daftar[*jumlah].pref_pagi = 0;
            } else {
                daftar[*jumlah].pref_pagi = (pref_val > 0) ? 1 : 0;
            }
        }
        
        // Handle data invalid untuk preferensi siang
        token = strtok(NULL, ",");
        if (!token) {
            printf("Warning: Preferensi siang tidak ada pada baris %d, menggunakan nilai 0.\n", line_num);
            daftar[*jumlah].pref_siang = 0;
        } else {
            char *endptr;
            long pref_val = strtol(token, &endptr, 10);
            if (*endptr != '\0') {
                printf("Warning: Preferensi siang tidak valid pada baris %d (%s), menggunakan nilai 0.\n", line_num, token);
                daftar[*jumlah].pref_siang = 0;
            } else {
                daftar[*jumlah].pref_siang = (pref_val > 0) ? 1 : 0;
            }
        }
        
        // Handle data invalid untuk preferensi malam
        token = strtok(NULL, ",");
        if (!token) {
            printf("Warning: Preferensi malam tidak ada pada baris %d, menggunakan nilai 0.\n", line_num);
            daftar[*jumlah].pref_malam = 0;
        } else {
            char *endptr;
            long pref_val = strtol(token, &endptr, 10);
            if (*endptr != '\0') {
                printf("Warning: Preferensi malam tidak valid pada baris %d (%s), menggunakan nilai 0.\n", line_num, token);
                daftar[*jumlah].pref_malam = 0;
            } else {
                daftar[*jumlah].pref_malam = (pref_val > 0) ? 1 : 0;
            }
        }

        // Memastikan dokter terdaftar minimal punya satu preferensi (pagi/siang/malam). Kalau tidak ada yang untuk apa terdaftar
        if (daftar[*jumlah].pref_pagi == 0 && daftar[*jumlah].pref_siang == 0 && daftar[*jumlah].pref_malam == 0) {
            printf("Error: Dokter %s pada baris %d tidak memiliki preferensi shift sama sekali. Data diabaikan.\n", 
                   daftar[*jumlah].nama, line_num);
            continue; 
        }
        (*jumlah)++;
    }

    if (*jumlah == MAX_DOCTORS && !feof(file)) { // Cek jika jml dokter melebihi kapasitas RS (50). Hanya ambil 50 pertama
        printf("Warning: Jumlah dokter melebihi kapasitas. Hanya %d dokter pertama yang dimuat.\n", MAX_DOCTORS); 
    }

    fclose(file);
    return 1;
}