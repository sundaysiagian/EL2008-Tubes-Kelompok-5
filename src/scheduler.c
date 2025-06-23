#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "../include/types.h"
#include "../include/doctor_manager.h"
#include "../include/scheduler.h"
#include "../include/display.h"
#include "../include/utils.h"

#define DOCTORS_FILE "data/sample/daftar_dokter.csv"
#define SCHEDULE_FILE "data/sample/jadwal_dokter.csv"
#define LOAD_DOCTORS_FILE "data/doctors.csv"

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

void displayEnhancedMenu() {
    clearScreen();
    printf("\n=== RS SEMOGA SEHAT SELALU - SISTEM PENJADWALAN DOKTER ===\n\n");
    printf("[1] Manajemen Data Dokter\n");
    printf("[2] Buat Jadwal Otomatis\n");
    printf("[3] Tampilkan Jadwal Harian\n");
    printf("[4] Tampilkan Jadwal Mingguan\n");
    printf("[5] Tampilkan Jadwal Bulanan\n");
    printf("[6] Statistik dan Analisis Jadwal\n");
    printf("[7] Simpan Jadwal ke File CSV\n");
    printf("[8] Muat Jadwal dari File CSV\n");
    printf("[9] Edit Jadwal Secara Manual\n");
    printf("[10] Tampilkan Jadwal Per Dokter\n");
    printf("[11] Pencarian\n");
    printf("[0] Keluar dari Aplikasi\n\n");
    printLine('-', 60);
    printf("Masukkan pilihan Anda (0-11): ");
}

void cariDokter(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter) {
    char keyword[50];
    printf("Masukkan nama atau bagian nama dokter yang ingin dicari: ");
    if (fgets(keyword, 50, stdin) == NULL) {
        printf("Error membaca input\n");
        return;
    }
    keyword[strcspn(keyword, "\n")] = 0; // Hapus karakter newline dari input
    
    printf("\nHasil pencarian untuk '%s':\n", keyword);
    printLine('-', 50);
    
    int found = 0;
    // Loop semua dokter untuk mencari yang namanya mengandung keyword
    for (int i = 0; i < jumlah_dokter; i++) {
        if (strstr(daftar_dokter[i].nama, keyword) != NULL) {
            printf("Ditemukan dokter: %s\n", daftar_dokter[i].nama);
            found++;
            
            printf("Jadwal shift:\n");
            int shift_count = 0;
            
            // Cari semua shift yang dialokasikan untuk dokter ini
            for (int j = 0; j < TOTAL_SHIFT; j++) {
                for (int k = 0; k < jadwal[j].jumlah_dokter; k++) {
                    if (jadwal[j].dokter_bertugas[k] == &daftar_dokter[i]) {
                        printf("- Hari %d, Shift %s\n", 
                               jadwal[j].hari_ke + 1, 
                               jadwal[j].tipe_shift);
                        shift_count++;
                    }
                }
            }
            
            // Tampilkan pesan jika dokter tidak memiliki jadwal
            if (shift_count == 0) {
                printf("- Tidak ada shift terjadwal\n");
            }
            printf("\n");
        }
    }
    
    // Tampilkan ringkasan hasil pencarian
    if (found == 0) {
        printf("Tidak ditemukan dokter dengan nama mengandung '%s'\n", keyword);
    } else {
        printf("Ditemukan %d dokter\n", found);
    }
}

void cariTanggal(Shift jadwal[]) {
    int hari = getIntInput(1, 30, "Masukkan tanggal (1-30): ");
    hari -= 1; // Konversi ke indeks array (0-29)
    
    printf("\nJadwal untuk Hari %d:\n", hari + 1);
    printLine('-', 40);
    
    // Loop untuk tiap jenis shift dalam satu hari (pagi, siang, malam)
    for (int i = 0; i < 3; i++) {
        int idx = hari * 3 + i;
        
        printf("Shift %s: ", jadwal[idx].tipe_shift);
        if (jadwal[idx].jumlah_dokter == 0) {
            printf("Tidak ada dokter\n");
        } else {
            printf("\n");
            // Tampilkan semua dokter yang bertugas pada shift ini
            for (int j = 0; j < jadwal[idx].jumlah_dokter; j++) {
                printf("  %d. %s\n", j + 1, jadwal[idx].dokter_bertugas[j]->nama);
            }
        }
    }
}

