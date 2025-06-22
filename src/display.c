#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include "../include/types.h"
#include "../include/display.h"

int hitungJumlahShiftDokter(Shift jadwal[], Dokter *dokter) {
    int count = 0;
    
    for (int i = 0; i < TOTAL_SHIFT; i++) {
        for (int j = 0; j < jadwal[i].jumlah_dokter; j++) {
            if (jadwal[i].dokter_bertugas[j] == dokter) {
                count++;
                break; 
            }
        }
    }
    
    return count;
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

void tampilkanJumlahShiftDokter(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter) {
    if (jumlah_dokter <= 0) {
        printf("Tidak ada dokter untuk ditampilkan statistiknya.\n");
        return;
    }

    int *jumlah_shift = calloc(jumlah_dokter, sizeof(int));
    if (!jumlah_shift) {
        printf("Error: Gagal mengalokasikan memori.\n");
        return;
    }

    for (int i = 0; i < TOTAL_SHIFT; i++) {
        for (int j = 0; j < jadwal[i].jumlah_dokter; j++) {
            if (jadwal[i].dokter_bertugas[j] != NULL) {
                for (int k = 0; k < jumlah_dokter; k++) {
                    if (jadwal[i].dokter_bertugas[j] == &daftar_dokter[k]) {
                        jumlah_shift[k]++;
                        break;
                    }
                }
            }
        }
    }

    printf("\nJumlah Shift per Dokter:\n");
    printf("========================\n");
    for (int i = 0; i < jumlah_dokter; i++) {
        printf("%-25s: %d shift\n", daftar_dokter[i].nama, jumlah_shift[i]);
    }
    printf("\n");

    free(jumlah_shift);
}

void tampilkanPelanggaranPreferensiPerDokter(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter) {
    int *pelanggaran = calloc(jumlah_dokter, sizeof(int));
    if (!pelanggaran) {
        printf("Error: Gagal mengalokasikan memori.\n");
        return;
    }

    for (int i = 0; i < TOTAL_SHIFT; i++) {
        for (int j = 0; j < jadwal[i].jumlah_dokter; j++) {
            Dokter* d = jadwal[i].dokter_bertugas[j];
            if (d) {
                for (int k = 0; k < jumlah_dokter; k++) {
                    if (d == &daftar_dokter[k]) {
                        if ((strcmp(jadwal[i].tipe_shift, "Pagi") == 0 && d->pref_pagi == 0) ||
                            (strcmp(jadwal[i].tipe_shift, "pagi") == 0 && d->pref_pagi == 0) ||
                            (strcmp(jadwal[i].tipe_shift, "Siang") == 0 && d->pref_siang == 0) ||
                            (strcmp(jadwal[i].tipe_shift, "siang") == 0 && d->pref_siang == 0) ||
                            (strcmp(jadwal[i].tipe_shift, "Malam") == 0 && d->pref_malam == 0) ||
                            (strcmp(jadwal[i].tipe_shift, "malam") == 0 && d->pref_malam == 0)) {
                            pelanggaran[k]++;
                        }
                        break;
                    }
                }
            }
        }
    }

    printf("\nPelanggaran Preferensi per Dokter:\n");
    printf("==================================\n");
    for (int i = 0; i < jumlah_dokter; i++) {
        printf("%-25s: %d pelanggaran\n", daftar_dokter[i].nama, pelanggaran[i]);
    }
    printf("\n");

    free(pelanggaran);
}

void simpanJadwalKeCSV(Shift jadwal[], const char* nama_file) {
    FILE *f = fopen(nama_file, "w");
    if (!f) {
        printf("Error: Gagal buka file untuk tulis: %s\n", nama_file);
        return;
    }

    if (fprintf(f, "Hari,Shift,DoctorPosition,NamaDokter\n") < 0) {
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
    printf("Coefficient of variation: %.2f%%\n", (std_dev/avg_shifts)*100);

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
