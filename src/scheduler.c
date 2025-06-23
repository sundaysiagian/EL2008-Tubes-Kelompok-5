/*
Program Penjadwalan Dokter
Fungsi:
1. Mengedit jadwal manual dokter
2. Membaca jadwal dari file CSV
3. Menyusun jadwal otomatis dengan memperhatikan constraint
4. Menginisialisasi dan menampilkan jadwal dokter
*/

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
#include "../include/constraints.h"

#define SCHEDULE_FILE "data/sample/jadwal_dokter.csv"

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
                if (apakahAlokasiValid(jadwal, &daftar_dokter[dok_idx], idx, true)) {
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

    bool solveJadwalWithConstraints(Shift jadwal[], int idx, int doctors_needed_per_shift, Dokter dokter[], int jumlah_dokter) {
    int doctor_loads[MAX_DOCTORS] = {0};

    for (int i = 0; i < jumlah_dokter; i++) {
        doctor_loads[i] = 0;
        for (int j = 0; j < idx; j++) {
            for (int k = 0; k < jadwal[j].jumlah_dokter; k++) {
                if (jadwal[j].dokter_bertugas[k] == &dokter[i]) {
                    doctor_loads[i]++;
                    break;
                }
            }
        }
    }
    // Base case - semua shift sudah terisi
    if (idx >= TOTAL_SHIFT) {
        return true;
    }
    
    if (jadwal[idx].jumlah_dokter >= doctors_needed_per_shift) {
        return solveJadwalWithConstraints(jadwal, idx + 1, doctors_needed_per_shift, dokter, jumlah_dokter);
    }
    
    for (int load = 0; load <= 15; load++) {
        for (int i = 0; i < jumlah_dokter; i++) {
            if (doctor_loads[i] == load && apakahAlokasiValid(jadwal, &dokter[i], idx, true)) {
                jadwal[idx].dokter_bertugas[jadwal[idx].jumlah_dokter] = &dokter[i];
                jadwal[idx].jumlah_dokter++;
                
                if (jadwal[idx].jumlah_dokter >= doctors_needed_per_shift) {
                    if (solveJadwalWithConstraints(jadwal, idx + 1, doctors_needed_per_shift, dokter, jumlah_dokter)) {
                        return true;
                    }
                } else {
                    if (solveJadwalWithConstraints(jadwal, idx, doctors_needed_per_shift, dokter, jumlah_dokter)) {
                        return true;
                    }
                }
                
                jadwal[idx].jumlah_dokter--;
                jadwal[idx].dokter_bertugas[jadwal[idx].jumlah_dokter] = NULL;
            }
        }
    }
    
    // Jika tidak bisa dengan preferensi, coba tanpa preferensi
    if (jadwal[idx].jumlah_dokter == 0) {
        for (int load = 0; load <= 15; load++) {
            for (int i = 0; i < jumlah_dokter; i++) {
                if (doctor_loads[i] == load && apakahAlokasiValid(jadwal, &dokter[i], idx, false)) {
                    printf("Mengabaikan preferensi untuk dokter %s pada shift %s hari %d\n",
                           dokter[i].nama, jadwal[idx].tipe_shift, jadwal[idx].hari_ke + 1);
                    
                    jadwal[idx].dokter_bertugas[jadwal[idx].jumlah_dokter] = &dokter[i];
                    jadwal[idx].jumlah_dokter++;
                    
                    if (solveJadwalWithConstraints(jadwal, idx, doctors_needed_per_shift, dokter, jumlah_dokter)) {
                        return true;
                    }
                    
                    jadwal[idx].jumlah_dokter--;
                    jadwal[idx].dokter_bertugas[jadwal[idx].jumlah_dokter] = NULL;
                }
            }
        }
    }
    
    // Jika shift masih kosong, lanjut ke shift berikutnya jika tidak kritis
    // Shift kosong mungkin tidak diizinkan tapi kita coba teruskan penjadwalan
    if (jadwal[idx].jumlah_dokter == 0) {
        printf("WARNING: Tidak dapat mengisi shift %s pada hari %d\n", 
               jadwal[idx].tipe_shift, jadwal[idx].hari_ke + 1);
        return solveJadwalWithConstraints(jadwal, idx + 1, doctors_needed_per_shift, dokter, jumlah_dokter);
    }
    
    // Jika shift memiliki dokter tapi kurang dari target
    if (jadwal[idx].jumlah_dokter < doctors_needed_per_shift && jadwal[idx].jumlah_dokter > 0) {
        return solveJadwalWithConstraints(jadwal, idx + 1, doctors_needed_per_shift, dokter, jumlah_dokter);
    }
    
    return false;
}

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

    // Analisis kelayakan constraint
    printf("\nAnalisis kelayakan constraint:\n");
    
    // Hitung total shift yang dibutuhkan
    int total_shift_needed = TOTAL_SHIFT * doctors_needed_per_shift;
    
    // Hitung kapasitas maksimal dokter (5 shift per minggu * 5 minggu)
    int max_capacity = jumlah_dokter * 25; // Asumsi 5 minggu
    
    printf("- Total slot shift dibutuhkan: %d\n", total_shift_needed);
    printf("- Kapasitas maksimal dokter: %d\n", max_capacity);
    
    if (total_shift_needed > max_capacity) {
        printf("TIDAK MUNGKIN: Kapasitas dokter tidak mencukupi!\n");
        printf("   Kurangi jumlah dokter per shift atau tambah dokter.\n");
        return 0;
    }
    
    // Analisis preferensi
    int morning_capable = 0, afternoon_capable = 0, night_capable = 0;
    for (int i = 0; i < jumlah_dokter; i++) {
        if (dokter[i].pref_pagi) morning_capable++;
        if (dokter[i].pref_siang) afternoon_capable++;
        if (dokter[i].pref_malam) night_capable++;
    }
    
    printf("- Dokter yang bisa shift pagi: %d (butuh %d shift)\n", morning_capable, 30 * doctors_needed_per_shift);
    printf("- Dokter yang bisa shift siang: %d (butuh %d shift)\n", afternoon_capable, 30 * doctors_needed_per_shift);
    printf("- Dokter yang bisa shift malam: %d (butuh %d shift)\n", night_capable, 30 * doctors_needed_per_shift);

    // Proses pembuatan jadwal dengan constraint
    printf("\nMemulai penjadwalan dengan constraint...\n");
    startProgress("Menyusun jadwal dengan constraint...");
    
    if (solveJadwalWithConstraints(jadwal, 0, doctors_needed_per_shift, dokter, jumlah_dokter)) {
        endProgress("Jadwal berhasil dibuat!");
        
        // Validasi constraint
        if (validasiConstraintLengkap(jadwal, dokter, jumlah_dokter)) {
            printf("\nPembuatan jadwal selesai.\n");
        } else {
            printf("\nPembuatan jadwal selesai.\n");
        }
        
        return 1;
    } else {
        endProgress("Gagal membuat jadwal!");
        printf("Tidak dapat membuat jadwal yang memenuhi semua constraint.\n");
        printf("Saran:\n");
        printf("- Kurangi jumlah dokter per shift\n");
        printf("- Tambah jumlah dokter\n");
        printf("- Relaxkan preferensi dokter\n");
        return 0;
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