void pencarian(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter) {
    while (1) {
        clearScreen();
        printf("\n=== MENU PENCARIAN ===\n");
        printf("1. Cari dokter\n");
        printf("2. Cari berdasarkan tanggal\n");
        printf("0. Kembali ke menu utama\n");
        printLine('-', 40);
        
        // Minta input pilihan menu dari user
        int pilihan = getIntInput(0, 2, "Pilihan Anda: ");
        
        // Switchcase pilihan menu
        switch (pilihan) {
            case 0:
                return; // Kembali ke menu utama
            case 1:
                cariDokter(jadwal, daftar_dokter, jumlah_dokter); // Pencarian berdasarkan nama dokter
                break;
            case 2:
                cariTanggal(jadwal); // Pencarian berdasarkan tanggal
                break;
        }
        
        // Pause singkat
        printf("\nTekan Enter untuk melanjutkan...");
        getchar();
    }
}

void editJadwalManual(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter) {
    int hari, shift_type, action;
    
    while (1) {
        clearScreen();
        printf("\n=== EDIT JADWAL MANUAL ===\n");
        printf("1. Tambah dokter ke shift\n");
        printf("2. Hapus dokter dari shift\n");
        printf("0. Kembali ke menu utama\n");
        printLine('-', 40);
        
        action = getIntInput(0, 2, "Pilih aksi: ");
        
        if (action == 0) return; // Kembali ke menu utama
        
        hari = getIntInput(1, 30, "Masukkan hari (1-30): ");
        
        printf("Pilih shift:\n");
        printf("1. Pagi\n");
        printf("2. Siang\n");
        printf("3. Malam\n");
        shift_type = getIntInput(1, 3, "Pilihan: ");
        
        // Hitung indeks shift berdasarkan hari dan jenis shift
        int idx = (hari - 1) * 3 + (shift_type - 1);
        
        // Tampilkan jadwal saat ini
        printf("\nJadwal saat ini untuk Hari %d Shift %s:\n", 
               hari, jadwal[idx].tipe_shift);
        
        if (jadwal[idx].jumlah_dokter == 0) {
            printf("Tidak ada dokter yang dijadwalkan.\n");
        } else {
            for (int i = 0; i < jadwal[idx].jumlah_dokter; i++) {
                printf("%d. %s\n", i + 1, jadwal[idx].dokter_bertugas[i]->nama);
            }
        }
        printf("\n");
        
        if (action == 1) {  // Tambah dokter ke shift
            if (jadwal[idx].jumlah_dokter >= MAX_DOCTORS_PER_SHIFT) {
                printf("Shift ini sudah penuh dengan %d dokter.\n", MAX_DOCTORS_PER_SHIFT);
            } else {
                printf("\nDaftar dokter yang tersedia:\n");
                tampilkanDaftarDokter(daftar_dokter, jumlah_dokter);
                
                int dok_idx = getIntInput(1, jumlah_dokter, "Pilih dokter (nomor): ") - 1;
                
                // Validasi alokasi dokter
                if (apakahAlokasiValid(jadwal, &daftar_dokter[dok_idx], idx)) {
                    jadwal[idx].dokter_bertugas[jadwal[idx].jumlah_dokter] = &daftar_dokter[dok_idx];
                    jadwal[idx].jumlah_dokter++;
                    printf("Dokter %s berhasil ditambahkan ke jadwal.\n", daftar_dokter[dok_idx].nama);
                } else {
                    printf("Error: Penambahan dokter tidak valid. Mungkin terjadi konflik jadwal atau preferensi.\n");
                }
            }
        } else if (action == 2) {  // Hapus dokter dari shift
            if (jadwal[idx].jumlah_dokter == 0) {
                printf("Tidak ada dokter untuk dihapus dari shift ini.\n");
            } else {
                int dok_idx = getIntInput(1, jadwal[idx].jumlah_dokter, "Pilih dokter untuk dihapus (nomor): ") - 1;
                
                // Geser data dokter setelah yang dihapus
                for (int i = dok_idx; i < jadwal[idx].jumlah_dokter - 1; i++) {
                    jadwal[idx].dokter_bertugas[i] = jadwal[idx].dokter_bertugas[i + 1];
                }
                jadwal[idx].jumlah_dokter--;
                printf("Dokter berhasil dihapus dari jadwal.\n");
            }
        }
        
        printf("\nTekan Enter untuk melanjutkan...");
        getchar();
    }
}

