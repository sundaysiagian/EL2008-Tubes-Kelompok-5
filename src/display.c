#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include "../include/types.h"
#include "../include/display.h"

// hitung berapa kali dokter ditaro di jadwal
int hitungJumlahShiftDokter(Shift jadwal[], Dokter *target_dokter) {
    int jumlah_shift = 0;

    for (int i = 0; i < TOTAL_SHIFT; i++) {
        Shift shift = jadwal[i];
        
        for (int j = 0; j < shift.jumlah_dokter; j++) {
            // dari pointer
            if (shift.dokter_bertugas[j] == target_dokter) {
                jumlah_shift++;
                break; // dokter dah ada di shift, next shift
            }
        }
    }

    return jumlah_shift;
}


void tampilkanJadwalHarian(Shift jadwal[], int hari) {
    if (hari < 0 || hari >= 30) {
        printf("Error: Hari harus antara 1-30.\n");
        return;
    }

    printf("Jadwal Hari Ke-%d:\n", hari + 1);
    for (int i = 0; i < 3; i++) {
        int idx = hari * 3 + i;
        if (idx < 0 || idx >= TOTAL_SHIFT) {
            continue;
        }

        printf("  Shift %s:", jadwal[idx].tipe_shift);

        if (jadwal[idx].jumlah_dokter == 0) {
            printf(" KOSONG\n");
        } else {
            printf("\n");
            for (int j = 0; j < jadwal[idx].jumlah_dokter; j++) {
                printf("    - %s\n", jadwal[idx].dokter_bertugas[j] ?
                       jadwal[idx].dokter_bertugas[j]->nama : "KOSONG");
            }
        }
    }
    printf("\n");
}

// berapa kali tiap dokter ditugaskan kerja di semua shift
void tampilkanJumlahShiftDokter(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter) {
    // belum ada dokter, exit
    if (jumlah_dokter <= 0) {
        printf("Tidak ada dokter untuk ditampilkan statistiknya.\n");
        return;
    }

    // init array
    int *jumlah_shift = calloc(jumlah_dokter, sizeof(int));
    if (!jumlah_shift) {
        // if gagal
        printf("Error: Gagal mengalokasikan memori.\n");
        return;
    }

    // loop tiap shift
    for (int i = 0; i < TOTAL_SHIFT; i++) {
        // loop semua dokter di shift
        for (int j = 0; j < jadwal[i].jumlah_dokter; j++) {
            // pastikan ada dokter
            if (jadwal[i].dokter_bertugas[j] != NULL) {
                // cek dokternya dimana
                for (int k = 0; k < jumlah_dokter; k++) {
                    // bandingkan dengan pointer
                    if (jadwal[i].dokter_bertugas[j] == &daftar_dokter[k]) {
                        // tambah jumlah shift dia
                        jumlah_shift[k]++;
                        break; // break 
                    }
                }
            }
        }
    }

    // hasil hitung
    printf("\nJumlah Shift per Dokter:\n");
    printf("========================\n");
    for (int i = 0; i < jumlah_dokter; i++) {
        printf("%-25s: %d shift\n", daftar_dokter[i].nama, jumlah_shift[i]);
    }
    printf("\n");

    // free memori
    free(jumlah_shift);
}


// jumlah pelanggaran preferensi shift per dokter
void tampilkanPelanggaranPreferensiPerDokter(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter) {
    // alokasi memori
    int *pelanggaran = calloc(jumlah_dokter, sizeof(int));
    if (!pelanggaran) {
        printf("Error: Gagal mengalokasikan memori.\n");
        return;
    }

    // loop tiap shift
    for (int i = 0; i < TOTAL_SHIFT; i++) {
        // loop per dokter
        for (int j = 0; j < jadwal[i].jumlah_dokter; j++) {
            Dokter* d = jadwal[i].dokter_bertugas[j];
            
            // kalau ada dokter
            if (d) {
                // cari dokter
                for (int k = 0; k < jumlah_dokter; k++) {
                    if (d == &daftar_dokter[k]) {
                        // sesuaikan dgn preferensi

                        if ((strcmp(jadwal[i].tipe_shift, "Pagi") == 0 && d->pref_pagi == 0) ||
                            (strcmp(jadwal[i].tipe_shift, "pagi") == 0 && d->pref_pagi == 0) ||
                            (strcmp(jadwal[i].tipe_shift, "Siang") == 0 && d->pref_siang == 0) ||
                            (strcmp(jadwal[i].tipe_shift, "siang") == 0 && d->pref_siang == 0) ||
                            (strcmp(jadwal[i].tipe_shift, "Malam") == 0 && d->pref_malam == 0) ||
                            (strcmp(jadwal[i].tipe_shift, "malam") == 0 && d->pref_malam == 0)) {
                            pelanggaran[k]++;  // tambah pelanggaran
                        }
                        break; // next
                    }
                }
            }
        }
    }

    // output
    printf("\nPelanggaran Preferensi per Dokter:\n");
    printf("==================================\n");
    for (int i = 0; i < jumlah_dokter; i++) {
        printf("%-25s: %d pelanggaran\n", daftar_dokter[i].nama, pelanggaran[i]);
    }
    printf("\n");

    // free memori
    free(pelanggaran);
}


