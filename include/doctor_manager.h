#ifndef DOCTOR_MANAGER_H
#define DOCTOR_MANAGER_H

#include "types.h"

void tambahDokter(Dokter daftar[], int *jumlah, const char *nama, int maks_shift, int pref_pagi, int pref_siang, int pref_malam);
void hapusDokter(Dokter daftar[], int *jumlah, const char *nama);
void tampilkanDaftarDokter(Dokter daftar[], int jumlah);
int bacaDokterDariCSV(const char *filename, Dokter daftar[], int *jumlah);
int simpanDokterKeCSV(const char *filename, Dokter daftar[], int jumlah);

#endif