int bacaJadwalDariCSV(Shift jadwal[], const char* nama_file, Dokter daftar_dokter[], int jumlah_dokter) {
    FILE *f = fopen(nama_file, "r");
    if (!f) {
        printf("Error: File %s tidak ditemukan atau tidak dapat dibuka.\n", nama_file);
        return 0;
    }

    inisialisasiJadwal(jadwal); // Reset jadwal yang ada
    
    char line[256];
    if (!fgets(line, sizeof(line), f)) { // Baca baris header dan cek apakah file kosong
        printf("Error: Format file tidak valid.\n");
        fclose(f);
        return 0;
    }
    
    int success = 1;
    int line_count = 1;
    
    // Proses file baris per baris
    while (fgets(line, sizeof(line), f)) {
        line_count++;
        line[strcspn(line, "\n")] = 0; // Hapus karakter newline
        
        if (strlen(line) == 0) continue; // Lewati baris kosong
        
        char *token;
        char *rest = line;
        
        // Parsing kolom hari
        token = strtok(rest, ",");
        if (!token) {
            printf("Warning: Format tidak valid pada baris %d\n", line_count);
            continue;
        }
        int hari = atoi(token) - 1; // Konversi ke indeks array (0-29)
        
        // Parsing kolom tipe shift
        token = strtok(rest, ",");
        if (!token) {
            printf("Warning: Format tidak valid pada baris %d\n", line_count);
            continue;
        }
        char shift_type[20];
        strncpy(shift_type, token, 19);
        shift_type[19] = '\0';
        
        // Konversi tipe shift ke indeks (0=pagi, 1=siang, 2=malam)
        int shift_index;
        if (strcmp(shift_type, "pagi") == 0 || strcmp(shift_type, "Pagi") == 0) {
            shift_index = 0;
        } else if (strcmp(shift_type, "siang") == 0 || strcmp(shift_type, "Siang") == 0) {
            shift_index = 1;
        } else if (strcmp(shift_type, "malam") == 0 || strcmp(shift_type, "Malam") == 0) {
            shift_index = 2;
        } else {
            printf("Warning: Tipe shift tidak valid pada baris %d: %s\n", line_count, shift_type);
            continue;
        }
        
        // Hitung indeks shift dalam array jadwal
        int idx = hari * 3 + shift_index;
        
        // Skip kolom-kolom yang tidak digunakan
        token = strtok(rest, ",");
        if (!token) {
            printf("Warning: Format tidak valid pada baris %d\n", line_count);
            continue;
        }
        
        // Ambil nama dokter dari kolom terakhir
        token = strtok(rest, ",");
        if (!token) {
            printf("Warning: Format tidak valid pada baris %d\n", line_count);
            continue;
        }
        
        // Cari dokter dalam daftar berdasarkan nama
        Dokter* dokter_ptr = NULL;
        for (int i = 0; i < jumlah_dokter; i++) {
            if (strcmp(daftar_dokter[i].nama, token) == 0) {
                dokter_ptr = &daftar_dokter[i];
                break;
            }
        }
        
        // Tambahkan dokter ke jadwal jika ditemukan dan masih ada slot
        if (dokter_ptr && jadwal[idx].jumlah_dokter < MAX_DOCTORS_PER_SHIFT) {
            jadwal[idx].dokter_bertugas[jadwal[idx].jumlah_dokter] = dokter_ptr;
            jadwal[idx].jumlah_dokter++;
        } else if (!dokter_ptr) {
            printf("Warning: Dokter '%s' tidak ditemukan, diabaikan\n", token);
        }
    }
    
    fclose(f);
    return success;
}

