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

#define DOCTORS_FILE "data/doctors.csv"
#define SCHEDULE_FILE "data/schedule.csv"

int getIntInput(int min, int max, const char* prompt) {
    int value;
    char input[100];

    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input. Please try again.\n");
            continue;
        }

        input[strcspn(input, "\n")] = 0;

        if (strlen(input) == 0) {
            printf("Input tidak boleh kosong. Silakan coba lagi.\n");
            continue;
        }

        char *endptr;
        value = strtol(input, &endptr, 10);

        if (*endptr != '\0') {
            printf("Input harus berupa angka. Silakan coba lagi.\n");
            continue;
        }

        if (value < min || value > max) {
            printf("Input harus antara %d dan %d. Silakan coba lagi.\n", min, max);
            continue;
        }

        return value;
    }
}

void displayMenu() {
    printf("\n=== RS SEMOGA SEHAT SELALU - SISTEM PENJADWALAN DOKTER ===\n");
    printLine('-', 60);
    printf("1. Manajemen Data Dokter\n");
    printf("2. Buat Jadwal Otomatis\n");
    printf("3. Tampilkan Jadwal Harian\n");
    printf("4. Tampilkan Jadwal Mingguan\n");
    printf("5. Tampilkan Jadwal Bulanan\n");
    printf("6. Statistik Jadwal\n");
    printf("7. Simpan Jadwal ke File\n");
    printf("0. Keluar\n");
    printLine('-', 60);
    printf("Pilihan Anda: ");
}

void displayEnhancedMenu() {
    clearScreen();
    printf("\n=== RS SEMOGA SEHAT SELALU - SISTEM PENJADWALAN DOKTER ===\n\n");
    printf("[1] Manajemen Data Dokter\n");
    printf("    - Tambah, hapus, dan lihat dokter\n");
    printf("    - Impor dan ekspor data dokter dari/ke CSV\n\n");
    printf("[2] Buat Jadwal Otomatis\n");
    printf("    - Buat jadwal 30 hari dengan 3 shift per hari\n\n");
    printf("[3] Tampilkan Jadwal Harian\n");
    printf("[4] Tampilkan Jadwal Mingguan\n");
    printf("[5] Tampilkan Jadwal Bulanan\n\n");
    printf("[6] Statistik dan Analisis Jadwal\n");
    printf("    - Lihat jumlah shift per dokter\n");
    printf("    - Analisis distribusi dan keadilan jadwal\n");
    printf("    - Lihat pelanggaran preferensi per dokter\n\n");
    printf("[7] Simpan Jadwal ke File CSV\n");
    printf("[8] Muat Jadwal dari File CSV\n");
    printf("[9] Edit Jadwal Secara Manual\n");
    printf("[10] Tampilkan Jadwal Per Dokter\n");
    printf("[11] Pencarian\n\n");
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
    keyword[strcspn(keyword, "\n")] = 0;
    
    printf("\nHasil pencarian untuk '%s':\n", keyword);
    printLine('-', 50);
    
    int found = 0;
    for (int i = 0; i < jumlah_dokter; i++) {
        if (strstr(daftar_dokter[i].nama, keyword) != NULL) {
            printf("Ditemukan dokter: %s\n", daftar_dokter[i].nama);
            found++;
            
            // List shifts for this doctor
            printf("Jadwal shift:\n");
            int shift_count = 0;
            
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
            
            if (shift_count == 0) {
                printf("- Tidak ada shift terjadwal\n");
            }
            printf("\n");
        }
    }
    
    if (found == 0) {
        printf("Tidak ditemukan dokter dengan nama mengandung '%s'\n", keyword);
    } else {
        printf("Ditemukan %d dokter\n", found);
    }
}

