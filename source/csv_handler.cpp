#include "csv_handler.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

// ============================================================
// SIPUTAR - Sistem Puskesmas Pintar
// CSV Handler - Implementasi
// ============================================================

// ---- Helper: Trim whitespace dari string ----
static string trim(const string& str) {
    int start = 0;
    int end = str.length() - 1;
    while (start <= end && (str[start] == ' ' || str[start] == '\t' || str[start] == '\r' || str[start] == '\n'))
        start++;
    while (end >= start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\r' || str[end] == '\n'))
        end--;
    if (start > end) return "";
    return str.substr(start, end - start + 1);
}

// ---- Helper: Parse satu baris CSV (handle koma di dalam quotes) ----
static vector<string> parseCSVLine(const string& line) {
    vector<string> fields;
    string field = "";
    bool inQuotes = false;

    for (int i = 0; i < (int)line.length(); i++) {
        char c = line[i];
        if (c == '"') {
            if (inQuotes && i + 1 < (int)line.length() && line[i + 1] == '"') {
                // Escaped quote ("") -> satu tanda kutip
                field += '"';
                i++;
            } else {
                inQuotes = !inQuotes;
            }
        } else if (c == ',' && !inQuotes) {
            fields.push_back(trim(field));
            field = "";
        } else {
            field += c;
        }
    }
    fields.push_back(trim(field));
    return fields;
}

// ---- Helper: Escape field CSV (tambah quotes jika perlu) ----
static string escapeCSV(const string& field) {
    bool needQuotes = false;
    for (int i = 0; i < (int)field.length(); i++) {
        if (field[i] == ',' || field[i] == '"' || field[i] == '\n') {
            needQuotes = true;
            break;
        }
    }
    if (!needQuotes) return field;

    string escaped = "\"";
    for (int i = 0; i < (int)field.length(); i++) {
        if (field[i] == '"') {
            escaped += "\"\"";
        } else {
            escaped += field[i];
        }
    }
    escaped += "\"";
    return escaped;
}

// ---- Cek apakah file ada ----
bool fileExists(const string& filename) {
    ifstream f(filename);
    return f.good();
}

// ---- Inisialisasi file CSV dengan header jika belum ada ----
void initCSV(const string& filenamePasien, const string& filenameRiwayat) {
    if (!fileExists(filenamePasien)) {
        ofstream f(filenamePasien);
        f << "NIK,Nama,JenisKelamin,Umur" << endl;
        f.close();
    }
    if (!fileExists(filenameRiwayat)) {
        ofstream f(filenameRiwayat);
        f << "NIK,TanggalPeriksa,NamaPenyakit,CatatanDokter" << endl;
        f.close();
    }
}

// ---- Load data pasien dari CSV ----
vector<Pasien> loadPasien(const string& filename) {
    vector<Pasien> daftar;
    ifstream file(filename);

    if (!file.is_open()) {
        return daftar;
    }

    string line;
    bool isHeader = true;

    while (getline(file, line)) {
        if (isHeader) {
            isHeader = false;
            continue;  // Skip header row
        }

        line = trim(line);
        if (line.empty()) continue;

        vector<string> fields = parseCSVLine(line);

        Pasien p;
        p.nik = (fields.size() > 0) ? fields[0] : "";
        p.nama = (fields.size() > 1) ? fields[1] : "";
        p.jenisKelamin = (fields.size() > 2) ? fields[2] : "";

        if (fields.size() > 3 && !fields[3].empty()) {
            // Parse integer manually tanpa stoi error handling
            try {
                p.umur = stoi(fields[3]);
            } catch (...) {
                p.umur = -1;
            }
        } else {
            p.umur = -1;
        }

        daftar.push_back(p);
    }

    file.close();
    return daftar;
}

// ---- Simpan semua data pasien ke CSV (overwrite) ----
void savePasien(const string& filename, const vector<Pasien>& daftarPasien) {
    ofstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Tidak dapat membuka file " << filename << endl;
        return;
    }

    // Header
    file << "NIK,Nama,JenisKelamin,Umur" << endl;

    for (int i = 0; i < (int)daftarPasien.size(); i++) {
        const Pasien& p = daftarPasien[i];
        file << escapeCSV(p.nik) << ","
             << escapeCSV(p.nama) << ","
             << escapeCSV(p.jenisKelamin) << ",";

        if (p.umur >= 0) {
            file << p.umur;
        }
        file << endl;
    }

    file.close();
}

// ---- Tambah 1 pasien ke CSV (append) ----
void tambahPasienKeCSV(const string& filename, const Pasien& pasien) {
    ofstream file(filename, ios::app);

    if (!file.is_open()) {
        cerr << "Error: Tidak dapat membuka file " << filename << endl;
        return;
    }

    file << escapeCSV(pasien.nik) << ","
         << escapeCSV(pasien.nama) << ","
         << escapeCSV(pasien.jenisKelamin) << ",";

    if (pasien.umur >= 0) {
        file << pasien.umur;
    }
    file << endl;

    file.close();
}

// ---- Update data pasien (overwrite semua) ----
void updatePasienCSV(const string& filename, const vector<Pasien>& daftarPasien) {
    savePasien(filename, daftarPasien);
}

// ---- Load riwayat penyakit dari CSV ----
vector<RiwayatPenyakit> loadRiwayat(const string& filename) {
    vector<RiwayatPenyakit> daftar;
    ifstream file(filename);

    if (!file.is_open()) {
        return daftar;
    }

    string line;
    bool isHeader = true;

    while (getline(file, line)) {
        if (isHeader) {
            isHeader = false;
            continue;
        }

        line = trim(line);
        if (line.empty()) continue;

        vector<string> fields = parseCSVLine(line);

        RiwayatPenyakit r;
        r.nik = (fields.size() > 0) ? fields[0] : "";
        r.tanggalPeriksa = (fields.size() > 1) ? fields[1] : "";
        r.namaPenyakit = (fields.size() > 2) ? fields[2] : "";
        r.catatanDokter = (fields.size() > 3) ? fields[3] : "";

        daftar.push_back(r);
    }

    file.close();
    return daftar;
}

// ---- Simpan semua riwayat penyakit ke CSV (overwrite) ----
void saveRiwayat(const string& filename, const vector<RiwayatPenyakit>& daftarRiwayat) {
    ofstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Tidak dapat membuka file " << filename << endl;
        return;
    }

    file << "NIK,TanggalPeriksa,NamaPenyakit,CatatanDokter" << endl;

    for (int i = 0; i < (int)daftarRiwayat.size(); i++) {
        const RiwayatPenyakit& r = daftarRiwayat[i];
        file << escapeCSV(r.nik) << ","
             << escapeCSV(r.tanggalPeriksa) << ","
             << escapeCSV(r.namaPenyakit) << ","
             << escapeCSV(r.catatanDokter) << endl;
    }

    file.close();
}

// ---- Tambah 1 riwayat penyakit ke CSV (append) ----
void tambahRiwayatKeCSV(const string& filename, const RiwayatPenyakit& riwayat) {
    ofstream file(filename, ios::app);

    if (!file.is_open()) {
        cerr << "Error: Tidak dapat membuka file " << filename << endl;
        return;
    }

    file << escapeCSV(riwayat.nik) << ","
         << escapeCSV(riwayat.tanggalPeriksa) << ","
         << escapeCSV(riwayat.namaPenyakit) << ","
         << escapeCSV(riwayat.catatanDokter) << endl;

    file.close();
}
