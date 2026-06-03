#ifndef CSV_HANDLER_H
#define CSV_HANDLER_H

#include <vector>
#include <string>
#include "patient.h"

using namespace std;

// ============================================================
// SIPUTAR - Sistem Puskesmas Pintar
// CSV Handler - Header
// ============================================================

// Load semua data pasien dari file CSV
vector<Pasien> loadPasien(const string& filename);

// Simpan semua data pasien ke file CSV (overwrite)
void savePasien(const string& filename, const vector<Pasien>& daftarPasien);

// Tambah 1 pasien ke file CSV (append)
void tambahPasienKeCSV(const string& filename, const Pasien& pasien);

// Update data pasien tertentu (berdasarkan NIK)
void updatePasienCSV(const string& filename, const vector<Pasien>& daftarPasien);

// Load semua riwayat penyakit dari file CSV
vector<RiwayatPenyakit> loadRiwayat(const string& filename);

// Simpan semua riwayat penyakit ke file CSV (overwrite)
void saveRiwayat(const string& filename, const vector<RiwayatPenyakit>& daftarRiwayat);

// Tambah 1 riwayat penyakit ke file CSV (append)
void tambahRiwayatKeCSV(const string& filename, const RiwayatPenyakit& riwayat);

// Cek apakah file CSV ada
bool fileExists(const string& filename);

// Buat file CSV dengan header jika belum ada
void initCSV(const string& filenamePasien, const string& filenameRiwayat);

#endif // CSV_HANDLER_H
