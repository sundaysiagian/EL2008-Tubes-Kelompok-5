#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Menu and user interface functions
void show_main_menu(void) {
    print_header("SISTEM INFORMASI JADWAL DOKTER");
    printf("1. Kelola Data Dokter\n");
    printf("2. Lihat Jadwal\n");
    printf("3. Cari Dokter\n");
    printf("4. Statistik\n");
    printf("0. Keluar\n");
    print_separator();
    printf("Pilih menu: ");
}

void show_doctor_menu(void) {
    print_header("MENU KELOLA DOKTER");
    printf("1. Tampilkan Semua Dokter\n");
    printf("2. Cari Dokter berdasarkan Nama\n");
    printf("3. Cari Dokter berdasarkan Spesialisasi\n");
    printf("4. Tambah Dokter Baru\n");
    printf("5. Edit Data Dokter\n");
    printf("0. Kembali ke Menu Utama\n");
    print_separator();
    printf("Pilih menu: ");
}

void show_schedule_menu(void) {
    print_header("MENU JADWAL DOKTER");
    printf("1. Tampilkan Semua Jadwal\n");
    printf("2. Jadwal berdasarkan Hari\n");
    printf("3. Jadwal berdasarkan Dokter\n");
    printf("4. Jadwal berdasarkan Spesialisasi\n");
    printf("5. Jadwal Hari Ini\n");
    printf("6. Generate Jadwal Mingguan\n");
    printf("0. Kembali ke Menu Utama\n");
    print_separator();
    printf("Pilih menu: ");
}

int get_user_choice(int min, int max) {
    int choice;
    char input[100];
    
    while (1) {
        if (fgets(input, sizeof(input), stdin) != NULL) {
            if (sscanf(input, "%d", &choice) == 1) {
                if (choice >= min && choice <= max) {
                    return choice;
                } else {
                    print_error("Pilihan tidak valid! Silakan pilih antara %d-%d");
                    printf("Pilih menu: ");
                }
            } else {
                print_error("Input tidak valid! Masukkan angka.");
                printf("Pilih menu: ");
            }
        }
    }
}

// Display formatting functions
void print_header(const char* title) {
    int title_len = strlen(title);
    int total_width = 60;
    int padding = (total_width - title_len - 2) / 2;
    
    printf("\n");
    for (int i = 0; i < total_width; i++) printf("=");
    printf("\n");
    
    printf("🏥");
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s", title);
    for (int i = 0; i < padding; i++) printf(" ");
    printf("🏥\n");
    
    for (int i = 0; i < total_width; i++) printf("=");
    printf("\n");
}

void print_separator(void) {
    for (int i = 0; i < 60; i++) printf("-");
    printf("\n");
}

void print_table_header_doctors(void) {
    printf("\n");
    printf("%-8s | %-25s | %-15s | %-8s | %-12s | %-8s | %-8s\n",
           "ID", "Nama Dokter", "Spesialisasi", "Hari", "Jam", "Ruangan", "Status");
    for (int i = 0; i < 90; i++) printf("-");
    printf("\n");
}

void print_table_header_schedule(void) {
    printf("\n");
    printf("%-10s | %-8s | %-25s | %-15s | %-8s\n",
           "Hari", "Jam", "Dokter", "Spesialisasi", "Ruangan");
    for (int i = 0; i < 80; i++) printf("-");
    printf("\n");
}

// Interactive functions
void display_doctor_selection(const Doctor* doctors, int count) {
    if (count == 0) {
        print_warning("Tidak ada data dokter tersedia.");
        return;
    }
    
    print_header("DAFTAR DOKTER");
    print_table_header_doctors();
    
    for (int i = 0; i < count; i++) {
        printf("%-8s | %-25s | %-15s | %-8s | %s-%s | %-8s | %-8s\n",
               doctors[i].id_dokter,
               doctors[i].nama_dokter,
               doctors[i].spesialisasi,
               doctors[i].hari,
               doctors[i].jam_mulai,
               doctors[i].jam_selesai,
               doctors[i].ruangan,
               doctors[i].status);
    }
    
    printf("\nTotal: %d dokter\n", count);
}

