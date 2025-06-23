#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "types.h"

int getIntInput(int min, int max, const char* prompt);
void displayMenu();
void displayEnhancedMenu();
void doctorManagementMenu(Dokter daftar[], int *jumlah);
void statistikJadwalMenu(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter);
void inisialisasiJadwal(Shift jadwal[]);
int buatJadwalOtomatis(Shift jadwal[], Dokter dokter[], int jumlah_dokter);
int hitungShiftMingguIni(Shift jadwal[], Dokter* dokter, int minggu);
bool apakahAlokasiValid(Shift jadwal[], Dokter* dokter, int idx, bool check_preferences);
bool solveJadwalWithConstraints(Shift jadwal[], int idx, int doctors_needed_per_shift, Dokter dokter[], int jumlah_dokter);
int bacaJadwalDariCSV(Shift jadwal[], const char* nama_file, Dokter daftar_dokter[], int jumlah_dokter);
void editJadwalManual(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter);
void pencarian(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter);
void cariDokter(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter);
void cariTanggal(Shift jadwal[]);

#endif