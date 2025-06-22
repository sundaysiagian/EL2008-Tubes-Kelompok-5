#include <stdio.h>
#include <stdlib.h>
#include "../include/types.h"
#include "../include/doctor_manager.h"
#include "../include/scheduler.h"
#include "../include/utils.h"

// This file contains a simplified workflow test
// that mimics the end-to-end user experience

int main() {
    printf("=== MAIN APPLICATION WORKFLOW TEST ===\n\n");
    
    // 1. Initialize doctor list
    Dokter daftar_dokter[MAX_DOCTORS];
    int jumlah_dokter = 0;
    
    printf("1. Adding test doctors...\n");
    tambahDokter(daftar_dokter, &jumlah_dokter, "Dr. Smith", 7, 1, 1, 0);
    tambahDokter(daftar_dokter, &jumlah_dokter, "Dr. Johnson", 7, 1, 0, 1);
    tambahDokter(daftar_dokter, &jumlah_dokter, "Dr. Williams", 7, 0, 1, 1);
    tambahDokter(daftar_dokter, &jumlah_dokter, "Dr. Brown", 5, 1, 1, 1);
    tambahDokter(daftar_dokter, &jumlah_dokter, "Dr. Jones", 6, 1, 1, 0);
    
    printf("Doctor count: %d\n", jumlah_dokter);
    
    // 2. Create schedule
    Shift jadwal[TOTAL_SHIFT];
    printf("\n2. Creating automated schedule...\n");
    int result = buatJadwalOtomatis(jadwal, daftar_dokter, jumlah_dokter);
    
    if (result) {
        printf("Schedule created successfully!\n");
        
        // 3. Display some schedule information
        printf("\n3. Schedule statistics:\n");
        printf("Total shifts: %d\n", TOTAL_SHIFT);
        
        int shifts_filled = 0;
        for (int i = 0; i < TOTAL_SHIFT; i++) {
            if (jadwal[i].jumlah_dokter > 0) {
                shifts_filled++;
            }
        }
        printf("Shifts filled: %d (%.1f%%)\n", shifts_filled, (float)shifts_filled/TOTAL_SHIFT*100);
        
        // 4. Doctor shift counts
        printf("\n4. Doctor shift counts:\n");
        for (int i = 0; i < jumlah_dokter; i++) {
            int shift_count = 0;
            for (int j = 0; j < TOTAL_SHIFT; j++) {
                for (int k = 0; k < jadwal[j].jumlah_dokter; k++) {
                    if (jadwal[j].dokter_bertugas[k] == &daftar_dokter[i]) {
                        shift_count++;
                    }
                }
            }
            printf("%s: %d shifts\n", daftar_dokter[i].nama, shift_count);
        }
        
        printf("\nWorkflow test completed successfully!\n");
        return 0;
    } else {
        printf("Failed to create schedule!\n");
        return 1;
    }
}