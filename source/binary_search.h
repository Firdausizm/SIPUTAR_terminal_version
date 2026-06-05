#ifndef BINARY_SEARCH_H
#define BINARY_SEARCH_H

#include <vector>
#include <string>
#include "patient.h"

using namespace std;


// Sort data pasien berdasarkan NIK (ascending) menggunakan Merge Sort
void sortPasienByNIK(vector<Pasien>& daftar);

int cariPasienByNIK(const vector<Pasien>& daftar, const string& nik);

// Cari semua riwayat penyakit berdasarkan NIK (linear search, karena tidak unique)
vector<RiwayatPenyakit> cariRiwayatByNIK(const vector<RiwayatPenyakit>& daftar, const string& nik);

#endif // BINARY_SEARCH_H
