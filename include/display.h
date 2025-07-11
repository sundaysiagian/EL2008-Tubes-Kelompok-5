#ifndef DISPLAY_H
#define DISPLAY_H

#include "types.h"

void tampilkanJadwalHarian(Shift jadwal[], int hari);
void tampilkanJadwalMingguan(Shift jadwal[], int minggu);
void tampilkanJadwalBulanan(Shift jadwal[]);
void tampilkanJumlahShiftDokter(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter);
void tampilkanDistribusiShift(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter);
void analisisKeadilan(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter);
int hitungPelanggaranPreferensi(Shift jadwal[]);
void simpanJadwalKeCSV(Shift jadwal[], const char* nama_file);
void tampilkanJadwalPerDokter(Shift jadwal[], Dokter *dokter);
int hitungJumlahShiftDokter(Shift jadwal[], Dokter *dokter); 
void validasiJadwal(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter);
void printLine(char character, int length);
void tampilkanPelanggaranPreferensiPerDokter(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter);

#endif