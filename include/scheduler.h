#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "types.h"

// Schedule generation
int generate_schedule(const Doctor* doctors, int doctor_count, 
                     ScheduleEntry** schedule, int* schedule_count);
int create_weekly_schedule(const Doctor* doctors, int doctor_count,
                          ScheduleEntry** schedule, int* schedule_count);

// Schedule manipulation
int add_schedule_entry(ScheduleEntry** schedule, int* count, 
                      const ScheduleEntry* entry);
int remove_schedule_entry(ScheduleEntry* schedule, int* count, int index);

// Schedule validation and conflict resolution
int check_schedule_conflicts(const ScheduleEntry* schedule, int count);
int resolve_room_conflicts(ScheduleEntry* schedule, int count);
int validate_schedule_entry(const ScheduleEntry* entry);

// Schedule output
int save_schedule_to_csv(const ScheduleEntry* schedule, int count, 
                        const char* filename);
void print_schedule(const ScheduleEntry* schedule, int count);
void print_daily_schedule(const ScheduleEntry* schedule, int count, 
                         const char* day);

#endif // SCHEDULER_H