void display_schedule_by_day(const ScheduleEntry* schedule, int count) {
    if (count == 0) {
        print_warning("Tidak ada jadwal tersedia.");
        return;
    }
    
    // Group by days
    char days[][10] = {"Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu", "Minggu"};
    
    for (int d = 0; d < 7; d++) {
        int day_count = 0;
        
        // Count entries for this day
        for (int i = 0; i < count; i++) {
            if (strcmp(schedule[i].hari, days[d]) == 0) {
                day_count++;
            }
        }
        
        if (day_count > 0) {
            printf("\n📅 JADWAL HARI %s (%d dokter)\n", days[d], day_count);
            print_separator();
            
            for (int i = 0; i < count; i++) {
                if (strcmp(schedule[i].hari, days[d]) == 0) {
                    printf("🕐 %-8s | 👨‍⚕️ %-25s | 🏥 %-15s | 🚪 %s\n",
                           schedule[i].jam,
                           schedule[i].dokter,
                           schedule[i].spesialisasi,
                           schedule[i].ruangan);
                }
            }
        }
    }
}

void display_schedule_by_doctor(const ScheduleEntry* schedule, int count, const char* doctor_name) {
    if (count == 0) {
        print_warning("Tidak ada jadwal tersedia.");
        return;
    }
    
    int found = 0;
    printf("\n📅 JADWAL DOKTER: %s\n", doctor_name);
    print_separator();
    
    for (int i = 0; i < count; i++) {
        if (strstr(schedule[i].dokter, doctor_name) != NULL) {
            if (!found) {
                printf("Spesialisasi: %s\n\n", schedule[i].spesialisasi);
                found = 1;
            }
            
            printf("📍 %-10s | 🕐 %-8s | 🚪 Ruangan %s\n",
                   schedule[i].hari,
                   schedule[i].jam,
                   schedule[i].ruangan);
        }
    }
    
    if (!found) {
        print_error("Tidak ditemukan jadwal untuk dokter tersebut.");
    } else {
        printf("\nTotal jadwal: %d\n", found);
    }
}

// Error and status messages
void print_error(const char* message) {
    printf("\n❌ ERROR: %s\n", message);
}

void print_success(const char* message) {
    printf("\n✅ SUCCESS: %s\n", message);
}

void print_warning(const char* message) {
    printf("\n⚠️  WARNING: %s\n", message);
}

// Additional utility display functions
void display_doctor_card(const Doctor* doctor) {
    printf("\n┌────────────────────────────────────────┐\n");
    printf("│             KARTU DOKTER               │\n");
    printf("├────────────────────────────────────────┤\n");
    printf("│ ID       : %-27s │\n", doctor->id_dokter);
    printf("│ Nama     : %-27s │\n", doctor->nama_dokter);
    printf("│ Spesialis: %-27s │\n", doctor->spesialisasi);
    printf("│ Hari     : %-27s │\n", doctor->hari);
    printf("│ Jam      : %s - %-18s │\n", doctor->jam_mulai, doctor->jam_selesai);
    printf("│ Ruangan  : %-27s │\n", doctor->ruangan);
    printf("│ Status   : %-27s │\n", doctor->status);
    printf("└────────────────────────────────────────┘\n");
}

void display_statistics_summary(int total_doctors, int total_schedules, int total_specialties) {
    print_header("STATISTIK SISTEM");
    printf("📊 Total Dokter      : %d\n", total_doctors);
    printf("📅 Total Jadwal      : %d\n", total_schedules);
    printf("🏥 Total Spesialisasi: %d\n", total_specialties);
    print_separator();
}

void display_loading_animation(const char* message) {
    printf("%s", message);
    for (int i = 0; i < 3; i++) {
        printf(".");
        fflush(stdout);
        // Simple delay simulation
        for (volatile int j = 0; j < 100000000; j++);
    }
    printf(" Selesai!\n");
}

void clear_screen(void) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pause_screen(void) {
    printf("\nTekan Enter untuk melanjutkan...");
    getchar();
}

void display_welcome_banner(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════╗\n");
    printf("║                                                          ║\n");
    printf("║        🏥 SISTEM MANAJEMEN JADWAL DOKTER 🏥            ║\n");
    printf("║                                                          ║\n");
    printf("║              Kelompok 5 - EL2008 Tubes                  ║\n");
    printf("║                                                          ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n");
    printf("\n");
}

void display_goodbye_message(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════╗\n");
    printf("║                                                          ║\n");
    printf("║                 👋 Terima Kasih! 👋                    ║\n");
    printf("║                                                          ║\n");
    printf("║         Sistem Jadwal Dokter telah ditutup              ║\n");
    printf("║                                                          ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n");
}