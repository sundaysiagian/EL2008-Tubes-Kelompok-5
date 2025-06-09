#ifndef UTILS_H
#define UTILS_H

#include "types.h"

// File I/O functions
int read_config(const char* filename, Config* config);
int read_csv_line(char* line, char* fields[], int max_fields);
void trim_whitespace(char* str);

// String manipulation
int split_string(char* str, char delimiter, char* tokens[], int max_tokens);
void to_lowercase(char* str);

// Memory management
void free_doctor_array(Doctor* doctors, int count);
void free_schedule_array(ScheduleEntry* schedule, int count);

// Validation functions
int validate_time_format(const char* time);
int validate_day(const char* day);

#endif // UTILS_H