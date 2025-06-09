#ifndef TYPES_H
#define TYPES_H

// Doctor structure
typedef struct {
    char id_dokter[10];
    char nama_dokter[100];
    char spesialisasi[50];
    char hari[10];
    char jam_mulai[10];
    char jam_selesai[10];
    char ruangan[10];
    char status[10];
} Doctor;

// Schedule entry structure
typedef struct {
    char hari[10];
    char jam[10];
    char dokter[100];
    char spesialisasi[50];
    char ruangan[10];
} ScheduleEntry;

// Configuration structure
typedef struct {
    int max_doctors;
    int max_schedule_entries;
    char output_format[20];
} Config;

#endif // TYPES_H