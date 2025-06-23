/*
Program Statistik dan Analisis Jadwal Dokter
Fungsi:
1. Menampilkan grafik distribusi shift dokter
2. Menampilkan menu statistik dan analisis jadwal
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/types.h"
#include "../include/display.h"
#include "../include/utils.h"
#include "../include/constraints.h"
#include "../include/statistics.h"

void tampilkanGrafikDistribusi(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter) {
    printf("Distribusi Shift (# = 1 shift):\n\n");
    
    for (int i = 0; i < jumlah_dokter; i++) {
        int shifts = hitungJumlahShiftDokter(jadwal, &daftar_dokter[i]);
        printf("%-20s |", daftar_dokter[i].nama);
        
        // Tampilkan distribusi dengan karakter blok
        for (int j = 0; j < shifts; j++) {
            printf("#");
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
                validasiConstraintLengkap(jadwal, daftar_dokter, jumlah_dokter);
                break;
        }
        
        printf("\nTekan Enter untuk melanjutkan...");
        getchar();
    }
}