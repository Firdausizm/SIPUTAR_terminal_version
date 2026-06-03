#ifndef BINARY_SEARCH_H
#define BINARY_SEARCH_H

#include <vector>
#include <string>
#include "patient.h"

using namespace std;

// ============================================================
// SIPUTAR - Sistem Puskesmas Pintar
// Binary Search & Sorting - Header
// Implementasi manual tanpa library tambahan
// ============================================================

// Sort data pasien berdasarkan NIK (ascending) menggunakan Merge Sort
void sortPasienByNIK(vector<Pasien>& daftar);

// Binary Search: cari pasien berdasarkan NIK
// Return: index pasien jika ditemukan, -1 jika tidak ditemukan
int cariPasienByNIK(const vector<Pasien>& daftar, const string& nik);

// Cari semua riwayat penyakit berdasarkan NIK (linear search, karena tidak unique)
vector<RiwayatPenyakit> cariRiwayatByNIK(const vector<RiwayatPenyakit>& daftar, const string& nik);

#endif // BINARY_SEARCH_H