bool apakahAlokasiValid(Shift jadwal[], Dokter* dokter, int index_shift) {
    int hari_ini = index_shift / 3;
    int shift_dalam_hari = index_shift % 3; // 0=pagi, 1=siang, 2=malam
    
    // Cek apakah shift sudah penuh
    if (jadwal[index_shift].jumlah_dokter >= MAX_DOCTORS_PER_SHIFT) {
        return false;
    }

    // Cek apakah dokter sudah ada di shift ini
    for (int i = 0; i < jadwal[index_shift].jumlah_dokter; i++) {
        if (jadwal[index_shift].dokter_bertugas[i] == dokter) {
            return false;
        }
    }

    // Cek apakah dokter sudah ada di shift lain di hari yang sama
    int awal_shift_hari_ini = hari_ini * 3;
    for (int i = awal_shift_hari_ini; i < awal_shift_hari_ini + 3; i++) {
        if (i != index_shift) {
            for (int j = 0; j < jadwal[i].jumlah_dokter; j++) {
                if (jadwal[i].dokter_bertugas[j] == dokter) {
                    return false;
                }
            }
        }
    }

    // CONSTRAINT 1 & 2: Cek jumlah shift dan shift malam per minggu
    int minggu_ini = hari_ini / 7;
    int jumlah_shift_minggu_ini = 0;
    int jumlah_shift_malam_minggu_ini = 0;
    
    int awal_minggu = minggu_ini * 7 * 3; // Indeks shift awal minggu
    int akhir_minggu = awal_minggu + 7 * 3; // Indeks shift akhir minggu
    
    if (akhir_minggu > TOTAL_SHIFT) {
        akhir_minggu = TOTAL_SHIFT;
    }
    
    for (int i = awal_minggu; i < akhir_minggu; i++) {
        for (int j = 0; j < jadwal[i].jumlah_dokter; j++) {
            if (jadwal[i].dokter_bertugas[j] == dokter) {
                jumlah_shift_minggu_ini++;
                
                // Cek apakah ini shift malam
                if (i % 3 == 2) { // indeks % 3 == 2 berarti shift malam
                    jumlah_shift_malam_minggu_ini++;
                }
            }
        }
    }
    
    // Tambahkan shift yang sedang diperiksa jika itu shift malam
    if (shift_dalam_hari == 2) { // 2 = malam
        jumlah_shift_malam_minggu_ini++;
    }
    
    // Constraint: Maksimum 3 kali shift malam per minggu
    if (jumlah_shift_malam_minggu_ini > 3) {
        return false;
    }
    
    // Constraint: Maksimum 5 shift per minggu
    if (jumlah_shift_minggu_ini >= 5) {
        return false;
    }
    
    // CONSTRAINT 3: Dilarang bekerja dua shift secara beruntun tanpa istirahat
    // Cek shift sebelumnya (hari sebelumnya malam, dan kemudian pagi)
    if (shift_dalam_hari == 0 && hari_ini > 0) { // Jika ini shift pagi dan bukan hari pertama
        int shift_malam_kemarin = (hari_ini - 1) * 3 + 2; // Shift malam hari sebelumnya
        
        for (int j = 0; j < jadwal[shift_malam_kemarin].jumlah_dokter; j++) {
            if (jadwal[shift_malam_kemarin].dokter_bertugas[j] == dokter) {
                return false; // Dokter tidak boleh shift pagi setelah shift malam hari sebelumnya
            }
        }
    }
    
    // Cek shift berikutnya (jika shift malam, kemudian pagi hari berikutnya)
    if (shift_dalam_hari == 2 && hari_ini < 29) { // Jika ini shift malam dan bukan hari terakhir
        int shift_pagi_besok = (hari_ini + 1) * 3; // Shift pagi hari berikutnya
        
        for (int j = 0; j < jadwal[shift_pagi_besok].jumlah_dokter; j++) {
            if (jadwal[shift_pagi_besok].dokter_bertugas[j] == dokter) {
                return false; // Dokter tidak boleh shift malam sebelum shift pagi hari berikutnya
            }
        }
    }
    
    // Cek preferensi shift dokter
    const char *shift_type = jadwal[index_shift].tipe_shift;
    if ((strcmp(shift_type, "pagi") == 0 || strcmp(shift_type, "Pagi") == 0) && dokter->pref_pagi == 0) {
        return false;
    }
    if ((strcmp(shift_type, "siang") == 0 || strcmp(shift_type, "Siang") == 0) && dokter->pref_siang == 0) {
        return false;
    }
    if ((strcmp(shift_type, "malam") == 0 || strcmp(shift_type, "Malam") == 0) && dokter->pref_malam == 0) {
        return false;
    }
    
    return true;
}

