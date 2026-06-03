#ifndef PATIENT_H
#define PATIENT_H

#include <string>

using namespace std;

// ============================================================
// SIPUTAR - Sistem Puskesmas Pintar
// Definisi Struct Data
// ============================================================

// Tingkat Darurat (5 Level)
// Semakin kecil angka, semakin tinggi prioritasnya
enum TingkatDarurat {
    GAWAT_DARURAT = 1,   // Mengancam nyawa (tidak dapat bernafas, pendarahan hebat)
    DARURAT       = 2,   // Butuh penanganan segera (patah tulang, luka bakar)
    MENDESAK      = 3,   // Cukup serius (demam tinggi, diare berat)
    BIASA         = 4,   // Pemeriksaan standar (batuk, pilek, sakit kepala)
    KONTROL       = 5    // Kontrol ulang / check-up rutin
};

// Struct untuk data master pasien
struct Pasien {
    string nik;             // Bisa kosong
    string nama;            // Bisa kosong
    string jenisKelamin;    // Bisa kosong ("Laki-laki" / "Perempuan")
    int umur;               // -1 jika kosong
};

// Struct untuk riwayat penyakit pasien
struct RiwayatPenyakit {
    string nik;
    string tanggalPeriksa;  // Format: DD-MM-YYYY
    string namaPenyakit;
    string catatanDokter;
};

// Struct untuk node antrean (digunakan di Priority Queue)
struct AntreanNode {
    Pasien pasien;
    int prioritas;          
    int nomorUrut;          
    string keluhan;         
};

// Fungsi helper untuk mendapatkan label tingkat darurat
inline string getLabelPrioritas(int prioritas) {
    switch (prioritas) {
        case GAWAT_DARURAT: return "GAWAT DARURAT";
        case DARURAT:       return "DARURAT";
        case MENDESAK:      return "MENDESAK";
        case BIASA:         return "BIASA";
        case KONTROL:       return "KONTROL";
        default:            return "TIDAK DIKETAHUI";
    }
}

// Fungsi helper untuk mendapatkan warna ANSI berdasarkan prioritas
inline string getWarnaPrioritas(int prioritas) {
    switch (prioritas) {
        case GAWAT_DARURAT: return "\033[1;91m"; // Merah terang + bold
        case DARURAT:       return "\033[1;31m"; // Merah
        case MENDESAK:      return "\033[1;33m"; // Kuning
        case BIASA:         return "\033[1;32m"; // Hijau
        case KONTROL:       return "\033[1;36m"; // Cyan
        default:            return "\033[0m";    // Reset
    }
}

#endif // PATIENT_H
