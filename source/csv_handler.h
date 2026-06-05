#ifndef CSV_HANDLER_H
#define CSV_HANDLER_H

#include <vector>
#include <string>
#include "patient.h"

using namespace std;


vector<Pasien> loadPasien(const string& filename);

void savePasien(const string& filename, const vector<Pasien>& daftarPasien);

void tambahPasienKeCSV(const string& filename, const Pasien& pasien);

void updatePasienCSV(const string& filename, const vector<Pasien>& daftarPasien);

vector<RiwayatPenyakit> loadRiwayat(const string& filename);

void saveRiwayat(const string& filename, const vector<RiwayatPenyakit>& daftarRiwayat);

void tambahRiwayatKeCSV(const string& filename, const RiwayatPenyakit& riwayat);

bool fileExists(const string& filename);

void initCSV(const string& filenamePasien, const string& filenameRiwayat);

#endif // CSV_HANDLER_H