void cariTanggal(Shift jadwal[]) {
    int hari = getIntInput(1, 30, "Masukkan tanggal (1-30): ");
    hari -= 1; // Convert to 0-based
    
    printf("\nJadwal untuk Hari %d:\n", hari + 1);
    printLine('-', 40);
    
    for (int i = 0; i < 3; i++) {
        int idx = hari * 3 + i;
        
        printf("Shift %s: ", jadwal[idx].tipe_shift);
        if (jadwal[idx].jumlah_dokter == 0) {
            printf("Tidak ada dokter\n");
        } else {
            printf("\n");
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
        
        int pilihan = getIntInput(0, 2, "Pilihan Anda: ");
        
        switch (pilihan) {
            case 0:
                return;
            case 1:
                cariDokter(jadwal, daftar_dokter, jumlah_dokter);
                break;
            case 2:
                cariTanggal(jadwal);
                break;
        }
        
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
        
        if (action == 0) return;
        
        hari = getIntInput(1, 30, "Masukkan hari (1-30): ");
        
        printf("Pilih shift:\n");
        printf("1. Pagi\n");
        printf("2. Siang\n");
        printf("3. Malam\n");
        shift_type = getIntInput(1, 3, "Pilihan: ");
        
        // Calculate shift index
        int idx = (hari - 1) * 3 + (shift_type - 1);
        
        // Display current schedule for this shift
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
        
        if (action == 1) {  // Add doctor to shift
            if (jadwal[idx].jumlah_dokter >= MAX_DOCTORS_PER_SHIFT) {
                printf("Shift ini sudah penuh dengan %d dokter.\n", MAX_DOCTORS_PER_SHIFT);
            } else {
                // Show available doctors
                printf("\nDaftar dokter yang tersedia:\n");
                tampilkanDaftarDokter(daftar_dokter, jumlah_dokter);
                
                int dok_idx = getIntInput(1, jumlah_dokter, "Pilih dokter (nomor): ") - 1;
                
                // Check if allocation is valid
                if (apakahAlokasiValid(jadwal, &daftar_dokter[dok_idx], idx)) {
                    // Add doctor to shift
                    jadwal[idx].dokter_bertugas[jadwal[idx].jumlah_dokter] = &daftar_dokter[dok_idx];
                    jadwal[idx].jumlah_dokter++;
                    printf("Dokter %s berhasil ditambahkan ke jadwal.\n", daftar_dokter[dok_idx].nama);
                } else {
                    printf("Error: Penambahan dokter tidak valid. Mungkin terjadi konflik jadwal atau preferensi.\n");
                }
            }
        } else if (action == 2) {  // Remove doctor from shift
            if (jadwal[idx].jumlah_dokter == 0) {
                printf("Tidak ada dokter untuk dihapus dari shift ini.\n");
            } else {
                int dok_idx = getIntInput(1, jadwal[idx].jumlah_dokter, "Pilih dokter untuk dihapus (nomor): ") - 1;
                
                // Remove the doctor
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

    // Initialize empty schedule
    inisialisasiJadwal(jadwal);
    
    // Read and skip header line
    char line[256];
    if (!fgets(line, sizeof(line), f)) {
        printf("Error: Format file tidak valid.\n");
        fclose(f);
        return 0;
    }
    
    // Read schedule data
    int success = 1;
    int line_count = 1;
    
    while (fgets(line, sizeof(line), f)) {
        line_count++;
        line[strcspn(line, "\n")] = 0; // Remove newline
        
        // Skip empty lines
        if (strlen(line) == 0) continue;
        
        // Parse CSV line: Hari,Shift,DoctorPosition,NamaDokter
        char *token;
        char *rest = line;
        
        // Get day
        token = strtok_r(rest, ",", &rest);
        if (!token) {
            printf("Warning: Format tidak valid pada baris %d\n", line_count);
            continue;
        }
        int hari = atoi(token) - 1; // Convert to 0-based index
        
        // Get shift type
        token = strtok_r(rest, ",", &rest);
        if (!token) {
            printf("Warning: Format tidak valid pada baris %d\n", line_count);
            continue;
        }
        char shift_type[20];
        strncpy(shift_type, token, 19);
        shift_type[19] = '\0';
        
        // Calculate shift index
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
        
        // Calculate absolute shift index
        int idx = hari * 3 + shift_index;
        
        // Skip doctor position, we don't need it
        token = strtok_r(rest, ",", &rest);
        if (!token) {
            printf("Warning: Format tidak valid pada baris %d\n", line_count);
            continue;
        }
        
        // Get doctor name
        token = strtok_r(rest, ",", &rest);
        if (!token) {
            printf("Warning: Format tidak valid pada baris %d\n", line_count);
            continue;
        }
        
        // Find doctor in the list
        Dokter* dokter_ptr = NULL;
        for (int i = 0; i < jumlah_dokter; i++) {
            if (strcmp(daftar_dokter[i].nama, token) == 0) {
                dokter_ptr = &daftar_dokter[i];
                break;
            }
        }
        
        // If doctor found, add to schedule
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
    
    // Check if the shift is already fully staffed
    if (jadwal[index_shift].jumlah_dokter >= MAX_DOCTORS_PER_SHIFT) {
        return false;
    }

    // Check if doctor is already assigned to this shift
    for (int i = 0; i < jadwal[index_shift].jumlah_dokter; i++) {
        if (jadwal[index_shift].dokter_bertugas[i] == dokter) {
            return false;
        }
    }

    // Check if doctor is already working another shift on the same day
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

    // Check weekly shift limit
    int minggu_ini = hari_ini / 7;
    int jumlah_shift_minggu_ini = hitungShiftMingguIni(jadwal, dokter, minggu_ini);

    if (jumlah_shift_minggu_ini >= dokter->maks_shift_mingguan) {
        return false;
    }
    
    // Check doctor's preference
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
            case 1:
                clearScreen();
                printf("\n=== DAFTAR DOKTER ===\n");
                tampilkanDaftarDokter(daftar, *jumlah);
                printf("\nTekan Enter untuk kembali ke menu manajemen dokter...");
                getchar();
                break;

            case 2:
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
                printf("Preferensi shift (1=ya, 0=tidak):\n");
                pref_pagi = getIntInput(0, 1, "Shift pagi (1=ya, 0=tidak): ");
                pref_siang = getIntInput(0, 1, "Shift siang (1=ya, 0=tidak): ");
                pref_malam = getIntInput(0, 1, "Shift malam (1=ya, 0=tidak): ");

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

            case 3:
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

                if (confirmAction("Yakin ingin menghapus dokter ini")) {
                    hapusDokter(daftar, jumlah, nama);
                } else {
                    printColored("Penghapusan dibatalkan.\n");
                }
                printf("\nTekan Enter untuk kembali ke menu manajemen dokter...");
                getchar();
                break;

            case 4:
                clearScreen();
                printf("\n=== MUAT DATA DARI CSV ===\n");
                printf("Data akan dimuat dari file: %s\n", DOCTORS_FILE);
                if (confirmAction("Lanjutkan? Data dokter saat ini akan diganti")) {
                    startProgress("Memuat data dari CSV...");
                    if (bacaDokterDariCSV(DOCTORS_FILE, daftar, jumlah)) {
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

            case 5:
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

            case 0:
                return;

            default:
                printColored("Pilihan tidak valid.\n");
                printf("\nTekan Enter untuk melanjutkan...");
                getchar();
        }
    }
}

void tampilkanJadwalKalender(Shift jadwal[]) {
    printf("       JADWAL DOKTER JAGA - KALENDER BULANAN\n\n");
    printf(" Min  Sen  Sel  Rab  Kam  Jum  Sab\n");
    printLine('-', 50);
    
    // Define the start day of the month (0 = Sunday, 1 = Monday, etc.)
    // Assuming the month starts on a Monday for this example
    int start_day = 1; 
    
    // Print initial spaces for days before start of the month
    for (int i = 0; i < start_day; i++) {
        printf("     ");
    }
    
    // Print days in calendar format
    for (int day = 1; day <= 30; day++) {
        // Calculate the current day of week
        int day_of_week = (start_day + day - 1) % 7;
        
        // Print day number with padding for single digit days
        printf("%3d  ", day);
        
        // If it's Saturday or the last day, start a new line
        if (day_of_week == 6 || day == 30) {
            printf("\n");
            
            // Print shift indicators under each day in this week
            int start_of_week = day - day_of_week;
            
            // Print first row of shift indicators (P for Pagi)
            for (int j = 0; j < start_day && start_of_week == 1; j++) {
                printf("     ");  // Spaces for days outside the month
            }
            
            for (int j = start_of_week; j <= day; j++) {
                if (j > 0 && j <= 30) {
                    int base_idx = (j-1) * 3;
                    char p = (jadwal[base_idx].jumlah_dokter > 0) ? 'P' : '-';
                    printf("  %c  ", p);
                }
            }
            printf("\n");
            
            // Print second row (S for Siang)
            for (int j = 0; j < start_day && start_of_week == 1; j++) {
                printf("     ");
            }
            
            for (int j = start_of_week; j <= day; j++) {
                if (j > 0 && j <= 30) {
                    int base_idx = (j-1) * 3 + 1;
                    char s = (jadwal[base_idx].jumlah_dokter > 0) ? 'S' : '-';
                    printf("  %c  ", s);
                }
            }
            printf("\n");
            
            // Print third row (M for Malam)
            for (int j = 0; j < start_day && start_of_week == 1; j++) {
                printf("     ");
            }
            
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
    
    printf("\nP = Pagi, S = Siang, M = Malam\n");
    printf("- = Tidak ada dokter terjadwal\n");
}

void tampilkanJadwalPerDokter(Shift jadwal[], Dokter *dokter) {
    printf("Jadwal untuk Dr. %s:\n\n", dokter->nama);
    printf("%-10s %-10s\n", "Tanggal", "Shift");
    printLine('-', 25);
    
    int count = 0;
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
    
    if (count == 0) {
        printf("Dokter ini tidak memiliki jadwal shift.\n");
    } else {
        printf("\nTotal: %d shift\n", count);
    }
}

void navigasiJadwal(Shift jadwal[]) {
    int current_day = 0;
    char input;
    
    while (1) {
        clearScreen();
        tampilkanJadwalHarian(jadwal, current_day);
        printf("\n[<] Hari sebelumnya  [>] Hari berikutnya  [q] Keluar\n");
        
        input = getchar();
        if (input == 'q') break;
        else if (input == '<' && current_day > 0) current_day--;
        else if (input == '>' && current_day < 29) current_day++;
    }
}

int hitungShiftMingguIni(Shift jadwal[], Dokter* dokter, int minggu) {
    int shift_awal_minggu = minggu * (7 * 3);
    int shift_akhir_minggu = shift_awal_minggu + (7 * 3);
    
    // Make sure we don't go beyond the total number of shifts
    if (shift_akhir_minggu > TOTAL_SHIFT) {
        shift_akhir_minggu = TOTAL_SHIFT;
    }
    
    int jumlah_shift = 0;
    for (int i = shift_awal_minggu; i < shift_akhir_minggu; i++) {
        for (int j = 0; j < jadwal[i].jumlah_dokter; j++) {
            if (jadwal[i].dokter_bertugas[j] == dokter) {
                jumlah_shift++;
            }
        }
    }
    
    return jumlah_shift;
}

void tampilkanGrafikDistribusi(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter) {
    printf("Distribusi Shift (█ = 1 shift):\n\n");
    
    for (int i = 0; i < jumlah_dokter; i++) {
        int shifts = hitungJumlahShiftDokter(jadwal, &daftar_dokter[i]);
        printf("%-20s |", daftar_dokter[i].nama);
        
        for (int j = 0; j < shifts; j++) {
            printf("█");
        }
        printf(" (%d)\n", shifts);
    }
}

void statistikJadwalMenu(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter) {
    printf("\n=== STATISTIK JADWAL ===\n");
    printLine('-', 40);

    tampilkanJumlahShiftDokter(jadwal, daftar_dokter, jumlah_dokter);
    tampilkanDistribusiShift(jadwal, daftar_dokter, jumlah_dokter);
    analisisKeadilan(jadwal, daftar_dokter, jumlah_dokter);

    int pelanggaran = hitungPelanggaranPreferensi(jadwal);
    printf("\nJumlah pelanggaran preferensi: ");
    if (pelanggaran > 0) {
        printf("%s", pelanggaran > 2 ? "TINGGI" : "RENDAH");
        printf(" (%d)\n", pelanggaran);
    } else {
        printf("TIDAK ADA (0)\n");
    }

    printf("\nTekan Enter untuk melanjutkan...");
    getchar();
}

void inisialisasiJadwal(Shift jadwal[]) {
    for (int hari = 0, i = 0; hari < 30; hari++) {
        //  pagi
        jadwal[i].hari_ke = hari;
        strcpy(jadwal[i].tipe_shift, "pagi");
        jadwal[i].jumlah_dokter = 0;
        for (int j = 0; j < MAX_DOCTORS_PER_SHIFT; j++) {
            jadwal[i].dokter_bertugas[j] = NULL;
        }
        i++;

        //  siang
        jadwal[i].hari_ke = hari;
        strcpy(jadwal[i].tipe_shift, "siang");
        jadwal[i].jumlah_dokter = 0;
        for (int j = 0; j < MAX_DOCTORS_PER_SHIFT; j++) {
            jadwal[i].dokter_bertugas[j] = NULL;
        }
        i++;

        //  malam
        jadwal[i].hari_ke = hari;
        strcpy(jadwal[i].tipe_shift, "malam");
        jadwal[i].jumlah_dokter = 0;
        for (int j = 0; j < MAX_DOCTORS_PER_SHIFT; j++) {
            jadwal[i].dokter_bertugas[j] = NULL;
        }
        i++;
    }
}

// Simplified backtracking algorithm that focuses on one shift at a time
bool solveJadwal(Shift jadwal[], int idx, int doctors_needed_per_shift, Dokter dokter[], int jumlah_dokter) {
    // Base case: we've processed all shifts
    if (idx >= TOTAL_SHIFT) {
        return true;
    }
    
// If we already have the required doctors or reached MAX_DOCTORS_PER_SHIFT, move to next shift
if (jadwal[idx].jumlah_dokter >= doctors_needed_per_shift || 
    jadwal[idx].jumlah_dokter >= MAX_DOCTORS_PER_SHIFT) {
    return solveJadwal(jadwal, idx + 1, doctors_needed_per_shift, dokter, jumlah_dokter);
}

// If we have at least one doctor but not enough, try adding more doctors or move on
if (jadwal[idx].jumlah_dokter >= 1) {
    // Try to add more doctors to this shift
    for (int i = 0; i < jumlah_dokter; i++) {
        if (apakahAlokasiValid(jadwal, &dokter[i], idx)) {
            // Assign doctor to this shift
            jadwal[idx].dokter_bertugas[jadwal[idx].jumlah_dokter] = &dokter[i];
            jadwal[idx].jumlah_dokter++;
            
            // Try to fill the rest of the shifts (stay on same shift to add more doctors)
            if (solveJadwal(jadwal, idx, doctors_needed_per_shift, dokter, jumlah_dokter)) {
                return true;
            }
            
            // Backtrack
            jadwal[idx].jumlah_dokter--;
            jadwal[idx].dokter_bertugas[jadwal[idx].jumlah_dokter] = NULL;
        }
    }
    
    // If we couldn't add more doctors but have at least one, proceed to next shift
    printf("Shift %s hari %d hanya terisi %d dari %d dokter yang dibutuhkan.\n", 
           jadwal[idx].tipe_shift, jadwal[idx].hari_ke + 1, 
           jadwal[idx].jumlah_dokter, doctors_needed_per_shift);
    return solveJadwal(jadwal, idx + 1, doctors_needed_per_shift, dokter, jumlah_dokter);
}
    
    // Try each doctor for this shift
    for (int i = 0; i < jumlah_dokter; i++) {
        if (apakahAlokasiValid(jadwal, &dokter[i], idx)) {
            // Assign doctor to this shift
            jadwal[idx].dokter_bertugas[jadwal[idx].jumlah_dokter] = &dokter[i];
            jadwal[idx].jumlah_dokter++;
            
            // Try to fill the rest of the shifts
            if (solveJadwal(jadwal, idx + 1, doctors_needed_per_shift, dokter, jumlah_dokter)) {
                return true;
            }
            
            // Backtrack
            jadwal[idx].jumlah_dokter--;
            jadwal[idx].dokter_bertugas[jadwal[idx].jumlah_dokter] = NULL;
        }
    }
    
    // If we couldn't staff this shift with the minimum requirement, try to add more doctors
    // But only if this is an additional request (doctors_needed_per_shift > 1)
    if (doctors_needed_per_shift > 1 && jadwal[idx].jumlah_dokter >= 1) {
        printf("Shift %s hari %d hanya terisi %d dari %d dokter yang dibutuhkan.\n", 
               jadwal[idx].tipe_shift, jadwal[idx].hari_ke + 1, 
               jadwal[idx].jumlah_dokter, doctors_needed_per_shift);
        return solveJadwal(jadwal, idx + 1, doctors_needed_per_shift, dokter, jumlah_dokter);
    }
    
    // Couldn't find a solution for this shift
    return false;
}

int buatJadwalOtomatis(Shift jadwal[], Dokter dokter[], int jumlah_dokter) {
    inisialisasiJadwal(jadwal);

    if (jumlah_dokter < 1) {
        printf("Error: Jumlah dokter (%d) tidak mencukupi untuk membuat jadwal.\n", jumlah_dokter);
        return 0;
    }

    // Get the number of doctors needed per shift from user
    int max_possible = (jumlah_dokter > MAX_DOCTORS_PER_SHIFT) ? MAX_DOCTORS_PER_SHIFT : jumlah_dokter;
    char prompt_buffer[100];
    sprintf(prompt_buffer, "Masukkan jumlah dokter yang dibutuhkan per shift (1-%d): ", max_possible);
    int doctors_needed_per_shift = getIntInput(1, max_possible, prompt_buffer);

    if (jumlah_dokter < 1) {
        printf("Error: Jumlah dokter (%d) tidak mencukupi untuk membuat jadwal.\n", jumlah_dokter);
        return 0;
    }

    printf("Membuat jadwal dengan minimal %d dokter per shift...\n", doctors_needed_per_shift);
    startProgress("Menyusun jadwal...");
    
    if (solveJadwal(jadwal, 0, doctors_needed_per_shift, dokter, jumlah_dokter)) {
        endProgress("Jadwal berhasil dibuat!");
        
        // After successfully creating a schedule with minimum requirements,
        // try to add more doctors if requested and if there are enough doctors available
        if (doctors_needed_per_shift > 1) {
            printf("Mencoba menambahkan dokter tambahan ke shift yang masih kurang...\n");
            for (int i = 0; i < TOTAL_SHIFT; i++) {
                if (jadwal[i].jumlah_dokter < doctors_needed_per_shift) {
                    printf("Shift %s hari %d: %d/%d dokter\n", 
                           jadwal[i].tipe_shift, jadwal[i].hari_ke + 1,
                           jadwal[i].jumlah_dokter, doctors_needed_per_shift);
                }
            }
        }
        
        return 1;
    } else {
        endProgress("Gagal membuat jadwal!");
        printf("Tidak ada solusi yang memungkinkan dengan batasan yang ada.\n");
        printf("Coba kurangi batasan atau tambah jumlah dokter.\n");
        return 0;
    }
}
