#ifndef DISPLAY_H
#define DISPLAY_H

#include "types.h"

// Menu and user interface
void show_main_menu(void);
void show_doctor_menu(void);
void show_schedule_menu(void);
int get_user_choice(int min, int max);

// Display formatting
void print_header(const char* title);
void print_separator(void);
void print_table_header_doctors(void);
void print_table_header_schedule(void);

// Interactive functions
void display_doctor_selection(const Doctor* doctors, int count);
void display_schedule_by_day(const ScheduleEntry* schedule, int count);
void display_schedule_by_doctor(const ScheduleEntry* schedule, int count, 
                               const char* doctor_name);

// Error and status messages
void print_error(const char* message);
void print_success(const char* message);
void print_warning(const char* message);

#endif // DISPLAY_H