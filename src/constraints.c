/**
 * Memvalidasi apakah penugasan dokter ke shift tertentu sesuai dengan ketentuan berikut:
 * 1. Kapasitas shift
 * 2. Dokter sudah ada di shift ini 
 * 3. Dokter sudah bertugas di shift lain pada hari yang sama
 * 4. Maksimal 3 shift malam per minggu
 * 5. Maksimal 5 shift total per minggu
 * 6. Tidak ada shift berturut-turut (shift malam diikuti shift pagi)
 * 7. Memperhatikan preferensi shift dokter
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/types.h"
#include "../include/constraints.h"

bool apakahAlokasiValid(Shift jadwal[], Dokter* dokter, int index_shift, bool check_preferences) {
    int hari_ini = index_shift / 3; // Hitung hari dari index shift (setiap hari ada 3 shift)
    int shift_dalam_hari = index_shift % 3; // Hitung jenis shift dalam hari (0=pagi, 1=siang, 2=malam)
    
    if (jadwal[index_shift].jumlah_dokter >= MAX_DOCTORS_PER_SHIFT) {
        return false; // Tolak jika jumlah dokter sudah mencapai maksimum (Constraint kapasitas shift)
    }

    for (int i = 0; i < jadwal[index_shift].jumlah_dokter; i++) {
        if (jadwal[index_shift].dokter_bertugas[i] == dokter) {
            return false; // Tolak jika dokter sudah ditugaskan di shift ini (Constraint duplikasi dokter)
        }
    }

    int awal_shift_hari_ini = hari_ini * 3; // Hitung index shift pertama untuk hari ini
    if (shift_dalam_hari > 0) { // Jika bukan shift pagi (shift > 0)
        int shift_sebelumnya = awal_shift_hari_ini + (shift_dalam_hari - 1);
        for (int j = 0; j < jadwal[shift_sebelumnya].jumlah_dokter; j++) {
            if (jadwal[shift_sebelumnya].dokter_bertugas[j] == dokter) {
                return false; // Tolak jika dokter bertugas di shift sebelumnya (Constraint shift berurutan)
            }
        }
    }
    
    if (shift_dalam_hari < 2) { // Jika bukan shift malam (shift < 2)
        int shift_sesudahnya = awal_shift_hari_ini + (shift_dalam_hari + 1);
        for (int j = 0; j < jadwal[shift_sesudahnya].jumlah_dokter; j++) {
            if (jadwal[shift_sesudahnya].dokter_bertugas[j] == dokter) {
                return false; // Tolak jika dokter bertugas di shift sesudahnya (Constraint shift berurutan)
            }
        }
    }

    if (check_preferences) { // Jika perlu validasi preferensi
        const char *shift_type = jadwal[index_shift].tipe_shift;
        if ((strcmp(shift_type, "pagi") == 0 || strcmp(shift_type, "Pagi") == 0) && dokter->pref_pagi == 0) {
            return false; // Tolak jika shift pagi tapi dokter tidak prefer shift pagi (Constraint preferensi)
        }
        if ((strcmp(shift_type, "siang") == 0 || strcmp(shift_type, "Siang") == 0) && dokter->pref_siang == 0) {
            return false; // Tolak jika shift siang tapi dokter tidak prefer shift siang (Constraint preferensi)
        }
        if ((strcmp(shift_type, "malam") == 0 || strcmp(shift_type, "Malam") == 0) && dokter->pref_malam == 0) {
            return false; // Tolak jika shift malam tapi dokter tidak prefer shift malam (Constraint preferensi)
        }
    }
    
    return true; // Semua constraint terpenuhi, penugasan valid
}

int hitungShiftDokterMinggu(Shift jadwal[], Dokter* dokter, int minggu) {
    int count = 0;                                   
    for (int hari = minggu * 7; hari < (minggu + 1) * 7 && hari < 30; hari++) {  
        for (int shift = 0; shift < 3; shift++) {     
            int idx = hari * 3 + shift;               
            if (idx < TOTAL_SHIFT) { // Cek apakah index masih valid (tidak melebihi total shift)
                for (int j = 0; j < jadwal[idx].jumlah_dokter; j++) { // Loop semua dokter yang bertugas di shift ini
                    if (jadwal[idx].dokter_bertugas[j] == dokter) { // Cek apakah dokter yang dicari bertugas di shift ini
                        count++; // Jika iya, increment counter
                    }
                }
            }
        }
    }
    return count;                                    
}

int hitungShiftMalamDokterMinggu(Shift jadwal[], Dokter* dokter, int minggu) {
    int count = 0; 
    for (int hari = minggu * 7; hari < (minggu + 1) * 7 && hari < 30; hari++) { // Loop semua hari 
        int idx = hari * 3 + 2; // Hitung index shift malam (indeks 2 = shift malam)
        if (idx < TOTAL_SHIFT) { // Cek apakah index masih valid
            for (int j = 0; j < jadwal[idx].jumlah_dokter; j++) { // Loop semua dokter pada shift malam tersebut
                if (jadwal[idx].dokter_bertugas[j] == dokter) { // Cek apakah dokter yang dicari bertugas di shift malam ini
                    count++; // Jika ya, increment counter
                }
            }
        }
    }
    return count; 
}

bool validasiConstraintLengkap(Shift jadwal[], Dokter dokter[], int jumlah_dokter) {
    bool valid = true;
    
    printf("\n=== PENGECEKAN CONSTRAINT ===\n");
    
    int empty_shifts = 0;
    for (int i = 0; i < TOTAL_SHIFT; i++) {
        if (jadwal[i].jumlah_dokter == 0) {
            empty_shifts++;
            printf("PELANGGARAN KRITIS: Shift %s hari %d kosong\n", 
                   jadwal[i].tipe_shift, jadwal[i].hari_ke + 1);
            valid = false;
        }
    }
    if (empty_shifts == 0) {
        printf("(v) Semua shift terisi minimal 1 dokter\n");
    } else {
        printf("(x) %d shift kosong\n", empty_shifts);
    }
    
    int pelanggaran_shift_konsekutif = 0;
    for (int hari = 0; hari < 30; hari++) {
        for (int i = 0; i < jumlah_dokter; i++) {
            // Cek pagi+siang
            bool pagi = false, siang = false, malam = false;
            int idx_pagi = hari * 3;
            int idx_siang = hari * 3 + 1;
            int idx_malam = hari * 3 + 2;
            
            for (int j = 0; j < jadwal[idx_pagi].jumlah_dokter; j++) {
                if (jadwal[idx_pagi].dokter_bertugas[j] == &dokter[i]) {
                    pagi = true;
                    break;
                }
            }
            
            for (int j = 0; j < jadwal[idx_siang].jumlah_dokter; j++) {
                if (jadwal[idx_siang].dokter_bertugas[j] == &dokter[i]) {
                    siang = true;
                    break;
                }
            }
            
            for (int j = 0; j < jadwal[idx_malam].jumlah_dokter; j++) {
                if (jadwal[idx_malam].dokter_bertugas[j] == &dokter[i]) {
                    malam = true;
                    break;
                }
            }
            
            if ((pagi && siang) || (siang && malam)) {
                printf("PELANGGARAN: Dr. %s memiliki shift konsekutif pada hari %d\n", 
                       dokter[i].nama, hari + 1);
                pelanggaran_shift_konsekutif++;
                valid = false;
            }
        }
    }
    
    if (pelanggaran_shift_konsekutif == 0) {
        printf("(v) Tidak ada shift konsekutif dalam hari yang sama\n");
    } else {
        printf("(x) %d pelanggaran shift konsekutif\n", pelanggaran_shift_konsekutif);
    }
    
    int pelanggaran_preferensi = 0;
    for (int i = 0; i < TOTAL_SHIFT; i++) {
        for (int j = 0; j < jadwal[i].jumlah_dokter; j++) {
            Dokter* dok = jadwal[i].dokter_bertugas[j];
            if ((strcmp(jadwal[i].tipe_shift, "pagi") == 0 && dok->pref_pagi == 0) ||
                (strcmp(jadwal[i].tipe_shift, "siang") == 0 && dok->pref_siang == 0) ||
                (strcmp(jadwal[i].tipe_shift, "malam") == 0 && dok->pref_malam == 0)) {
                pelanggaran_preferensi++;
            }
        }
    }
    
    if (pelanggaran_preferensi == 0) {
        printf("(v) Semua preferensi dokter dihormati\n");
    } else {
        printf("(!) %d pelanggaran preferensi dokter\n", pelanggaran_preferensi);
    }
    
    if (valid) {
        printf("\nSemua constraint utama telah terpenuhi.\n");
    } else {
        printf("\nAda pelanggaran constraint utama. Awas!\n");
    }
    
    return valid;
} 