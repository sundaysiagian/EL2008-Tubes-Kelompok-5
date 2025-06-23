/*
Program Pencarian Jadwal Dokter
Fungsi:
1. Mencari dokter berdasarkan nama atau bagian nama
2. Mencari jadwal berdasarkan tanggal
3. Menampilkan menu pencarian untuk mengakses fungsi di atas
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/types.h"
#include "../include/search.h"
#include "../include/utils.h"

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