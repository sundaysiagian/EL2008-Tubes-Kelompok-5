#ifndef CONSTRAINTS_H
#define CONSTRAINTS_H

#include <stdbool.h>
#include "types.h"

bool apakahAlokasiValid(Shift jadwal[], Dokter* dokter, int index_shift, bool check_preferences);
int hitungShiftDokterMinggu(Shift jadwal[], Dokter* dokter, int minggu);
int hitungShiftMalamDokterMinggu(Shift jadwal[], Dokter* dokter, int minggu);
bool validasiConstraintLengkap(Shift jadwal[], Dokter dokter[], int jumlah_dokter);

#endif