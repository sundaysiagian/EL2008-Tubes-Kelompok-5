#include "unity/unity.h"
#include "../include/scheduler.h"
#include "../include/doctor_manager.h"
#include "../include/types.h"
#include <string.h>

void test_scheduler_functionality(void) {
    // Setup test data
    Shift jadwal[TOTAL_SHIFT];
    Dokter daftar_dokter[MAX_DOCTORS];
    int jumlah_dokter = 0;
    
    // Initialize some test doctors
    tambahDokter(daftar_dokter, &jumlah_dokter, "Dr. One", 7, 1, 1, 1);
    tambahDokter(daftar_dokter, &jumlah_dokter, "Dr. Two", 7, 1, 1, 1);
    tambahDokter(daftar_dokter, &jumlah_dokter, "Dr. Three", 7, 1, 1, 1);
    
    // Test jadwal initialization
    inisialisasiJadwal(jadwal);
    TEST_ASSERT_EQUAL(0, jadwal[0].jumlah_dokter);
    TEST_ASSERT_EQUAL(0, jadwal[0].hari_ke);
    TEST_ASSERT_EQUAL_STRING("pagi", jadwal[0].tipe_shift);
    
    // Test allocation validity
    TEST_ASSERT_EQUAL(1, apakahAlokasiValid(jadwal, &daftar_dokter[0], 0));
    
    // Add more comprehensive tests here
    printf("Scheduler tests completed\n");
}