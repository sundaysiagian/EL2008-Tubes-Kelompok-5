#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/types.h"
#include "../include/doctor_manager.h"
#include "../include/scheduler.h"
#include "../include/display.h"
#include "../include/utils.h"
#define SCHEDULE_FILE "../data/jadwal.csv"

void displaySplashScreen() {
    clearScreen();
    printf("\n\n");
    printf("  ███████╗███████╗███╗   ███╗ ██████╗  ██████╗  █████╗ \n");
    printf("  ██╔════╝██╔════╝████╗ ████║██╔═══██╗██╔════╝ ██╔══██╗\n");
    printf("  ███████╗█████╗  ██╔████╔██║██║   ██║██║  ███╗███████║\n");
    printf("  ╚════██║██╔══╝  ██║╚██╔╝██║██║   ██║██║   ██║██╔══██║\n");
    printf("  ███████║███████╗██║ ╚═╝ ██║╚██████╔╝╚██████╔╝██║  ██║\n");
    printf("  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝  ╚═════╝ ╚═╝  ╚═╝\n");
    printf("\n");
    printf("  ███████╗███████╗██╗  ██╗ █████╗ ████████╗\n");
    printf("  ██╔════╝██╔════╝██║  ██║██╔══██╗╚══██╔══╝\n");
    printf("  ███████╗█████╗  ███████║███████║   ██║   \n");
    printf("  ╚════██║██╔══╝  ██╔══██║██╔══██║   ██║   \n");
    printf("  ███████║███████╗██║  ██║██║  ██║   ██║   \n");
    printf("  ╚══════╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   \n");
    printf("\n");
    printf("  ███████╗███████╗██╗      █████╗ ██╗     ██╗   ██╗\n");
    printf("  ██╔════╝██╔════╝██║     ██╔══██╗██║     ██║   ██║\n");
    printf("  ███████╗█████╗  ██║     ███████║██║     ██║   ██║\n");
    printf("  ╚════██║██╔══╝  ██║     ██╔══██║██║     ██║   ██║\n");
    printf("  ███████║███████╗███████╗██║  ██║███████╗╚██████╔╝\n");
    printf("  ╚══════╝╚══════╝╚══════╝╚═╝  ╚═╝╚══════╝ ╚═════╝ \n");
    printf("\n\n");
    printf("              Programmed by Kelompok 5             \n");
    printf("\n\n");
    printf("                  For Pak Hilton                   \n");
    printf("\n\n");
    
    // Animation for loading
    printf("                      Loading system");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf(".");
            fflush(stdout);
            usleep(300000); // 300ms delay
        }
        printf("\b\b\b   \b\b\b"); // Clear the dots
        fflush(stdout);
    }
    
    printf("\n\n                Press Enter to continue...");
    getchar();
}
int main() {
    Dokter daftar_dokter[MAX_DOCTORS];
    Shift jadwal[TOTAL_SHIFT];
    int jumlah_dokter = 0;
    int jadwal_dibuat = 0;
    int pilihan;
    char input[100];
    
    // Display the splash screen
    displaySplashScreen();
    
    // Main program loop
    while (1) {
        clearScreen();
        displayEnhancedMenu();
        
        pilihan = getIntInput(0, 11, "");
        
        switch (pilihan) {
            case 1:
                doctorManagementMenu(daftar_dokter, &jumlah_dokter);
                break;
                
            case 2:
                clearScreen();
                printf("\n=== BUAT JADWAL OTOMATIS ===\n");
                if (jumlah_dokter < 1) {
                    printf("Error: Tidak ada dokter untuk dijadwalkan.\n");
                    printf("Silakan tambahkan dokter terlebih dahulu.\n");
                    printf("\nTekan Enter untuk melanjutkan...");
                    getchar();
                    break;
                }
                
                printf("Data dokter yang tersedia:\n");
                tampilkanDaftarDokter(daftar_dokter, jumlah_dokter);
                
                if (confirmAction("Buat jadwal otomatis dengan data dokter di atas")) {
                    if (buatJadwalOtomatis(jadwal, daftar_dokter, jumlah_dokter)) {
                        jadwal_dibuat = 1;
                        printf("\nJadwal berhasil dibuat!\n");
                    } else {
                        printf("\nGagal membuat jadwal. Mohon periksa data dokter.\n");
                    }
                } else {
                    printf("Pembuatan jadwal dibatalkan.\n");
                }
                
                printf("\nTekan Enter untuk melanjutkan...");
                getchar();
                break;
                
            case 3:
                clearScreen();
                printf("\n=== TAMPILKAN JADWAL HARIAN ===\n");
                if (!jadwal_dibuat) {
                    printf("Error: Jadwal belum dibuat.\n");
                    printf("Silakan buat jadwal terlebih dahulu.\n");
                    printf("\nTekan Enter untuk melanjutkan...");
                    getchar();
                    break;
                }
                
                int hari = getIntInput(1, 30, "Masukkan hari (1-30): ");
                tampilkanJadwalHarian(jadwal, hari - 1);
                
                printf("\nTekan Enter untuk melanjutkan...");
                getchar();
                break;
                
            case 4:
                clearScreen();
                printf("\n=== TAMPILKAN JADWAL MINGGUAN ===\n");
                if (!jadwal_dibuat) {
                    printf("Error: Jadwal belum dibuat.\n");
                    printf("Silakan buat jadwal terlebih dahulu.\n");
                    printf("\nTekan Enter untuk melanjutkan...");
                    getchar();
                    break;
                }
                
                int minggu = getIntInput(1, 5, "Masukkan minggu (1-5): ");
                tampilkanJadwalMingguan(jadwal, minggu - 1);
                
                printf("\nTekan Enter untuk melanjutkan...");
                getchar();
                break;
                
            case 5:
                clearScreen();
                printf("\n=== TAMPILKAN JADWAL BULANAN ===\n");
                if (!jadwal_dibuat) {
                    printf("Error: Jadwal belum dibuat.\n");
                    printf("Silakan buat jadwal terlebih dahulu.\n");
                    printf("\nTekan Enter untuk melanjutkan...");
                    getchar();
                    break;
                }
                
                tampilkanJadwalBulanan(jadwal);
                
                printf("\nTekan Enter untuk melanjutkan...");
                getchar();
                break;
                
            case 6:
                clearScreen();
                if (!jadwal_dibuat) {
                    printf("Error: Jadwal belum dibuat.\n");
                    printf("Silakan buat jadwal terlebih dahulu.\n");
                    printf("\nTekan Enter untuk melanjutkan...");
                    getchar();
                    break;
                }
                
                statistikJadwalMenu(jadwal, daftar_dokter, jumlah_dokter);
                break;
                
            case 7:
                clearScreen();
                printf("\n=== SIMPAN JADWAL KE FILE ===\n");
                if (!jadwal_dibuat) {
                    printf("Error: Jadwal belum dibuat.\n");
                    printf("Silakan buat jadwal terlebih dahulu.\n");
                    printf("\nTekan Enter untuk melanjutkan...");
                    getchar();
                    break;
                }
                
                printf("Jadwal akan disimpan ke: %s\n", SCHEDULE_FILE);
                if (confirmAction("Lanjutkan penyimpanan")) {
                    startProgress("Menyimpan jadwal ke CSV...");
                    simpanJadwalKeCSV(jadwal, SCHEDULE_FILE);
                    endProgress("Jadwal berhasil disimpan ke CSV");
                } else {
                    printf("Penyimpanan dibatalkan.\n");
                }
                
                printf("\nTekan Enter untuk melanjutkan...");
                getchar();
                break;

                case 8:
                    clearScreen();
                    printf("\n=== TAMPILKAN JADWAL PER DOKTER ===\n");
                    if (!jadwal_dibuat) {
                        printf("Error: Jadwal belum dibuat.\n");
                        printf("Silakan buat jadwal terlebih dahulu.\n");
                        printf("\nTekan Enter untuk melanjutkan...");
                        getchar();
                        break;
                    }
                    
                    if (jumlah_dokter <= 0) {
                        printf("Tidak ada dokter yang tersedia.\n");
                        printf("\nTekan Enter untuk melanjutkan...");
                        getchar();
                        break;
                    }
                    
                    tampilkanDaftarDokter(daftar_dokter, jumlah_dokter);
                    int dok_idx = getIntInput(1, jumlah_dokter, "Pilih nomor dokter: ") - 1;
                    tampilkanJadwalPerDokter(jadwal, &daftar_dokter[dok_idx]);
                    
                    printf("\nTekan Enter untuk melanjutkan...");
                    getchar();
                    break;

            case 9:
                clearScreen();
                printf("\n=== MUAT JADWAL DARI FILE ===\n");
                if (jumlah_dokter <= 0) {
                    printf("Error: Tidak ada data dokter yang tersedia.\n");
                    printf("Silakan tambahkan dokter terlebih dahulu.\n");
                    printf("\nTekan Enter untuk melanjutkan...");
                    getchar();
                    break;
                }
                
                printf("Jadwal akan dimuat dari: %s\n", SCHEDULE_FILE);
                if (confirmAction("Lanjutkan? Jadwal yang sudah ada akan ditimpa")) {
                    startProgress("Memuat jadwal dari CSV...");
                    if (bacaJadwalDariCSV(jadwal, SCHEDULE_FILE, daftar_dokter, jumlah_dokter)) {
                        jadwal_dibuat = 1;
                        endProgress("Jadwal berhasil dimuat dari CSV");
                    } else {
                        endProgress("Gagal memuat jadwal dari CSV");
                    }
                } else {
                    printf("Pembacaan jadwal dibatalkan.\n");
                }
                
                printf("\nTekan Enter untuk melanjutkan...");
                getchar();
                break;
            
            case 10:
                clearScreen();
                printf("\n=== EDIT JADWAL MANUAL ===\n");
                if (!jadwal_dibuat) {
                    printf("Error: Jadwal belum dibuat.\n");
                    printf("Silakan buat jadwal terlebih dahulu.\n");
                    printf("\nTekan Enter untuk melanjutkan...");
                    getchar();
                    break;
                }
                
                if (jumlah_dokter <= 0) {
                    printf("Error: Tidak ada dokter yang tersedia.\n");
                    printf("\nTekan Enter untuk melanjutkan...");
                    getchar();
                    break;
                }
                
                editJadwalManual(jadwal, daftar_dokter, jumlah_dokter);
                break;
        
            case 11:
                clearScreen();
                printf("\n=== PENCARIAN ===\n");
                if (!jadwal_dibuat) {
                    printf("Error: Jadwal belum dibuat.\n");
                    printf("Silakan buat jadwal terlebih dahulu.\n");
                    printf("\nTekan Enter untuk melanjutkan...");
                    getchar();
                    break;
                }
                
                pencarian(jadwal, daftar_dokter, jumlah_dokter);
                break;
                
            case 0:
                clearScreen();
                printf("\n=== KELUAR APLIKASI ===\n");
                if (confirmAction("Apakah Anda yakin ingin keluar")) {
                    printf("\nTerima kasih telah menggunakan Sistem Penjadwalan Dokter RS Semoga Sehat Selalu!\n");
                    return 0;
                }
                break;
                
            default:
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
                printf("\nTekan Enter untuk melanjutkan...");
                getchar();
        }
    }
    
    return 0;
}