void simpanJadwalKeCSV(Shift jadwal[], const char* nama_file) {
    FILE *f = fopen(nama_file, "w");
    if (!f) {
        printf("Error: Gagal buka file untuk tulis: %s\n", nama_file);
        return;
    }

    if (fprintf(f, "Hari,Shift,JabatanDokter,NamaDokter\n") < 0) {
        printf("Error: Gagal menulis header ke file.\n");
        fclose(f);
        return;
    }

    int success = 1;
    for (int i = 0; i < TOTAL_SHIFT; i++) {
        if (jadwal[i].jumlah_dokter == 0) {
            if (fprintf(f, "%d,%s,1,KOSONG\n", jadwal[i].hari_ke + 1, jadwal[i].tipe_shift) < 0) {
                success = 0;
                break;
            }
        } else {
            for (int j = 0; j < jadwal[i].jumlah_dokter; j++) {
                const char* nama = jadwal[i].dokter_bertugas[j] ? jadwal[i].dokter_bertugas[j]->nama : "KOSONG";
                if (fprintf(f, "%d,%s,%d,%s\n", jadwal[i].hari_ke + 1, jadwal[i].tipe_shift, j+1, nama) < 0) {
                    success = 0;
                    break;
                }
            }
        }

        if (!success) {
            printf("Error: Gagal menulis data ke file pada index %d.\n", i);
            break;
        }
    }

    fclose(f);

    if (success) {
        printf("File %s berhasil disimpan.\n", nama_file);
    }
}

void tampilkanJadwalMingguan(Shift jadwal[], int minggu) {
    if (minggu < 0 || minggu >= 5) {
        printf("Error: Minggu harus antara 1-5.\n");
        return;
    }

    int start = minggu * 7;
    int end = start + 7;
    if (end > 30) end = 30;

    printf("Jadwal Minggu Ke-%d:\n", minggu + 1);
    for (int d = start; d < end; d++) {
        printf("Hari %d:\n", d + 1);
        for (int i = 0; i < 3; i++) {
            int idx = d * 3 + i;
            if (idx < 0 || idx >= TOTAL_SHIFT) {
                continue;
            }
            
            printf("  Shift %s:", jadwal[idx].tipe_shift);

            if (jadwal[idx].jumlah_dokter == 0) {
                printf(" KOSONG\n");
            } else {
                printf("\n");
                for (int j = 0; j < jadwal[idx].jumlah_dokter; j++) {
                    printf("    - %s\n", jadwal[idx].dokter_bertugas[j] ?
                           jadwal[idx].dokter_bertugas[j]->nama : "KOSONG");
                }
            }
        }
        printf("\n");
    }
}

void tampilkanJadwalBulanan(Shift jadwal[]) {
    for (int hari = 0; hari < 30; hari++) {
        printf("Hari %d:\n", hari + 1);
        for (int i = 0; i < 3; i++) {
            int idx = hari * 3 + i;
            if (idx < 0 || idx >= TOTAL_SHIFT) {
                continue;
            }
            
            printf("  Shift %s:", jadwal[idx].tipe_shift);

            if (jadwal[idx].jumlah_dokter == 0) {
                printf(" KOSONG\n");
            } else {
                printf("\n");
                for (int j = 0; j < jadwal[idx].jumlah_dokter; j++) {
                    printf("    - %s\n", jadwal[idx].dokter_bertugas[j] ?
                           jadwal[idx].dokter_bertugas[j]->nama : "KOSONG");
                }
            }
        }
        printf("\n");
    }
}