// Fungsi untuk menampilkan dan mengelola menu manajemen data dokter
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
                    printColored("Nama tidak boleh kosong.\n");
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
                    printColored("Error: Dokter harus memiliki minimal satu preferensi shift.\n");
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
                    printColored("Penghapusan dibatalkan.\n");
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
                printColored("Pilihan tidak valid.\n");
                printf("\nTekan Enter untuk melanjutkan...");
                getchar();
        }
    }
}

// Fungsi untuk menampilkan jadwal dokter dalam format kalender bulanan
void tampilkanJadwalKalender(Shift jadwal[]) {
    printf("       JADWAL DOKTER JAGA - KALENDER BULANAN\n\n");
    printf(" Min  Sen  Sel  Rab  Kam  Jum  Sab\n");
    printLine('-', 50);
    
    int start_day = 1; // Hari pertama bulan (0=Minggu, 1=Senin, dst)
    
    // Tambahkan spasi untuk posisi hari pertama
    for (int i = 0; i < start_day; i++) {
        printf("     ");
    }
    
    // Loop untuk semua hari dalam bulan
    for (int day = 1; day <= 30; day++) {
        int day_of_week = (start_day + day - 1) % 7;
        
        printf("%3d  ", day);
        
        // Jika hari terakhir minggu atau hari terakhir bulan
        if (day_of_week == 6 || day == 30) {
            printf("\n");
            
            int start_of_week = day - day_of_week;
            
            // Spasi di awal minggu pertama
            for (int j = 0; j < start_day && start_of_week == 1; j++) {
                printf("     ");  
            }
            
            // Tampilkan status shift pagi untuk satu minggu
            for (int j = start_of_week; j <= day; j++) {
                if (j > 0 && j <= 30) {
                    int base_idx = (j-1) * 3;
                    char p = (jadwal[base_idx].jumlah_dokter > 0) ? 'P' : '-';
                    printf("  %c  ", p);
                }
            }
            printf("\n");
            
            // Spasi di awal minggu pertama
            for (int j = 0; j < start_day && start_of_week == 1; j++) {
                printf("     ");
            }
            
            // Tampilkan status shift siang untuk satu minggu
            for (int j = start_of_week; j <= day; j++) {
                if (j > 0 && j <= 30) {
                    int base_idx = (j-1) * 3 + 1;
                    char s = (jadwal[base_idx].jumlah_dokter > 0) ? 'S' : '-';
                    printf("  %c  ", s);
                }
            }
            printf("\n");
            
            // Spasi di awal minggu pertama
            for (int j = 0; j < start_day && start_of_week == 1; j++) {
                printf("     ");
            }
            
            // Tampilkan status shift malam untuk satu minggu
            for (int j = start_of_week; j <= day; j++) {
                if (j > 0 && j <= 30) {
                    int base_idx = (j-1) * 3 + 2;
                    char m = (jadwal[base_idx].jumlah_dokter > 0) ? 'M' : '-';
                    printf("  %c  ", m);
                }
            }
            printf("\n");
            printLine('-', 50);
        }
    }
    
    // Tampilkan keterangan
    printf("\nP = Pagi, S = Siang, M = Malam\n");
    printf("- = Tidak ada dokter terjadwal\n");
}

