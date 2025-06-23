// test/test_scheduler.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "../include/types.h"
#include "../include/doctor_manager.h"
#include "../include/scheduler.h"
#include "../include/display.h"
#include "../include/utils.h"

// Variabel global untuk test
Dokter test_doctors[MAX_DOCTORS];
Shift test_shifts[TOTAL_SHIFT];
int test_doctor_count = 0;

// Setup dan teardown
void setUp(void) {
    // Inisialisasi sebelum setiap test
    test_doctor_count = 0;
    memset(test_doctors, 0, sizeof(test_doctors));
    inisialisasiJadwal(test_shifts);
}

void tearDown(void) {
    // Bersihkan setelah test
}

// Fungsi bantuan untuk tambah dokter test
void add_test_doctors(void) {
    tambahDokter(test_doctors, &test_doctor_count, "Dr. John", 5, 1, 1, 0);
    tambahDokter(test_doctors, &test_doctor_count, "Dr. Smith", 6, 1, 0, 1);
    tambahDokter(test_doctors, &test_doctor_count, "Dr. Davis", 7, 0, 1, 1);
}

// Test fungsi manajemen dokter
void test_add_doctor(void) {
    tambahDokter(test_doctors, &test_doctor_count, "Dr. Test", 5, 1, 1, 0);
    TEST_ASSERT_EQUAL_INT(1, test_doctor_count);
    TEST_ASSERT_EQUAL_STRING("Dr. Test", test_doctors[0].nama);
    TEST_ASSERT_EQUAL_INT(5, test_doctors[0].maks_shift_mingguan);
    TEST_ASSERT_EQUAL_INT(1, test_doctors[0].pref_pagi);
    TEST_ASSERT_EQUAL_INT(1, test_doctors[0].pref_siang);
    TEST_ASSERT_EQUAL_INT(0, test_doctors[0].pref_malam);
}

void test_add_multiple_doctors(void) {
    add_test_doctors();
    TEST_ASSERT_EQUAL_INT(3, test_doctor_count);
    TEST_ASSERT_EQUAL_STRING("Dr. John", test_doctors[0].nama);
    TEST_ASSERT_EQUAL_STRING("Dr. Smith", test_doctors[1].nama);
    TEST_ASSERT_EQUAL_STRING("Dr. Davis", test_doctors[2].nama);
}

void test_remove_doctor(void) {
    add_test_doctors();
    hapusDokter(test_doctors, &test_doctor_count, "Dr. Smith");
    TEST_ASSERT_EQUAL_INT(2, test_doctor_count);
    TEST_ASSERT_EQUAL_STRING("Dr. John", test_doctors[0].nama);
    TEST_ASSERT_EQUAL_STRING("Dr. Davis", test_doctors[1].nama);
}

void test_remove_nonexistent_doctor(void) {
    add_test_doctors();
    int initial_count = test_doctor_count;
    hapusDokter(test_doctors, &test_doctor_count, "Dr. Nonexistent");
    TEST_ASSERT_EQUAL_INT(initial_count, test_doctor_count);
}

// Test fungsi scheduler
void test_initialize_schedule(void) {
    inisialisasiJadwal(test_shifts);
    
    // Cek beberapa shift acak untuk pastikan inisialisasi benar
    TEST_ASSERT_EQUAL_INT(0, test_shifts[0].hari_ke);
    TEST_ASSERT_EQUAL_STRING("pagi", test_shifts[0].tipe_shift);
    TEST_ASSERT_EQUAL_INT(0, test_shifts[0].jumlah_dokter);
    
    TEST_ASSERT_EQUAL_INT(15, test_shifts[45].hari_ke);
    TEST_ASSERT_EQUAL_STRING("malam", test_shifts[47].tipe_shift);
    TEST_ASSERT_EQUAL_INT(0, test_shifts[47].jumlah_dokter);
    
    TEST_ASSERT_EQUAL_INT(29, test_shifts[89].hari_ke);
    TEST_ASSERT_EQUAL_STRING("malam", test_shifts[89].tipe_shift);
    TEST_ASSERT_EQUAL_INT(0, test_shifts[89].jumlah_dokter);
}

void test_allocation_validity(void) {
    add_test_doctors();
    inisialisasiJadwal(test_shifts);
    
    // Test alokasi valid
    TEST_ASSERT_TRUE(apakahAlokasiValid(test_shifts, &test_doctors[0], 0));
    
    // Tambah dokter ke shift dan test alokasi duplikat
    test_shifts[0].dokter_bertugas[0] = &test_doctors[0];
    test_shifts[0].jumlah_dokter = 1;
    TEST_ASSERT_FALSE(apakahAlokasiValid(test_shifts, &test_doctors[0], 0));
    
    // Test konflik preferensi
    TEST_ASSERT_FALSE(apakahAlokasiValid(test_shifts, &test_doctors[0], 2)); // Dr. John tidak suka shift malam
    TEST_ASSERT_FALSE(apakahAlokasiValid(test_shifts, &test_doctors[1], 1)); // Dr. Smith tidak suka shift siang
    TEST_ASSERT_FALSE(apakahAlokasiValid(test_shifts, &test_doctors[2], 0)); // Dr. Davis tidak suka shift pagi
}

void test_count_weekly_shifts(void) {
    add_test_doctors();
    inisialisasiJadwal(test_shifts);
    
    // Tambah beberapa shift di minggu pertama
    test_shifts[0].dokter_bertugas[0] = &test_doctors[0];
    test_shifts[0].jumlah_dokter = 1;
    
    test_shifts[6].dokter_bertugas[0] = &test_doctors[0];
    test_shifts[6].jumlah_dokter = 1;
    
    TEST_ASSERT_EQUAL_INT(2, hitungShiftMingguIni(test_shifts, &test_doctors[0], 0));
    TEST_ASSERT_EQUAL_INT(0, hitungShiftMingguIni(test_shifts, &test_doctors[0], 1));
}

void test_automatic_scheduling(void) {
    add_test_doctors();
    
    // Kita hanya test apakah fungsi berjalan tanpa crash dan return sukses
    // Verifikasi penuh validitas jadwal akan lebih kompleks
    int result = buatJadwalOtomatis(test_shifts, test_doctors, test_doctor_count);
    TEST_ASSERT_TRUE(result);
    
    // Cek bahwa beberapa shift sudah diassign
    int assigned_shifts = 0;
    for (int i = 0; i < TOTAL_SHIFT; i++) {
        if (test_shifts[i].jumlah_dokter > 0) {
            assigned_shifts++;
        }
    }
    TEST_ASSERT_GREATER_THAN(0, assigned_shifts);
}

// Jalankan semua test
int main(void) {
    UNITY_BEGIN();
    
    // Test manajemen dokter
    RUN_TEST(test_add_doctor);
    RUN_TEST(test_add_multiple_doctors);
    RUN_TEST(test_remove_doctor);
    RUN_TEST(test_remove_nonexistent_doctor);
    
    // Test scheduler
    RUN_TEST(test_initialize_schedule);
    RUN_TEST(test_allocation_validity);
    RUN_TEST(test_count_weekly_shifts);
    RUN_TEST(test_automatic_scheduling);
    
    return UNITY_END();
}