void tampilkanDistribusiShift(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter) {
    int **shift_counts = malloc(jumlah_dokter * sizeof(int*));
    if (!shift_counts) {
        printf("Error: Gagal mengalokasikan memori.\n");
        return;
    }
    
    for (int i = 0; i < jumlah_dokter; i++) {
        shift_counts[i] = calloc(3, sizeof(int));
        if (!shift_counts[i]) {
            printf("Error: Gagal mengalokasikan memori.\n");
    
            for (int j = 0; j < i; j++) {
                free(shift_counts[j]);
            }
            free(shift_counts);
            return;
        }
    }

    for (int i = 0; i < TOTAL_SHIFT; i++) {
        for (int k = 0; k < jadwal[i].jumlah_dokter; k++) {
            Dokter* d = jadwal[i].dokter_bertugas[k];
            if (d) {
                int dokter_idx = -1;
                for (int j = 0; j < jumlah_dokter; j++) {
                    if (strcmp(d->nama, daftar_dokter[j].nama) == 0) {
                        dokter_idx = j;
                        break;
                    }
                }
                
                if (dokter_idx >= 0) {
                    if (strcmp(jadwal[i].tipe_shift, "Pagi") == 0 || 
                        strcmp(jadwal[i].tipe_shift, "pagi") == 0) {
                        shift_counts[dokter_idx][0]++;
                    } else if (strcmp(jadwal[i].tipe_shift, "Siang") == 0 || 
                               strcmp(jadwal[i].tipe_shift, "siang") == 0) {
                        shift_counts[dokter_idx][1]++;
                    } else if (strcmp(jadwal[i].tipe_shift, "Malam") == 0 || 
                               strcmp(jadwal[i].tipe_shift, "malam") == 0) {
                        shift_counts[dokter_idx][2]++;
                    }
                }
            }
        }
    }

    printf("\nDistribusi Tipe Shift per Dokter:\n");
    printf("=================================\n");
    printf("%-20s | %-10s | %-10s | %-10s\n", "Nama", "Pagi", "Siang", "Malam");
    printf("----------------------------------------------------------------\n");

    for (int i = 0; i < jumlah_dokter; i++) {
        printf("%-20s | %-10d | %-10d | %-10d\n",
               daftar_dokter[i].nama,
               shift_counts[i][0],
               shift_counts[i][1],
               shift_counts[i][2]);
    }

    for (int i = 0; i < jumlah_dokter; i++) {
        free(shift_counts[i]);
    }
    free(shift_counts);
}

void analisisKeadilan(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter) {
    if (jumlah_dokter <= 0) {
        printf("Tidak ada dokter untuk dianalisis.\n");
        return;
    }
    
    double avg_shifts = 0;
    int *shifts_per_doctor = calloc(jumlah_dokter, sizeof(int));
    if (!shifts_per_doctor) {
        printf("Error: Gagal mengalokasikan memori.\n");
        return;
    }

    for (int i = 0; i < TOTAL_SHIFT; i++) {
        for (int k = 0; k < jadwal[i].jumlah_dokter; k++) {
            Dokter* d = jadwal[i].dokter_bertugas[k];
            if (d) {
                for (int j = 0; j < jumlah_dokter; j++) {
                    if (strcmp(d->nama, daftar_dokter[j].nama) == 0) {
                        shifts_per_doctor[j]++;
                        break;
                    }
                }
            }
        }
    }

    for (int i = 0; i < jumlah_dokter; i++) {
        avg_shifts += shifts_per_doctor[i];
    }
    avg_shifts /= jumlah_dokter;

    double variance = 0;
    for (int i = 0; i < jumlah_dokter; i++) {
        variance += (shifts_per_doctor[i] - avg_shifts) * (shifts_per_doctor[i] - avg_shifts);
    }
    variance /= jumlah_dokter;
    double std_dev = sqrt(variance);

    printf("\nAnalisis Keadilan Distribusi Shift:\n");
    printf("=================================\n");
    printf("Rata-rata shift per dokter: %.2f\n", avg_shifts);
    printf("Standar deviasi: %.2f\n", std_dev);
    printf("Koefisien Variansi: %.2f%%\n", (std_dev/avg_shifts)*100);

    if ((std_dev/avg_shifts)*100 < 10) {
        printf("Distribusi shift sangat merata\n");
    } else if ((std_dev/avg_shifts)*100 < 20) {
        printf("Distribusi shift cukup merata\n");
    } else {
        printf("Distribusi shift kurang merata\n");
    }

    free(shifts_per_doctor);
}

int hitungPelanggaranPreferensi(Shift jadwal[]) {
    int pelanggaran = 0;

    for (int i = 0; i < TOTAL_SHIFT; i++) {
        for (int j = 0; j < jadwal[i].jumlah_dokter; j++) {
            Dokter* d = jadwal[i].dokter_bertugas[j];
            if (d) {
                if ((strcmp(jadwal[i].tipe_shift, "Pagi") == 0 && d->pref_pagi == 0) ||
                    (strcmp(jadwal[i].tipe_shift, "pagi") == 0 && d->pref_pagi == 0) ||
                    (strcmp(jadwal[i].tipe_shift, "Siang") == 0 && d->pref_siang == 0) ||
                    (strcmp(jadwal[i].tipe_shift, "siang") == 0 && d->pref_siang == 0) ||
                    (strcmp(jadwal[i].tipe_shift, "Malam") == 0 && d->pref_malam == 0) ||
                    (strcmp(jadwal[i].tipe_shift, "malam") == 0 && d->pref_malam == 0)) {
                    pelanggaran++;
                }
            }
        }
    }

    return pelanggaran;
}