void tampilkanJadwalPerDokter(Shift jadwal[], Dokter *dokter) {
    printf("Jadwal untuk Dr. %s:\n\n", dokter->nama);
    printf("%-10s %-10s\n", "Tanggal", "Shift");
    printLine('-', 25);
    
    int count = 0;
    // Loop semua shift untuk menemukan yang dialokasikan ke dokter ini
    for (int i = 0; i < TOTAL_SHIFT; i++) {
        for (int j = 0; j < jadwal[i].jumlah_dokter; j++) {
            if (jadwal[i].dokter_bertugas[j] == dokter) {
                printf("Hari %-6d %-10s\n", 
                       jadwal[i].hari_ke + 1, 
                       jadwal[i].tipe_shift);
                count++;
            }
        }
    }
    
    // Tampilkan pesan jika dokter tidak memiliki jadwal
    if (count == 0) {
        printf("Dokter ini tidak memiliki jadwal shift.\n");
    } else {
        printf("\nTotal: %d shift\n", count);
    }
}

// Fungsi untuk navigasi antar jadwal harian dengan tombol kiri-kanan
void navigasiJadwal(Shift jadwal[]) {
    int current_day = 0;
    char input;
    
    while (1) {
        clearScreen();
        tampilkanJadwalHarian(jadwal, current_day);
        printf("\n[<] Hari sebelumnya  [>] Hari berikutnya  [q] Keluar\n");
        
        input = getchar();
        if (input == 'q') break; // Keluar dari navigasi
        else if (input == '<' && current_day > 0) current_day--; // Pindah ke hari sebelumnya
        else if (input == '>' && current_day < 29) current_day++; // Pindah ke hari berikutnya
    }
}

void tampilkanGrafikDistribusi(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter) {
    printf("Distribusi Shift (█ = 1 shift):\n\n");
    
    for (int i = 0; i < jumlah_dokter; i++) {
        int shifts = hitungJumlahShiftDokter(jadwal, &daftar_dokter[i]);
        printf("%-20s |", daftar_dokter[i].nama);
        
        // Tampilkan distribusi dengan karakter blok
        for (int j = 0; j < shifts; j++) {
            printf("█");
        }
        printf(" (%d)\n", shifts);
    }
}

void statistikJadwalMenu(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter) {
    while (1) {
        clearScreen();
        printf("\n=== MENU STATISTIK DAN ANALISIS ===\n");
        printf("1. Tampilkan jumlah shift per dokter\n");
        printf("2. Tampilkan distribusi shift\n");
        printf("3. Analisis keadilan distribusi\n");
        printf("4. Tampilkan pelanggaran preferensi\n");
        printf("5. Validasi jadwal (komprehensif)\n");
        printf("0. Kembali ke menu utama\n");
        printLine('-', 40);
        
        int pilihan = getIntInput(0, 5, "Pilihan Anda: ");
        
        // Proses pilihan menu
        switch (pilihan) {
            case 0:
                return; // Kembali ke menu utama
            case 1:
                clearScreen();
                tampilkanJumlahShiftDokter(jadwal, daftar_dokter, jumlah_dokter);
                break;
            case 2:
                clearScreen();
                tampilkanDistribusiShift(jadwal, daftar_dokter, jumlah_dokter);
                break;
            case 3:
                clearScreen();
                analisisKeadilan(jadwal, daftar_dokter, jumlah_dokter);
                break;
            case 4:
                clearScreen();
                tampilkanPelanggaranPreferensiPerDokter(jadwal, daftar_dokter, jumlah_dokter);
                break;
            case 5:
                clearScreen();
                validasiJadwal(jadwal, daftar_dokter, jumlah_dokter);
                break;
        }
        
        printf("\nTekan Enter untuk melanjutkan...");
        getchar();
    }
}

