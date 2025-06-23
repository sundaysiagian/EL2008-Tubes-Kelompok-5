#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

#define MAX_DOCTORS 50
#define MAX_DOCTORS_PER_SHIFT 20
#define TOTAL_SHIFT 90
#define MAX_CONSECUTIVE_DAYS 5
#define MIN_REST_HOURS 8
#define MIN_REST_AFTER_NIGHT 24

typedef struct {
    char nama[50];
    int maks_shift_mingguan;
    int pref_pagi;
    int pref_siang;
    int pref_malam;
} Dokter;

typedef struct {
    int hari_ke;
    char tipe_shift[10];
    Dokter* dokter_bertugas[MAX_DOCTORS_PER_SHIFT];
    int jumlah_dokter;
} Shift;

typedef struct {
    char nama[50];
    char filepath_dokter[100];
    char filepath_jadwal[100];
    int doctors_per_shift;
    int is_loaded;
} UserPreferences;

#endif