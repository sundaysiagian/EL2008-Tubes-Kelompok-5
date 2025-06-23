/*
Program Utama Sistem Penjadwalan Dokter RS Semoga Sehat Selalu
Fungsi:
1. Menampilkan splash screen dan menu utama
2. Mengelola dokter (tambah, hapus, tampilkan)
3. Membuat jadwal otomatis dan manual
4. Menampilkan jadwal (harian, mingguan, bulanan, per dokter)
5. Menyediakan statistik dan analisis jadwal
6. Menyimpan dan memuat jadwal dari file CSV
7. Pencarian dan validasi jadwal
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/types.h"
#include "../include/doctor_manager.h"
#include "../include/scheduler.h"
#include "../include/display.h"
#include "../include/utils.h"
#define SCHEDULE_FILE "../data/sample/jadwal_dokter.csv"

#ifdef _WIN32 // Cek apakah sistem operasi Windows
#include <windows.h>
#define SLEEP(ms) Sleep(ms) // Fungsi sleep untuk Windows (dalam ms)
#else // Jika bukan Windows 
#include <unistd.h>
#define SLEEP(ms) usleep((ms) * 1000) // Fungsi sleep untuk Unix/Linux (konversi ms ke us)
#endif

void displaySplashScreen() {
    clearScreen(); // Bersihkan layar terminal
    printf("\n\n");
    
    // ASCII art 
    printf("     _______. _______ .___  ___.   ______     _______      ___      \n");
    printf("    /       ||   ____||   \\/   |  /  __  \\   /  _____|    /   \\     \n");
    printf("   |   (----`|  |__   |  \\  /  | |  |  |  | |  |  __     /  ^  \\    \n");
    printf("    \\   \\    |   __|  |  |\\/|  | |  |  |  | |  | |_ |   /  /_\\  \\   \n");
    printf(".----)   |   |  |____ |  |  |  | |  `--'  | |  |__| |  /  _____  \\  \n");
    printf("|_______/    |_______||__|  |__|  \\______/   \\______| /__/     \\__\\ \n");
    printf("                                                                    \n");
    printf("     _______. _______  __    __       ___   .___________.           \n");
    printf("    /       ||   ____||  |  |  |     /   \\  |           |           \n");
    printf("   |   (----`|  |__   |  |__|  |    /  ^  \\ `---|  |----`           \n");
    printf("    \\   \\    |   __|  |   __   |   /  /_\\  \\    |  |                \n");
    printf(".----)   |   |  |____ |  |  |  |  /  _____  \\   |  |                \n");
    printf("|_______/    |_______||__|  |__| /__/     \\__\\  |__|                \n");
    printf("                                                                    \n");
    printf("     _______. _______  __          ___       __       __    __      \n");
    printf("    /       ||   ____||  |        /   \\     |  |     |  |  |  |     \n");
    printf("   |   (----`|  |__   |  |       /  ^  \\    |  |     |  |  |  |     \n");
    printf("    \\   \\    |   __|  |  |      /  /_\\  \\   |  |     |  |  |  |     \n");
    printf(".----)   |   |  |____ |  `----./  _____  \\  |  `----.|  `--'  |     \n");
    printf("|_______/    |_______||_______/__/     \\__\\ |_______| \\______/      \n");
    printf("                                                                    \n");
    printf("\n\n");
    
    
    printf("                       Programmed by Kelompok 5                       \n");
    printf("\n");
    printf("                           For Pak Hilton                            \n");
    printf("\n\n");

    // Animasi sederhana loading screen
    printf("                           Loading system");
    for (int i = 0; i < 3; i++) { 
        for (int j = 0; j < 3; j++) { 
            printf(".");
            fflush(stdout); 
            SLEEP(300); 
        }
        printf("\b\b\b   \b\b\b"); 
        fflush(stdout);
    }

    printf("\n\n                      Press Enter to continue...");
    getchar(); // Tunggu user tekan Enter
}

int main() {
    Dokter daftar_dokter[MAX_DOCTORS]; 
    Shift jadwal[TOTAL_SHIFT]; 
    int jumlah_dokter = 0; 
    int jadwal_dibuat = 0; 
    int pilihan; 
    
    displaySplashScreen(); // Tampilkan splash screen saat program dimulai
    
    while (1) { // Infinite loop
        clearScreen(); 
        displayEnhancedMenu(); // Tampilkan menu utama
        
        pilihan = getIntInput(0, 11, ""); // Ambil input pilihan menu dari user (0-11)
        
        switch (pilihan) { // Switch case untuk handling setiap pilihan menu
            case 1: // Menu manajemen dokter
                doctorManagementMenu(daftar_dokter, &jumlah_dokter);
                break;
                
            case 2: // Menu buat njadwal otomatis
                clearScreen();
                printf("\n=== BUAT JADWAL OTOMATIS ===\n");
                if (jumlah_dokter < 1) { // Cek apakah ada dokter yang terdaftar
                    printf("Error: Tidak ada dokter untuk dijadwalkan.\n");
                    printf("Silakan tambahkan dokter terlebih dahulu.\n");
                    printf("\nTekan Enter untuk melanjutkan...");
                    getchar();
                    break;
                }
                
                printf("Data dokter yang tersedia:\n");
                tampilkanDaftarDokter(daftar_dokter, jumlah_dokter); // Tampilkan daftar dokter
                
                if (confirmAction("Buat jadwal otomatis dengan data dokter di atas")) { // Konfirmasi user
                    if (buatJadwalOtomatis(jadwal, daftar_dokter, jumlah_dokter)) { // Coba buat jadwal
                        jadwal_dibuat = 1; // Set flag jadwal sudah dibuat
                        printf("\n==============\n");
                    } else {
                        printf("\nGagal membuat jadwal. Mohon periksa data dokter.\n");
                    }
                } else {
                    printf("Pembuatan jadwal dibatalkan.\n");
                }
                
                printf("\nTekan Enter untuk melanjutkan...");
                getchar();
                break;
                
            case 3: // Menu tampilkan jadwal harian
                clearScreen();
                printf("\n=== TAMPILKAN JADWAL HARIAN ===\n");
                if (!jadwal_dibuat) { // Cek apakah jadwal sudah dibuat
                    printf("Error: Jadwal belum dibuat.\n");
                    printf("Silakan buat jadwal terlebih dahulu.\n");
                    printf("\nTekan Enter untuk melanjutkan...");
                    getchar();
                    break;
                }
                
                int hari = getIntInput(1, 30, "Masukkan hari (1-30): "); // Input hari yang ingin dilihat
                tampilkanJadwalHarian(jadwal, hari - 1); // Tampilkan jadwal (hari-1 karena array dimulai dari 0)
                
                printf("\nTekan Enter untuk melanjutkan...");
                getchar();
                break;
                
            case 4: // Menu tampilkan jadwal mingguan
                clearScreen();
                printf("\n=== TAMPILKAN JADWAL MINGGUAN ===\n");
                if (!jadwal_dibuat) { // Cek apakah jadwal sudah dibuat
                    printf("Error: Jadwal belum dibuat.\n");
                    printf("Silakan buat jadwal terlebih dahulu.\n");
                    printf("\nTekan Enter untuk melanjutkan...");
                    getchar();
                    break;
                }
                
                int minggu = getIntInput(1, 5, "Masukkan minggu (1-5): "); // Input minggu yang ingin dilihat
                tampilkanJadwalMingguan(jadwal, minggu - 1); // Tampilkan jadwal mingguan
                
                printf("\nTekan Enter untuk melanjutkan...");
                getchar();
                break;
                
            case 5: // Menu tampilkan jadwal bulanan
                clearScreen();
                printf("\n=== TAMPILKAN JADWAL BULANAN ===\n");
                if (!jadwal_dibuat) { // Cek apakah jadwal sudah dibuat
                    printf("Error: Jadwal belum dibuat.\n");
                    printf("Silakan buat jadwal terlebih dahulu.\n");
                    printf("\nTekan Enter untuk melanjutkan...");
                    getchar();
                    break;
                }
                
                tampilkanJadwalBulanan(jadwal); // Tampilkan jadwal untuk semua bulan
                
                printf("\nTekan Enter untuk melanjutkan...");
                getchar();
                break;
                
            case 6: // Menu statistik jadwal
                clearScreen();
                if (!jadwal_dibuat) { // Cek apakah jadwal sudah dibuat
                    printf("Error: Jadwal belum dibuat.\n");
                    printf("Silakan buat jadwal terlebih dahulu.\n");
                    printf("\nTekan Enter untuk melanjutkan...");
                    getchar();
                    break;
                }
                
                statistikJadwalMenu(jadwal, daftar_dokter, jumlah_dokter); // Tampilkan menu statistik
                break;
                
            case 7: // Menu simpan jadwal ke file
                clearScreen();
                printf("\n=== SIMPAN JADWAL KE FILE ===\n");
                if (!jadwal_dibuat) { // Cek apakah jadwal sudah dibuat
                    printf("Error: Jadwal belum dibuat.\n");
                    printf("Silakan buat jadwal terlebih dahulu.\n");
                    printf("\nTekan Enter untuk melanjutkan...");
                    getchar();
                    break;
                }
                
                printf("Jadwal akan disimpan ke: %s\n", SCHEDULE_FILE); // Tampilkan lokasi file
                if (confirmAction("Lanjutkan penyimpanan")) { // Konfirmasi user
                    startProgress("Menyimpan jadwal ke CSV..."); // Tampilkan progress
                    simpanJadwalKeCSV(jadwal, SCHEDULE_FILE); // Simpan ke file CSV
                    endProgress("Jadwal berhasil disimpan ke CSV");
                } else {
                    printf("Penyimpanan dibatalkan.\n");
                }
                
                printf("\nTekan Enter untuk melanjutkan...");
                getchar();
                break;

            case 8: // Menu tampilkan jadwal per dokter
                clearScreen();
                printf("\n=== TAMPILKAN JADWAL PER DOKTER ===\n");
                if (!jadwal_dibuat) { // Cek apakah jadwal sudah dibuat
                    printf("Error: Jadwal belum dibuat.\n");
                    printf("Silakan buat jadwal terlebih dahulu.\n");
                    printf("\nTekan Enter untuk melanjutkan...");
                    getchar();
                    break;
                }
                
                if (jumlah_dokter <= 0) { // Cek apakah ada dokter yang tersedia
                    printf("Tidak ada dokter yang tersedia.\n");
                    printf("\nTekan Enter untuk melanjutkan...");
                    getchar();
                    break;
                }
                
                tampilkanDaftarDokter(daftar_dokter, jumlah_dokter); // Tampilkan daftar dokter
                int dok_idx = getIntInput(1, jumlah_dokter, "Pilih nomor dokter: ") - 1; // Pilih dokter (index-1)
                tampilkanJadwalPerDokter(jadwal, &daftar_dokter[dok_idx]); // Tampilkan jadwal dokter terpilih
                
                printf("\nTekan Enter untuk melanjutkan...");
                getchar();
                break;

            case 9: // Menu muat jadwal dari file
                clearScreen();
                printf("\n=== MUAT JADWAL DARI FILE ===\n");
                if (jumlah_dokter <= 0) { // Cek apakah ada data dokter
                    printf("Error: Tidak ada data dokter yang tersedia.\n");
                    printf("Silakan tambahkan dokter terlebih dahulu.\n");
                    printf("\nTekan Enter untuk melanjutkan...");
                    getchar();
                    break;
                }
                
                printf("Jadwal akan dimuat dari: %s\n", SCHEDULE_FILE); // Tampilkan lokasi file
                if (confirmAction("Lanjutkan? Jadwal yang sudah ada akan ditimpa")) { // Konfirmasi overwrite
                    startProgress("Memuat jadwal dari CSV..."); // Tampilkan progress
                    if (bacaJadwalDariCSV(jadwal, SCHEDULE_FILE, daftar_dokter, jumlah_dokter)) { // Coba baca file
                        jadwal_dibuat = 1; // Set flag jadwal sudah dibuat
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
            
            case 10: // Menu edit jadwal manual
                clearScreen();
                printf("\n=== EDIT JADWAL MANUAL ===\n");
                if (!jadwal_dibuat) { // Cek apakah jadwal sudah dibuat
                    printf("Error: Jadwal belum dibuat.\n");
                    printf("Silakan buat jadwal terlebih dahulu.\n");
                    printf("\nTekan Enter untuk melanjutkan...");
                    getchar();
                    break;
                }
                
                if (jumlah_dokter <= 0) { // Cek apakah ada dokter yang tersedia
                    printf("Error: Tidak ada dokter yang tersedia.\n");
                    printf("\nTekan Enter untuk melanjutkan...");
                    getchar();
                    break;
                }
                
                editJadwalManual(jadwal, daftar_dokter, jumlah_dokter); // Masuk ke menu edit manual
                break;
        
            case 11: // Menu pencarian
                clearScreen();
                printf("\n=== PENCARIAN ===\n");
                if (!jadwal_dibuat) { // Cek apakah jadwal sudah dibuat
                    printf("Error: Jadwal belum dibuat.\n");
                    printf("Silakan buat jadwal terlebih dahulu.\n");
                    printf("\nTekan Enter untuk melanjutkan...");
                    getchar();
                    break;
                }
                
                pencarian(jadwal, daftar_dokter, jumlah_dokter); // Masuk ke menu pencarian
                break;
                
            case 0: // Menu keluar aplikasi
                clearScreen();
                printf("\n=== KELUAR APLIKASI ===\n");
                if (confirmAction("Apakah Anda yakin ingin keluar")) { // Konfirmasi keluar
                    printf("\nTerima kasih telah menggunakan Sistem Penjadwalan Dokter RS Semoga Sehat Selalu!\n");
                    return 0; // Keluar dari program
                }
                break;
                
            default: // Handle input tidak valid
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
                printf("\nTekan Enter untuk melanjutkan...");
                getchar();
        }
    }
    return 0;
}