void inisialisasiJadwal(Shift jadwal[]) {
    for (int hari = 0, i = 0; hari < 30; hari++) {
        // Inisialisasi shift pagi
        jadwal[i].hari_ke = hari;
        strcpy(jadwal[i].tipe_shift, "pagi");
        jadwal[i].jumlah_dokter = 0;
        for (int j = 0; j < MAX_DOCTORS_PER_SHIFT; j++) {
            jadwal[i].dokter_bertugas[j] = NULL;
        }
        i++;

        // Inisialisasi shift siang
        jadwal[i].hari_ke = hari;
        strcpy(jadwal[i].tipe_shift, "siang");
        jadwal[i].jumlah_dokter = 0;
        for (int j = 0; j < MAX_DOCTORS_PER_SHIFT; j++) {
            jadwal[i].dokter_bertugas[j] = NULL;
        }
        i++;

        // Inisialisasi shift malam
        jadwal[i].hari_ke = hari;
        strcpy(jadwal[i].tipe_shift, "malam");
        jadwal[i].jumlah_dokter = 0;
        for (int j = 0; j < MAX_DOCTORS_PER_SHIFT; j++) {
            jadwal[i].dokter_bertugas[j] = NULL;
        }
        i++;
    }
}

// Fungsi rekursif dengan backtracking untuk mengisi jadwal shift dokter
bool solveJadwal(Shift jadwal[], int idx, int doctors_needed_per_shift, Dokter dokter[], int jumlah_dokter) {
    // Base case - semua shift sudah terisi
    if (idx >= TOTAL_SHIFT) {
        return true;
    }
    
    // Jika shift sudah memiliki cukup dokter, lanjut ke shift berikutnya
    if (jadwal[idx].jumlah_dokter >= doctors_needed_per_shift || 
        jadwal[idx].jumlah_dokter >= MAX_DOCTORS_PER_SHIFT) {
        return solveJadwal(jadwal, idx + 1, doctors_needed_per_shift, dokter, jumlah_dokter);
    }
    
    // Coba alokasikan dokter ke shift saat ini
    for (int i = 0; i < jumlah_dokter; i++) {
        if (apakahAlokasiValid(jadwal, &dokter[i], idx)) {
            jadwal[idx].dokter_bertugas[jadwal[idx].jumlah_dokter] = &dokter[i];
            jadwal[idx].jumlah_dokter++;
            
            // Jika minimal satu dokter dan berhasil lanjut ke shift berikutnya
            if ((jadwal[idx].jumlah_dokter >= 1) && 
                solveJadwal(jadwal, (jadwal[idx].jumlah_dokter >= doctors_needed_per_shift) ? idx + 1 : idx,
                           doctors_needed_per_shift, dokter, jumlah_dokter)) {
                return true;
            }
            
            // Backtracking - batalkan alokasi dokter
            jadwal[idx].jumlah_dokter--;
            jadwal[idx].dokter_bertugas[jadwal[idx].jumlah_dokter] = NULL;
        }
    }
    
    // Jika shift kosong, penjadwalan gagal
    if (jadwal[idx].jumlah_dokter == 0) {
        return false;
    }
    
    // Jika shift memiliki dokter tapi kurang dari target, tampilkan peringatan
    if (jadwal[idx].jumlah_dokter < doctors_needed_per_shift) {
        printf("Shift %s hari %d hanya terisi %d dari %d dokter yang dibutuhkan.\n", 
               jadwal[idx].tipe_shift, jadwal[idx].hari_ke + 1, 
               jadwal[idx].jumlah_dokter, doctors_needed_per_shift);
        return solveJadwal(jadwal, idx + 1, doctors_needed_per_shift, dokter, jumlah_dokter);
    }
    
    return false;
}

