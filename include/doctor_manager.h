#ifndef DOCTOR_MANAGER_H
#define DOCTOR_MANAGER_H

#include "types.h"

// Doctor loading and management
int load_doctors_from_csv(const char* filename, Doctor** doctors, int* count);
void print_doctor_info(const Doctor* doctor);
void print_all_doctors(const Doctor* doctors, int count);

// Doctor search and filter functions
Doctor* find_doctor_by_id(const Doctor* doctors, int count, const char* id);
int filter_doctors_by_specialty(const Doctor* doctors, int count, 
                                const char* specialty, Doctor** filtered);
int filter_doctors_by_day(const Doctor* doctors, int count, 
                         const char* day, Doctor** filtered);

// Doctor validation
int validate_doctor_data(const Doctor* doctor);
int is_doctor_available(const Doctor* doctor, const char* day, const char* time);

#endif // DOCTOR_MANAGER_H