void validasiJadwal(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter) {
    printf("\n=== VALIDASI JADWAL ===\n");
    printLine('-', 50);
    
    int violations = 0;
    int errors = 0;
    
    // Cek 1: Memastikan tidak ada dokter yang melebihi batas maksimal shift per minggu
    printf("1. Memeriksa batas maksimal shift per minggu...\n");
    for (int minggu = 0; minggu < 5; minggu++) {
        for (int i = 0; i < jumlah_dokter; i++) {
            int shift_count = hitungShiftMingguIni(jadwal, &daftar_dokter[i], minggu);
            if (shift_count > daftar_dokter[i].maks_shift_mingguan) {
                printf("   ERROR: %s melebihi batas (Minggu %d: %d/%d shift)\n", 
                       daftar_dokter[i].nama, minggu + 1, shift_count, 
                       daftar_dokter[i].maks_shift_mingguan);
                errors++;
            }
        }
    }
    if (errors == 0) printf("   ✓ Semua dokter dalam batas maksimal shift\n");

    // Cek 2: Memeriksa pelanggaran preferensi  
    printf("2. Memeriksa pelanggaran preferensi...\n");
    for (int i = 0; i < TOTAL_SHIFT; i++) {
        for (int j = 0; j < jadwal[i].jumlah_dokter; j++) {
            Dokter* d = jadwal[i].dokter_bertugas[j];
            if (d) {
                if ((strcmp(jadwal[i].tipe_shift, "pagi") == 0 && d->pref_pagi == 0) ||
                    (strcmp(jadwal[i].tipe_shift, "siang") == 0 && d->pref_siang == 0) ||
                    (strcmp(jadwal[i].tipe_shift, "malam") == 0 && d->pref_malam == 0)) {
                    printf("   VIOLATION: %s ditempatkan di shift %s (Hari %d) tanpa preferensi\n",
                           d->nama, jadwal[i].tipe_shift, jadwal[i].hari_ke + 1);
                    violations++;
                }
            }
        }
    }
    if (violations == 0) printf("   ✓ Semua penempatan sesuai preferensi\n");
    
    // Cek 3: Memeriksa cakupan minimal per shift
    printf("3. Memeriksa cakupan minimal per shift...\n");
    int uncovered_shifts = 0;
    for (int i = 0; i < TOTAL_SHIFT; i++) {
        if (jadwal[i].jumlah_dokter == 0) {
            printf("   WARNING: Shift %s Hari %d tidak ada dokter\n",
                   jadwal[i].tipe_shift, jadwal[i].hari_ke + 1);
            uncovered_shifts++;
        }
    }
    if (uncovered_shifts == 0) printf("   ✓ Semua shift memiliki minimal 1 dokter\n");
    
    // Cek 4: Memeriksa double booking (dokter yang sama di beberapa shift pada hari yang sama)
    printf("4. Memeriksa konflik jadwal...\n");
    int conflicts = 0;
    for (int hari = 0; hari < 30; hari++) {
        for (int i = 0; i < jumlah_dokter; i++) {
            int shifts_today = 0;
            for (int shift = 0; shift < 3; shift++) {
                int idx = hari * 3 + shift;
                for (int j = 0; j < jadwal[idx].jumlah_dokter; j++) {
                    if (jadwal[idx].dokter_bertugas[j] == &daftar_dokter[i]) {
                        shifts_today++;
                    }
                }
            }
            if (shifts_today > 1) {
                printf("   CONFLICT: %s terjadwal %d shift di Hari %d\n",
                       daftar_dokter[i].nama, shifts_today, hari + 1);
                conflicts++;
            }
        }
    }
    if (conflicts == 0) printf("   ✓ Tidak ada konflik jadwal\n");
    
    // Ringkasan
    printf("\n=== RINGKASAN VALIDASI ===\n");
    printf("Errors (pelanggaran batas): %d\n", errors);
    printf("Violations (pelanggaran preferensi): %d\n", violations);
    printf("Conflicts (konflik jadwal): %d\n", conflicts);
    printf("Uncovered shifts: %d\n", uncovered_shifts);
    
    if (errors == 0 && conflicts == 0) {
        printf("STATUS: JADWAL VALID ✓\n");
    } else {
        printf("STATUS: JADWAL TIDAK VALID ✗\n");
    }
    
    if (violations > 0) {
        printf("CATATAN: Ada pelanggaran preferensi (dapat ditoleransi)\n");
    }
}