// Fungsi untuk membuat jadwal dokter secara otomatis
int buatJadwalOtomatis(Shift jadwal[], Dokter dokter[], int jumlah_dokter) {
    inisialisasiJadwal(jadwal);

    // Validasi jumlah dokter
    if (jumlah_dokter < 1) {
        printf("Error: Jumlah dokter (%d) tidak mencukupi untuk membuat jadwal.\n", jumlah_dokter);
        return 0;
    }

    // Minta input jumlah dokter per shift
    int max_possible = (jumlah_dokter > MAX_DOCTORS_PER_SHIFT) ? MAX_DOCTORS_PER_SHIFT : jumlah_dokter;
    char prompt_buffer[100];
    sprintf(prompt_buffer, "Masukkan jumlah dokter yang dibutuhkan per shift (1-%d): ", max_possible);
    int doctors_needed_per_shift = getIntInput(1, max_possible, prompt_buffer);

    // Analisis preferensi dokter
    printf("\nAnalisis Preferensi Dokter:\n");
    int morning_only = 0, afternoon_only = 0, night_only = 0;
    int morning_capable = 0, afternoon_capable = 0, night_capable = 0;
    
    for (int i = 0; i < jumlah_dokter; i++) {
        if (dokter[i].pref_pagi) morning_capable++;
        if (dokter[i].pref_siang) afternoon_capable++;
        if (dokter[i].pref_malam) night_capable++;
        
        if (dokter[i].pref_pagi && !dokter[i].pref_siang && !dokter[i].pref_malam) morning_only++;
        if (!dokter[i].pref_pagi && dokter[i].pref_siang && !dokter[i].pref_malam) afternoon_only++;
        if (!dokter[i].pref_pagi && !dokter[i].pref_siang && dokter[i].pref_malam) night_only++;
    }
    
    // Tampilkan hasil analisis
    printf("- Dokter yang bisa shift pagi: %d\n", morning_capable);
    printf("- Dokter yang bisa shift siang: %d\n", afternoon_capable);
    printf("- Dokter yang bisa shift malam: %d\n", night_capable);
    printf("- Dokter khusus pagi: %d\n", morning_only);
    printf("- Dokter khusus siang: %d\n", afternoon_only);
    printf("- Dokter khusus malam: %d\n", night_only);
    
    // Periksa kecukupan dokter
    if (morning_capable < doctors_needed_per_shift || 
        afternoon_capable < doctors_needed_per_shift || 
        night_capable < doctors_needed_per_shift) {
        printf("PERINGATAN: Ada shift yang tidak memiliki cukup dokter sesuai preferensi!\n");
    }

    // Proses pembuatan jadwal
    printf("Membuat jadwal dengan minimal %d dokter per shift...\n", doctors_needed_per_shift);
    startProgress("Menyusun jadwal...");
    
    if (solveJadwal(jadwal, 0, doctors_needed_per_shift, dokter, jumlah_dokter)) {
        endProgress("Jadwal berhasil dibuat!");
        
        // Evaluasi hasil penjadwalan
        int shifts_with_insufficient = 0;
        for (int i = 0; i < TOTAL_SHIFT; i++) {
            if (jadwal[i].jumlah_dokter < doctors_needed_per_shift) {
                shifts_with_insufficient++;
            }
        }
        
        if (shifts_with_insufficient > 0) {
            printf("PERINGATAN: %d shift tidak mencapai target %d dokter per shift\n", 
                   shifts_with_insufficient, doctors_needed_per_shift);
        } else {
            printf("BERHASIL: Semua shift memiliki minimal %d dokter\n", doctors_needed_per_shift);
        }
        
        return 1;
    } else {
        endProgress("Gagal membuat jadwal!");
        printf("Tidak ada solusi yang memungkinkan dengan batasan yang ada.\n");
        printf("Coba kurangi batasan atau tambah jumlah dokter.\n");
        return 0;
    }
}