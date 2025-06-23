#ifndef SEARCH_H
#define SEARCH_H

#include "types.h"

void cariDokter(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter);
void cariTanggal(Shift jadwal[]);
void pencarian(Shift jadwal[], Dokter daftar_dokter[], int jumlah_dokter);

#endif