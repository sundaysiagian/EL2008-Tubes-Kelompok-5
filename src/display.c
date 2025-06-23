/*
Program Display dan Analisis Jadwal Dokter
Fungsi:
1. Menampilkan jadwal (harian, mingguan, bulanan)
2. Menghitung dan menampilkan statistik shift dokter
3. Menganalisis pelanggaran preferensi dokter
4. Menganalisis distribusi dan keadilan shift
5. Validasi jadwal secara komprehensif
6. Menyimpan jadwal ke file CSV
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include "../include/types.h"
#include "../include/display.h"
#include "../include/scheduler.h"

int hitungJumlahShiftDokter(Shift jadwal[], Dokter *target_dokter) {
    int jumlah_shift = 0; // Reset counter jml shift

    for (int i = 0; i < TOTAL_SHIFT; i++) { // Loop semua shift yang ada (90 shift utk 30 hari)
        Shift shift = jadwal[i]; // Ambil data shift saat ini
        
        for (int j = 0; j < shift.jumlah_dokter; j++) { // Loop semua dokter yang bertugas di shift tersebut
            if (shift.dokter_bertugas[j] == target_dokter) { // Cari dokter 
                jumlah_shift++; 
                break; 
            }
        }
    }
    return jumlah_shift; // Tampilkan total shift dokter tersebut
}

void tampilkanJadwalHarian(Shift jadwal[], int hari) {
    if (hari < 0 || hari >= 30) { 
        printf("Error: Hari harus antara 1-30.\n");
        return;
    }

    printf("Jadwal Hari Ke-%d:\n", hari + 1); 
    for (int i = 0; i < 3; i++) { // Loop untuk 3 shift per hari (pagi, siang, malam)
        int idx = hari * 3 + i; // Hitung index shift dalam array jadwal 
        if (idx < 0 || idx >= TOTAL_SHIFT) { 
            continue;
        }

        printf("  Shift %s:", jadwal[idx].tipe_shift); // Tampilkan nama shift (pagi/siang/malam)

        if (jadwal[idx].jumlah_dokter == 0) { // Jika tidak ada dokter di shift ini
            printf(" KOSONG\n");
        } else { // Jika ada 
            printf("\n");
            for (int j = 0; j < jadwal[idx].jumlah_dokter; j++) { // Loop semua dokter yang bertugas di shift ini
                printf("    - %s\n", jadwal[idx].dokter_bertugas[j] ? 
                       jadwal[idx].dokter_bertugas[j]->nama : "KOSONG"); // Tampilkan nama dokter atau "KOSONG"
            }
        }
    }
    printf("\n"); 
}

void tampilkanJadwalMingguan(Shift jadwal[], int minggu) {
    if (minggu < 0 || minggu >= 5) { 
        printf("Error: Minggu harus antara 1-5.\n");
        return;
    }

    int start = minggu * 7; // Hitung hari MULAI minggu ini (minggu 0: hari 0-6, minggu 1: hari 7-13, dst)
    int end = start + 7; // Hitung hari SELESAI minggu ini
    if (end > 30) end = 30; 

    printf("Jadwal Minggu Ke-%d:\n", minggu + 1); 
    for (int d = start; d < end; d++) { // Loop semua hari dalam minggu ini, tampilkan nomor hari
        printf("Hari %d:\n", d + 1); 
        for (int i = 0; i < 3; i++) { // Loop untuk 3 shift per hari (pagi, siang, malam)
            int idx = d * 3 + i; // Hitung index shift dalam array jadwal
            if (idx < 0 || idx >= TOTAL_SHIFT) { // Lewati index jika invalid
                continue; 
            }
            
            printf("  Shift %s:", jadwal[idx].tipe_shift); // Tampilkan nama shift

            if (jadwal[idx].jumlah_dokter == 0) { // Jika tidak ada dokter di shift ini
                printf(" KOSONG\n");
            } else { // Jika ada dokter
                printf("\n");
                for (int j = 0; j < jadwal[idx].jumlah_dokter; j++) { // Loop semua dokter di shift ini
                    printf("    - %s\n", jadwal[idx].dokter_bertugas[j] ?
                           jadwal[idx].dokter_bertugas[j]->nama : "KOSONG"); // Tampilkan nama dokter atau "KOSONG"
                }
            }
        }
        printf("\n"); 
    }
}

void tampilkanJadwalBulanan(Shift jadwal[]) {
    for (int hari = 0; hari < 30; hari++) { // Loop semua hari dalam sebulan (30 hari), lalu tampilkan nomor hari
        printf("Hari %d:\n", hari + 1); 
        for (int i = 0; i < 3; i++) { // Loop untuk 3 shift per hari (pagi, siang, malam)
            int idx = hari * 3 + i; // Hitung index shift dalam array jadwal
            if (idx < 0 || idx >= TOTAL_SHIFT) { // Lewati index jika invalid
                continue; 
            }
            
            printf("  Shift %s:", jadwal[idx].tipe_shift); // Tampilkan nama shift

            if (jadwal[idx].jumlah_dokter == 0) { // Jika tidak ada dokter di shift ini
                printf(" KOSONG\n");
            } else { // Jika ada dokter
                printf("\n");
                for (int j = 0; j < jadwal[idx].jumlah_dokter; j++) { // Loop semua dokter di shift ini
                    printf("    - %s\n", jadwal[idx].dokter_bertugas[j] ?
                           jadwal[idx].dokter_bertugas[j]->nama : "KOSONG"); // Tampilkan nama dokter atau "KOSONG"
                }
            }
        }
        printf("\n"); 
    }
}

void tampilkanJumlahShiftDokter(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter) {
    // Cek apakah ada dokter yang terdaftar
    if (jumlah_dokter <= 0) {
        printf("Tidak ada dokter untuk ditampilkan statistiknya.\n");
        return;
    }

    // Alokasi memori untuk array counter shift tiap dokter
    int *jumlah_shift = calloc(jumlah_dokter, sizeof(int));
    if (!jumlah_shift) { 
        printf("Error: Gagal mengalokasikan memori.\n");
        return;
    }

    
    for (int i = 0; i < TOTAL_SHIFT; i++) { // Periksa semua shift yang ada, kemudian periksa semua dokter di shift tsb
        for (int j = 0; j < jadwal[i].jumlah_dokter; j++) { 
            if (jadwal[i].dokter_bertugas[j] != NULL) { 
                for (int k = 0; k < jumlah_dokter; k++) { // Cari dokter, cek apakah sama dengan yang di daftar
                    if (jadwal[i].dokter_bertugas[j] == &daftar_dokter[k]) { 
                        jumlah_shift[k]++; // Tambah jumlah shift utk dokter ini
                        break; 
                    }
                }
            }
        }
    }

    // Display hasil 
    printf("\nJumlah Shift per Dokter:\n");
    printf("========================\n");
    for (int i = 0; i < jumlah_dokter; i++) { // Loop semua dokter
        printf("%-25s: %d shift\n", daftar_dokter[i].nama, jumlah_shift[i]);
    }
    printf("\n");
    free(jumlah_shift);
}

int hitungPelanggaranPreferensi(Shift jadwal[]) {
    int pelanggaran = 0; 

    for (int i = 0; i < TOTAL_SHIFT; i++) { // Loop semua shift 
        for (int j = 0; j < jadwal[i].jumlah_dokter; j++) { // Loop semua dokter 
            Dokter* d = jadwal[i].dokter_bertugas[j]; // Ambil pointer dokter
            if (d) { 
                // Cek apakah ada dokter yang dilanggar preferensinya
                if ((strcmp(jadwal[i].tipe_shift, "Pagi") == 0 && d->pref_pagi == 0) ||
                    (strcmp(jadwal[i].tipe_shift, "pagi") == 0 && d->pref_pagi == 0) ||
                    (strcmp(jadwal[i].tipe_shift, "Siang") == 0 && d->pref_siang == 0) ||
                    (strcmp(jadwal[i].tipe_shift, "siang") == 0 && d->pref_siang == 0) ||
                    (strcmp(jadwal[i].tipe_shift, "Malam") == 0 && d->pref_malam == 0) ||
                    (strcmp(jadwal[i].tipe_shift, "malam") == 0 && d->pref_malam == 0)) {
                    pelanggaran++; // Increment jika ada 
                }
            }
        }
    }

    return pelanggaran; 
}

void tampilkanPelanggaranPreferensiPerDokter(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter) {
    int *pelanggaran = calloc(jumlah_dokter, sizeof(int)); // Allocate memori
    if (!pelanggaran) { 
        printf("Error: Gagal mengalokasikan memori.\n");
        return;
    }

    for (int i = 0; i < TOTAL_SHIFT; i++) { // Periksa semua shift yang ada
        for (int j = 0; j < jadwal[i].jumlah_dokter; j++) { // Periksa semua dokter di shift ini
            Dokter* d = jadwal[i].dokter_bertugas[j]; // Ambil pointer dokter, pastikan tdk NULL
            if (d) {
                for (int k = 0; k < jumlah_dokter; k++) { // Cari dokter di daftar, bandingkan pointer
                    if (d == &daftar_dokter[k]) { 
                        // Cek apakah penempatan shift melanggar preferensi si dokter
                        if ((strcmp(jadwal[i].tipe_shift, "Pagi") == 0 && d->pref_pagi == 0) ||
                            (strcmp(jadwal[i].tipe_shift, "pagi") == 0 && d->pref_pagi == 0) ||
                            (strcmp(jadwal[i].tipe_shift, "Siang") == 0 && d->pref_siang == 0) ||
                            (strcmp(jadwal[i].tipe_shift, "siang") == 0 && d->pref_siang == 0) ||
                            (strcmp(jadwal[i].tipe_shift, "Malam") == 0 && d->pref_malam == 0) ||
                            (strcmp(jadwal[i].tipe_shift, "malam") == 0 && d->pref_malam == 0)) {
                            pelanggaran[k]++; // Increment counter tiap preferensi yg dilanggar
                        }
                        break; 
                    }
                }
            }
        }
    }

    // Display hasil
    printf("\nPelanggaran Preferensi per Dokter:\n");
    printf("==================================\n");
    for (int i = 0; i < jumlah_dokter; i++) { // Loop semua dokter
        printf("%-25s: %d pelanggaran\n", daftar_dokter[i].nama, pelanggaran[i]);
    }
    printf("\n");
    free(pelanggaran);
}

void tampilkanDistribusiShift(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter) {
    int **shift_counts = malloc(jumlah_dokter * sizeof(int*)); // Alokasi array 2D untuk hitung shift tiap dokter
    if (!shift_counts) { // Cek apakah alokasi berhasil
        printf("Error: Gagal mengalokasikan memori.\n");
        return;
    }
    
    for (int i = 0; i < jumlah_dokter; i++) { // Alokasi array untuk setiap dokter (pagi, siang, malam)
        shift_counts[i] = calloc(3, sizeof(int)); // Index 0=pagi, 1=siang, 2=malam
        if (!shift_counts[i]) { // Cek apakah alokasi berhasil
            printf("Error: Gagal mengalokasikan memori.\n");
    
            for (int j = 0; j < i; j++) { // Bebaskan memori yang sudah dialokasi sebelumnya
                free(shift_counts[j]);
            }
            free(shift_counts);
            return;
        }
    }

    for (int i = 0; i < TOTAL_SHIFT; i++) { // Loop semua shift yang ada
        for (int k = 0; k < jadwal[i].jumlah_dokter; k++) { // Loop semua dokter di shift ini
            Dokter* d = jadwal[i].dokter_bertugas[k]; // Ambil pointer dokter
            if (d) { // Pastikan pointer tidak NULL
                int dokter_idx = -1; // Cari index dokter di daftar master
                for (int j = 0; j < jumlah_dokter; j++) { // Loop daftar dokter untuk mencari yang cocok
                    if (strcmp(d->nama, daftar_dokter[j].nama) == 0) { // Bandingkan nama dokter
                        dokter_idx = j; // Simpan index jika ketemu
                        break;
                    }
                }
                
                if (dokter_idx >= 0) { // Jika dokter ditemukan di daftar
                    if (strcmp(jadwal[i].tipe_shift, "Pagi") == 0 || 
                        strcmp(jadwal[i].tipe_shift, "pagi") == 0) { // Cek apakah shift pagi
                        shift_counts[dokter_idx][0]++; // Tambah counter shift pagi
                    } else if (strcmp(jadwal[i].tipe_shift, "Siang") == 0 || 
                               strcmp(jadwal[i].tipe_shift, "siang") == 0) { // Cek apakah shift siang
                        shift_counts[dokter_idx][1]++; // Tambah counter shift siang
                    } else if (strcmp(jadwal[i].tipe_shift, "Malam") == 0 || 
                               strcmp(jadwal[i].tipe_shift, "malam") == 0) { // Cek apakah shift malam
                        shift_counts[dokter_idx][2]++; // Tambah counter shift malam
                    }
                }
            }
        }
    }

    // Tampilkan hasil dalam bentuk tabel
    printf("\nDistribusi Tipe Shift per Dokter:\n");
    printf("=================================\n");
    printf("%-20s | %-10s | %-10s | %-10s\n", "Nama", "Pagi", "Siang", "Malam"); 
    printf("----------------------------------------------------------------\n");

    for (int i = 0; i < jumlah_dokter; i++) { // Loop semua dokter untuk tampilkan hasil
        printf("%-20s | %-10d | %-10d | %-10d\n",
               daftar_dokter[i].nama,
               shift_counts[i][0], // Jumlah shift pagi
               shift_counts[i][1], // Jumlah shift siang
               shift_counts[i][2]); // Jumlah shift malam
    }

    for (int i = 0; i < jumlah_dokter; i++) { // Bebaskan memori yang dialokasi
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
    int *shifts_per_doctor = calloc(jumlah_dokter, sizeof(int)); // Alokasi array untuk hitung shift tiap dokter
    if (!shifts_per_doctor) { 
        printf("Error: Gagal mengalokasikan memori.\n");
        return;
    }

    for (int i = 0; i < TOTAL_SHIFT; i++) { // Loop semua shift yang ada
        for (int k = 0; k < jadwal[i].jumlah_dokter; k++) { // Loop semua dokter di shift ini
            Dokter* d = jadwal[i].dokter_bertugas[k]; 
            if (d) { 
                for (int j = 0; j < jumlah_dokter; j++) { // Cari dokter di daftar master
                    if (strcmp(d->nama, daftar_dokter[j].nama) == 0) { 
                        shifts_per_doctor[j]++; // Tambah counter jika ditemukan
                        break; 
                    }
                }
            }
        }
    }

    // Hitung standar deviasi
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

    // Hasil analisis 
    printf("\nAnalisis Keadilan Distribusi Shift:\n");
    printf("=================================\n");
    printf("Rata-rata shift per dokter: %.2f\n", avg_shifts);
    printf("Standar deviasi: %.2f\n", std_dev);
    printf("Koefisien Variansi: %.2f%%\n", (std_dev/avg_shifts)*100); // Persentase variasi relatif

    if ((std_dev/avg_shifts)*100 < 10) { // Jika koefisien variansi < 10%
        printf("Distribusi shift sangat merata\n");
    } else if ((std_dev/avg_shifts)*100 < 20) { // Jika koefisien variansi 10-20%
        printf("Distribusi shift cukup merata\n");
    } else { // Jika koefisien variansi > 20%
        printf("Distribusi shift kurang merata\n");
    }

    free(shifts_per_doctor); // Bebaskan memori yang dialokasi
}

void simpanJadwalKeCSV(Shift jadwal[], const char* nama_file) {
    FILE *f = fopen(nama_file, "w"); // BUka file mode write
    if (!f) { 
        printf("Error: Gagal buka file untuk tulis: %s\n", nama_file);
        return;
    }

    // Tulis header CSV di baris pertama
    if (fprintf(f, "Hari,Shift,JabatanDokter,NamaDokter\n") < 0) { 
        printf("Error: Gagal menulis header ke file.\n");
        fclose(f);
        return;
    }

    int success = 1; 
    for (int i = 0; i < TOTAL_SHIFT; i++) { // Loop untuk menulis data setiap shift
        if (jadwal[i].jumlah_dokter == 0) { // Jika shift kosong (tidak ada dokter)
            if (fprintf(f, "%d,%s,1,KOSONG\n", jadwal[i].hari_ke + 1, jadwal[i].tipe_shift) < 0) {
                success = 0; 
                break;
            }
        } else { // Jika shift tdk kosong (ada dokter)
            for (int j = 0; j < jadwal[i].jumlah_dokter; j++) { 
                // Ambil nama dokter atau "KOSONG" jika pointer NULL
                const char* nama = jadwal[i].dokter_bertugas[j] ? jadwal[i].dokter_bertugas[j]->nama : "KOSONG";
                // Tulis data: hari, tipe shift, posisi dokter, nama dokter
                if (fprintf(f, "%d,%s,%d,%s\n", jadwal[i].hari_ke + 1, jadwal[i].tipe_shift, j+1, nama) < 0) {
                    success = 0;
                    break;
                }
            }
        }

        if (!success) { // Jika ada error saat menulis
            printf("Error: Gagal menulis data ke file pada index %d.\n", i);
            break; 
        }
    }

    fclose(f); 

    if (success) { 
        printf("File %s berhasil disimpan.\n", nama_file);
    }
}

void validasiJadwal(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter) {
    printf("\n=== VALIDASI JADWAL ===\n");
    printLine('-', 50); // Separator
    
    int violations = 0; // Counter pelanggaran preferensi
    int errors = 0; // Counter error batas shift
    
    // Cek 1: Memastikan tidak ada dokter yang melebihi batas maksimal shift per minggu
    printf("1. Memeriksa batas maksimal shift per minggu...\n");
    for (int minggu = 0; minggu < 5; minggu++) { // Loop minggu dlm sebulan
        for (int i = 0; i < jumlah_dokter; i++) { // Loop semua dokter
            int shift_count = hitungShiftMingguIni(jadwal, &daftar_dokter[i], minggu); // Hitung shift dokter di minggu ini
            if (shift_count > daftar_dokter[i].maks_shift_mingguan) { // Cek apakah melebihi batas
                printf("   ERROR: %s melebihi batas (Minggu %d: %d/%d shift)\n", 
                       daftar_dokter[i].nama, minggu + 1, shift_count, 
                       daftar_dokter[i].maks_shift_mingguan);
                errors++; // Tambah counter error
            }
        }
    }
    if (errors == 0) printf("   ✓ Semua dokter dalam batas maksimal shift\n"); // Jika no errors

    // Cek 2: Memeriksa pelanggaran preferensi  
    printf("2. Memeriksa pelanggaran preferensi...\n");
    for (int i = 0; i < TOTAL_SHIFT; i++) { // Loop semua shift
        for (int j = 0; j < jadwal[i].jumlah_dokter; j++) { // Loop semua dokter 
            Dokter* d = jadwal[i].dokter_bertugas[j]; // Ambil pointer dokter
            if (d) { 
                // Cek apakah dokter ditempatkan di shift yang tidak sesuai preferensinya
                if ((strcmp(jadwal[i].tipe_shift, "pagi") == 0 && d->pref_pagi == 0) ||
                    (strcmp(jadwal[i].tipe_shift, "siang") == 0 && d->pref_siang == 0) ||
                    (strcmp(jadwal[i].tipe_shift, "malam") == 0 && d->pref_malam == 0)) {
                    printf("   VIOLATION: %s ditempatkan di shift %s (Hari %d) tanpa preferensi\n",
                           d->nama, jadwal[i].tipe_shift, jadwal[i].hari_ke + 1);
                    violations++; // Tambah counter pelanggaran
                }
            }
        }
    }
    if (violations == 0) printf("   ✓ Semua penempatan sesuai preferensi\n"); // Jika no errors 
    
    // Cek 3: Memeriksa cakupan minimal per shift
    printf("3. Memeriksa cakupan minimal per shift...\n");
    int uncovered_shifts = 0; // Counter shift yang kosong
    for (int i = 0; i < TOTAL_SHIFT; i++) { // Loop semua shift
        if (jadwal[i].jumlah_dokter == 0) { // Cek apakah ada shift kosong
            printf("   WARNING: Shift %s Hari %d tidak ada dokter\n",
                   jadwal[i].tipe_shift, jadwal[i].hari_ke + 1);
            uncovered_shifts++; // Tambah counter shift kosong
        }
    }
    if (uncovered_shifts == 0) printf("   ✓ Semua shift memiliki minimal 1 dokter\n"); // Tampilkan pesan sukses jika semua shift terisi
    
    // Cek 4: Memeriksa double booking (dokter yang sama di beberapa shift pada hari yang sama)
    printf("4. Memeriksa konflik jadwal...\n");
    int conflicts = 0; // Counter konflik jadwal
    for (int hari = 0; hari < 30; hari++) { // Loop semua hari
        for (int i = 0; i < jumlah_dokter; i++) { // Loop semua dokter
            int shifts_today = 0; // Counter shift dokter di hari ini
            for (int shift = 0; shift < 3; shift++) { // Loop 3 shift per hari
                int idx = hari * 3 + shift; // Hitung index shift
                for (int j = 0; j < jadwal[idx].jumlah_dokter; j++) { // Loop semua dokter di shift ini
                    if (jadwal[idx].dokter_bertugas[j] == &daftar_dokter[i]) { // Cek apakah dokter sama
                        shifts_today++; // Tambah counter 
                    }
                }
            }
            if (shifts_today > 1) { // Jika dokter punya lebih dari 1 shift di hari yang sama
                printf("   CONFLICT: %s terjadwal %d shift di Hari %d\n",
                       daftar_dokter[i].nama, shifts_today, hari + 1);
                conflicts++; // Tambah counter konflik
            }
        }
    }
    if (conflicts == 0) printf("   ✓ Tidak ada konflik jadwal\n"); // Jika no errors
    
    // Tampilkan ringkasan 
    printf("\n=== RINGKASAN VALIDASI ===\n");
    printf("Errors (pelanggaran batas): %d\n", errors);
    printf("Violations (pelanggaran preferensi): %d\n", violations);
    printf("Conflicts (konflik jadwal): %d\n", conflicts);
    printf("Uncovered shifts: %d\n", uncovered_shifts);
    
    if (errors == 0 && conflicts == 0) { // Jika tidak ada error dan konflik
        printf("STATUS: JADWAL VALID ✓\n");
    } else { // Jika ada error atau konflik
        printf("STATUS: JADWAL TIDAK VALID ✗\n");
    }
    
    if (violations > 0) { // Jika ada pelanggaran preferensi (tp msh bisa ditoleransi)
        printf("CATATAN: Ada pelanggaran preferensi (dapat ditoleransi)\n");
    }
}
