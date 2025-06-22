#include "unity/unity.h"
#include "../include/doctor_manager.h"
#include "../include/types.h"
#include <string.h>

void test_doctor_manager(void) {
    // Setup test data
    Dokter daftar_dokter[MAX_DOCTORS];
    int jumlah_dokter = 0;
    
    // Test 1: Adding a doctor
    tambahDokter(daftar_dokter, &jumlah_dokter, "Dr. Test", 5, 1, 1, 0);
    TEST_ASSERT_EQUAL(1, jumlah_dokter);
    TEST_ASSERT_EQUAL_STRING("Dr. Test", daftar_dokter[0].nama);
    TEST_ASSERT_EQUAL(5, daftar_dokter[0].maks_shift_mingguan);
    TEST_ASSERT_EQUAL(1, daftar_dokter[0].pref_pagi);
    TEST_ASSERT_EQUAL(1, daftar_dokter[0].pref_siang);
    TEST_ASSERT_EQUAL(0, daftar_dokter[0].pref_malam);
    
    // Test 2: Adding a second doctor
    tambahDokter(daftar_dokter, &jumlah_dokter, "Dr. Sample", 7, 0, 1, 1);
    TEST_ASSERT_EQUAL(2, jumlah_dokter);
    TEST_ASSERT_EQUAL_STRING("Dr. Sample", daftar_dokter[1].nama);
    
    printf("Doctor manager tests completed\n");
}