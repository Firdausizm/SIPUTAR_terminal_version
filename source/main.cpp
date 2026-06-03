#include <iostream>
#include <string>
#include <vector>

#include "patient.h"
#include "utils.h"
#include "csv_handler.h"
#include "priority_queue.h"
#include "binary_search.h"

using namespace std;

// ============================================================
// SIPUTAR - Sistem Puskesmas Pintar
// Program Utama
// ============================================================

// Path file CSV
const string FILE_PASIEN  = "data_pasien.csv";
const string FILE_RIWAYAT = "riwayat_penyakit.csv";

// ---- Fungsi: Tambah Pasien ke Antrean ----
void tambahPasienAntrean(PriorityQueue& antrean) {
    clearScreen();
    printHeader();
    printSubHeader("Tambah Pasien ke Antrean");

    string nik, nama, jenisKelamin, umurStr, keluhan, prioritasStr;

    printPrompt("NIK Pasien (kosongkan jika tidak ada)");
    getline(cin, nik);

    printPrompt("Nama Pasien (kosongkan jika tidak ada)");
    getline(cin, nama);

    printPrompt("Jenis Kelamin [L/P] (kosongkan jika tidak ada)");
    getline(cin, jenisKelamin);

    // Konversi input jenis kelamin
    if (jenisKelamin == "L" || jenisKelamin == "l") {
        jenisKelamin = "Laki-laki";
    } else if (jenisKelamin == "P" || jenisKelamin == "p") {
        jenisKelamin = "Perempuan";
    } else if (!jenisKelamin.empty()) {
        // Biarkan apa adanya jika sudah ditulis lengkap
    }

    printPrompt("Umur Pasien (kosongkan jika tidak ada)");
    getline(cin, umurStr);

    int umur = -1;
    if (!umurStr.empty()) {
        try {
            umur = stoi(umurStr);
        } catch (...) {
            umur = -1;
        }
    }

    printPrompt("Keluhan Pasien");
    getline(cin, keluhan);

    cout << endl;
    cout << "    " << BOLD_WHITE << "Tingkat Darurat:" << RESET << endl;
    cout << "    " << getWarnaPrioritas(1) << "[1]" << RESET << " GAWAT DARURAT  " << DIM << "- Mengancam nyawa" << RESET << endl;
    cout << "    " << getWarnaPrioritas(2) << "[2]" << RESET << " DARURAT         " << DIM << "- Butuh penanganan segera" << RESET << endl;
    cout << "    " << getWarnaPrioritas(3) << "[3]" << RESET << " MENDESAK        " << DIM << "- Cukup serius" << RESET << endl;
    cout << "    " << getWarnaPrioritas(4) << "[4]" << RESET << " BIASA           " << DIM << "- Pemeriksaan standar" << RESET << endl;
    cout << "    " << getWarnaPrioritas(5) << "[5]" << RESET << " KONTROL         " << DIM << "- Kontrol ulang / check-up" << RESET << endl;
    cout << endl;

    printPrompt("Pilih tingkat darurat [1-5]");
    getline(cin, prioritasStr);

    int prioritas = 4;  // Default: Biasa
    if (!prioritasStr.empty()) {
        try {
            prioritas = stoi(prioritasStr);
            if (prioritas < 1 || prioritas > 5) prioritas = 4;
        } catch (...) {
            prioritas = 4;
        }
    }

    // Buat struct pasien
    Pasien pasien;
    pasien.nik = nik;
    pasien.nama = nama;
    pasien.jenisKelamin = jenisKelamin;
    pasien.umur = umur;

    // Jika NIK tidak kosong, cek apakah sudah ada di database
    if (!nik.empty()) {
        vector<Pasien> daftarPasien = loadPasien(FILE_PASIEN);
        sortPasienByNIK(daftarPasien);
        int idx = cariPasienByNIK(daftarPasien, nik);

        if (idx == -1) {
            // Pasien baru, simpan ke CSV
            tambahPasienKeCSV(FILE_PASIEN, pasien);
            printSuccess("Pasien baru disimpan ke database.");
        } else {
            // Pasien sudah ada, update data jika ada field baru
            bool updated = false;
            if (!nama.empty() && daftarPasien[idx].nama.empty()) {
                daftarPasien[idx].nama = nama;
                updated = true;
            }
            if (!jenisKelamin.empty() && daftarPasien[idx].jenisKelamin.empty()) {
                daftarPasien[idx].jenisKelamin = jenisKelamin;
                updated = true;
            }
            if (umur >= 0 && daftarPasien[idx].umur < 0) {
                daftarPasien[idx].umur = umur;
                updated = true;
            }
            if (updated) {
                updatePasienCSV(FILE_PASIEN, daftarPasien);
                printSuccess("Data pasien diperbarui.");
            }
            // Gunakan data lengkap dari database
            pasien = daftarPasien[idx];
        }
    }

    // Tambah ke antrean
    antrean.enqueue(pasien, prioritas, keluhan);

    cout << endl;
    cout << "  " << BOLD_GREEN << "✓ Pasien berhasil ditambahkan ke antrean!" << RESET << endl;
    cout << endl;
    printInfo("Nama", nama.empty() ? "(Tidak diisi)" : nama);
    printInfo("NIK", nik.empty() ? "(Tidak diisi)" : nik);
    printInfo("Keluhan", keluhan.empty() ? "(Tidak diisi)" : keluhan);
    cout << "    " << DIM << "Prioritas" << RESET << " : " << getWarnaPrioritas(prioritas) << getLabelPrioritas(prioritas) << RESET << endl;
    printInfo("Posisi Antrean", to_string(antrean.getUkuran()) + " pasien dalam antrean");

    pauseScreenNoIgnore();
}

// ---- Fungsi: Panggil Pasien Berikutnya ----
void panggilPasienBerikutnya(PriorityQueue& antrean) {
    clearScreen();
    printHeader();
    printSubHeader("Panggil Pasien Berikutnya");

    if (antrean.isEmpty()) {
        printWarning("Antrean kosong. Tidak ada pasien yang menunggu.");
        pauseScreenNoIgnore();
        return;
    }

    // Tampilkan pasien berikutnya
    AntreanNode node = antrean.dequeue();

    cout << "  " << BOLD_GREEN << "▶ PASIEN BERIKUTNYA:" << RESET << endl;
    cout << endl;
    printInfo("Nama", node.pasien.nama.empty() ? "(Tidak diisi)" : node.pasien.nama);
    printInfo("NIK", node.pasien.nik.empty() ? "(Tidak diisi)" : node.pasien.nik);
    printInfo("Jenis Kelamin", node.pasien.jenisKelamin.empty() ? "(Tidak diisi)" : node.pasien.jenisKelamin);
    printInfo("Umur", node.pasien.umur >= 0 ? to_string(node.pasien.umur) + " tahun" : "(Tidak diisi)");
    printInfo("Keluhan", node.keluhan.empty() ? "(Tidak diisi)" : node.keluhan);
    cout << "    " << DIM << "Prioritas" << RESET << " : " << getWarnaPrioritas(node.prioritas) << getLabelPrioritas(node.prioritas) << RESET << endl;
    printInfo("Sisa Antrean", to_string(antrean.getUkuran()) + " pasien");

    // Opsi: Tambah riwayat penyakit setelah diperiksa
    if (!node.pasien.nik.empty()) {
        cout << endl;
        cout << "    " << BOLD_YELLOW << "Apakah ingin menambah riwayat penyakit untuk pasien ini?" << RESET << endl;
        printMenuItem(1, "Ya, tambah riwayat penyakit");
        printMenuItem(2, "Tidak, lanjutkan");
        cout << endl;
        printPrompt("Pilihan");

        string pilihan;
        getline(cin, pilihan);

        if (pilihan == "1") {
            string tanggal, penyakit, catatan;

            cout << endl;
            printPrompt("Tanggal Periksa (DD-MM-YYYY)");
            getline(cin, tanggal);

            printPrompt("Nama Penyakit");
            getline(cin, penyakit);

            printPrompt("Catatan Dokter");
            getline(cin, catatan);

            RiwayatPenyakit riwayat;
            riwayat.nik = node.pasien.nik;
            riwayat.tanggalPeriksa = tanggal;
            riwayat.namaPenyakit = penyakit;
            riwayat.catatanDokter = catatan;

            tambahRiwayatKeCSV(FILE_RIWAYAT, riwayat);
            printSuccess("Riwayat penyakit berhasil disimpan!");
        }
    }

    pauseScreenNoIgnore();
}

// ---- Fungsi: Lihat Semua Antrean ----
void lihatSemuaAntrean(PriorityQueue& antrean) {
    clearScreen();
    printHeader();
    printSubHeader("Daftar Antrean Saat Ini");

    cout << "    " << BOLD_WHITE << "Total Pasien: " << RESET << antrean.getUkuran() << " pasien" << endl;
    cout << endl;

    antrean.displayAll();

    cout << endl;
    cout << "    " << DIM << "Keterangan Prioritas:" << RESET << endl;
    cout << "    " << getWarnaPrioritas(1) << "■" << RESET << " Gawat Darurat  ";
    cout << getWarnaPrioritas(2) << "■" << RESET << " Darurat  ";
    cout << getWarnaPrioritas(3) << "■" << RESET << " Mendesak  ";
    cout << getWarnaPrioritas(4) << "■" << RESET << " Biasa  ";
    cout << getWarnaPrioritas(5) << "■" << RESET << " Kontrol" << endl;

    pauseScreenNoIgnore();
}

// ---- Menu: Kelola Antrean Pasien ----
void menuAntrean(PriorityQueue& antrean) {
    while (true) {
        clearScreen();
        printHeader();
        printSubHeader("Kelola Antrean Pasien");

        cout << "    " << DIM << "Jumlah antrean saat ini: " << RESET 
             << BOLD_WHITE << antrean.getUkuran() << " pasien" << RESET << endl;
        cout << endl;

        printMenuItem(1, "Tambah Pasien ke Antrean");
        printMenuItem(2, "Panggil Pasien Berikutnya");
        printMenuItem(3, "Lihat Semua Antrean");
        printMenuItem(4, "Kembali ke Menu Utama");
        cout << endl;
        printPrompt("Pilihan");

        string pilihan;
        getline(cin, pilihan);

        if (pilihan == "1") {
            tambahPasienAntrean(antrean);
        } else if (pilihan == "2") {
            panggilPasienBerikutnya(antrean);
        } else if (pilihan == "3") {
            lihatSemuaAntrean(antrean);
        } else if (pilihan == "4") {
            break;
        } else {
            printError("Pilihan tidak valid!");
            pauseScreenNoIgnore();
        }
    }
}

// ---- Fungsi: Tampilkan Riwayat Penyakit ----
void tampilkanRiwayat(const Pasien& pasien, const vector<RiwayatPenyakit>& riwayat) {
    cout << endl;
    cout << "  " << BOLD_GREEN << "✓ Data Pasien Ditemukan!" << RESET << endl;
    cout << endl;

    // Info pasien
    cout << "  " << BOLD_CYAN << "┌──────────────────────────────────────────────────┐" << RESET << endl;
    cout << "  " << BOLD_CYAN << "│" << RESET << BOLD_WHITE << "  DATA PASIEN                                      " << RESET << BOLD_CYAN << "│" << RESET << endl;
    cout << "  " << BOLD_CYAN << "├──────────────────────────────────────────────────┤" << RESET << endl;
    
    // NIK
    cout << "  " << BOLD_CYAN << "│" << RESET;
    cout << "  " << DIM << "NIK" << RESET << "           : " << BOLD_WHITE << (pasien.nik.empty() ? "-" : pasien.nik) << RESET;
    int pad = 50 - 20 - (pasien.nik.empty() ? 1 : pasien.nik.length());
    for (int i = 0; i < pad && i < 30; i++) cout << " ";
    cout << BOLD_CYAN << "│" << RESET << endl;

    // Nama
    cout << "  " << BOLD_CYAN << "│" << RESET;
    string namaDisplay = pasien.nama.empty() ? "-" : pasien.nama;
    cout << "  " << DIM << "Nama" << RESET << "          : " << BOLD_WHITE << namaDisplay << RESET;
    pad = 50 - 20 - namaDisplay.length();
    for (int i = 0; i < pad && i < 30; i++) cout << " ";
    cout << BOLD_CYAN << "│" << RESET << endl;

    // Jenis Kelamin
    cout << "  " << BOLD_CYAN << "│" << RESET;
    string jkDisplay = pasien.jenisKelamin.empty() ? "-" : pasien.jenisKelamin;
    cout << "  " << DIM << "Jenis Kelamin" << RESET << " : " << BOLD_WHITE << jkDisplay << RESET;
    pad = 50 - 20 - jkDisplay.length();
    for (int i = 0; i < pad && i < 30; i++) cout << " ";
    cout << BOLD_CYAN << "│" << RESET << endl;

    // Umur
    cout << "  " << BOLD_CYAN << "│" << RESET;
    string umurDisplay = pasien.umur >= 0 ? to_string(pasien.umur) + " tahun" : "-";
    cout << "  " << DIM << "Umur" << RESET << "          : " << BOLD_WHITE << umurDisplay << RESET;
    pad = 50 - 20 - umurDisplay.length();
    for (int i = 0; i < pad && i < 30; i++) cout << " ";
    cout << BOLD_CYAN << "│" << RESET << endl;

    cout << "  " << BOLD_CYAN << "└──────────────────────────────────────────────────┘" << RESET << endl;

    // Riwayat penyakit
    cout << endl;
    cout << "  " << BOLD_CYAN << "┌──────────────────────────────────────────────────────────────────────────┐" << RESET << endl;
    cout << "  " << BOLD_CYAN << "│" << RESET << BOLD_WHITE << "  RIWAYAT PENYAKIT                                                         " << RESET << BOLD_CYAN << "│" << RESET << endl;
    cout << "  " << BOLD_CYAN << "├──────────────────────────────────────────────────────────────────────────┤" << RESET << endl;

    if (riwayat.empty()) {
        cout << "  " << BOLD_CYAN << "│" << RESET;
        cout << "  " << DIM << "(Belum ada riwayat penyakit)" << RESET;
        for (int i = 0; i < 46; i++) cout << " ";
        cout << BOLD_CYAN << "│" << RESET << endl;
    } else {
        // Header tabel
        cout << "  " << BOLD_CYAN << "│" << RESET;
        cout << BOLD_WHITE << "  No  Tanggal       Penyakit             Catatan Dokter                    " << RESET;
        cout << BOLD_CYAN << "│" << RESET << endl;

        cout << "  " << BOLD_CYAN << "│" << RESET;
        cout << "  ";
        for (int i = 0; i < 72; i++) cout << "─";
        cout << BOLD_CYAN << "│" << RESET << endl;

        for (int i = 0; i < (int)riwayat.size(); i++) {
            cout << "  " << BOLD_CYAN << "│" << RESET;
            
            // Nomor
            cout << "  " << BOLD_WHITE << (i + 1) << "." << RESET;
            if (i + 1 < 10) cout << "  "; else cout << " ";
            
            // Tanggal
            string tgl = riwayat[i].tanggalPeriksa.empty() ? "-" : riwayat[i].tanggalPeriksa;
            cout << tgl;
            for (int j = tgl.length(); j < 13; j++) cout << " ";

            // Penyakit
            string penyakit = riwayat[i].namaPenyakit.empty() ? "-" : riwayat[i].namaPenyakit;
            if (penyakit.length() > 20) penyakit = penyakit.substr(0, 17) + "...";
            cout << BOLD_YELLOW << penyakit << RESET;
            for (int j = penyakit.length(); j < 21; j++) cout << " ";

            // Catatan dokter
            string catatan = riwayat[i].catatanDokter.empty() ? "-" : riwayat[i].catatanDokter;
            if (catatan.length() > 33) catatan = catatan.substr(0, 30) + "...";
            cout << DIM << catatan << RESET;
            for (int j = catatan.length(); j < 34; j++) cout << " ";

            cout << BOLD_CYAN << "│" << RESET << endl;
        }
    }

    cout << "  " << BOLD_CYAN << "└──────────────────────────────────────────────────────────────────────────┘" << RESET << endl;
}

// ---- Menu: Cari Riwayat Penyakit ----
void menuCariRiwayat() {
    clearScreen();
    printHeader();
    printSubHeader("Cari Riwayat Penyakit Pasien");

    string nik;
    printPrompt("Masukkan NIK Pasien");
    getline(cin, nik);

    if (nik.empty()) {
        printError("NIK tidak boleh kosong untuk pencarian!");
        pauseScreenNoIgnore();
        return;
    }

    // Load dan sort data pasien
    vector<Pasien> daftarPasien = loadPasien(FILE_PASIEN);
    sortPasienByNIK(daftarPasien);

    // Binary Search
    int idx = cariPasienByNIK(daftarPasien, nik);

    if (idx == -1) {
        printError("Pasien dengan NIK \"" + nik + "\" tidak ditemukan!");
        
        cout << endl;
        cout << "    " << BOLD_YELLOW << "Apakah ingin mendaftarkan pasien baru?" << RESET << endl;
        printMenuItem(1, "Ya, daftarkan pasien baru");
        printMenuItem(2, "Tidak, kembali");
        cout << endl;
        printPrompt("Pilihan");

        string pilihan;
        getline(cin, pilihan);

        if (pilihan == "1") {
            string nama, jenisKelamin, umurStr;

            cout << endl;
            printPrompt("Nama Pasien");
            getline(cin, nama);

            printPrompt("Jenis Kelamin [L/P]");
            getline(cin, jenisKelamin);

            if (jenisKelamin == "L" || jenisKelamin == "l") {
                jenisKelamin = "Laki-laki";
            } else if (jenisKelamin == "P" || jenisKelamin == "p") {
                jenisKelamin = "Perempuan";
            }

            printPrompt("Umur");
            getline(cin, umurStr);

            int umur = -1;
            if (!umurStr.empty()) {
                try { umur = stoi(umurStr); } catch (...) { umur = -1; }
            }

            Pasien pasienBaru;
            pasienBaru.nik = nik;
            pasienBaru.nama = nama;
            pasienBaru.jenisKelamin = jenisKelamin;
            pasienBaru.umur = umur;

            tambahPasienKeCSV(FILE_PASIEN, pasienBaru);
            printSuccess("Pasien baru berhasil didaftarkan!");
        }

        pauseScreenNoIgnore();
        return;
    }

    // Pasien ditemukan
    Pasien pasien = daftarPasien[idx];

    // Load riwayat penyakit
    vector<RiwayatPenyakit> semuaRiwayat = loadRiwayat(FILE_RIWAYAT);
    vector<RiwayatPenyakit> riwayatPasien = cariRiwayatByNIK(semuaRiwayat, nik);

    tampilkanRiwayat(pasien, riwayatPasien);

    // Opsi: Tambah riwayat baru atau edit data pasien
    cout << endl;
    printMenuItem(1, "Tambah Riwayat Penyakit Baru");
    printMenuItem(2, "Edit Data Pasien");
    printMenuItem(3, "Kembali");
    cout << endl;
    printPrompt("Pilihan");

    string pilihan;
    getline(cin, pilihan);

    if (pilihan == "1") {
        // Tambah riwayat penyakit baru
        string tanggal, penyakit, catatan;

        cout << endl;
        printPrompt("Tanggal Periksa (DD-MM-YYYY)");
        getline(cin, tanggal);

        printPrompt("Nama Penyakit");
        getline(cin, penyakit);

        printPrompt("Catatan Dokter");
        getline(cin, catatan);

        RiwayatPenyakit riwayatBaru;
        riwayatBaru.nik = nik;
        riwayatBaru.tanggalPeriksa = tanggal;
        riwayatBaru.namaPenyakit = penyakit;
        riwayatBaru.catatanDokter = catatan;

        tambahRiwayatKeCSV(FILE_RIWAYAT, riwayatBaru);
        printSuccess("Riwayat penyakit berhasil ditambahkan!");

    } else if (pilihan == "2") {
        // Edit data pasien
        cout << endl;
        cout << "    " << DIM << "(Kosongkan untuk tidak mengubah)" << RESET << endl;
        cout << endl;

        string nama, jenisKelamin, umurStr;

        printPrompt("Nama baru [" + (pasien.nama.empty() ? "-" : pasien.nama) + "]");
        getline(cin, nama);

        printPrompt("Jenis Kelamin baru [" + (pasien.jenisKelamin.empty() ? "-" : pasien.jenisKelamin) + "] (L/P)");
        getline(cin, jenisKelamin);

        if (jenisKelamin == "L" || jenisKelamin == "l") {
            jenisKelamin = "Laki-laki";
        } else if (jenisKelamin == "P" || jenisKelamin == "p") {
            jenisKelamin = "Perempuan";
        }

        printPrompt("Umur baru [" + (pasien.umur >= 0 ? to_string(pasien.umur) : "-") + "]");
        getline(cin, umurStr);

        // Update hanya field yang diisi
        if (!nama.empty()) daftarPasien[idx].nama = nama;
        if (!jenisKelamin.empty()) daftarPasien[idx].jenisKelamin = jenisKelamin;
        if (!umurStr.empty()) {
            try {
                daftarPasien[idx].umur = stoi(umurStr);
            } catch (...) {}
        }

        updatePasienCSV(FILE_PASIEN, daftarPasien);
        printSuccess("Data pasien berhasil diperbarui!");
    }

    pauseScreenNoIgnore();
}

// ============================================================
// MAIN
// ============================================================
int main() {
    // Enable ANSI escape codes di Windows
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif

    // Inisialisasi file CSV jika belum ada
    initCSV(FILE_PASIEN, FILE_RIWAYAT);

    // Inisialisasi Priority Queue
    PriorityQueue antrean;

    // Loop menu utama
    while (true) {
        clearScreen();
        printHeader();

        cout << "    " << DIM << "Selamat datang!" << RESET << endl;
        cout << "    " << DIM << "Silakan pilih menu di bawah ini:" << RESET << endl;
        cout << endl;

        printMenuItem(1, "Kelola Antrean Pasien");
        printMenuItem(2, "Cari Riwayat Penyakit (NIK)");
        printMenuItem(3, "Keluar");

        cout << endl;
        printPrompt("Pilihan");

        string pilihan;
        getline(cin, pilihan);

        if (pilihan == "1") {
            menuAntrean(antrean);
        } else if (pilihan == "2") {
            menuCariRiwayat();
        } else if (pilihan == "3") {
            clearScreen();
            cout << endl;
            cout << "  " << BOLD_CYAN << "╔══════════════════════════════════════════════════╗" << RESET << endl;
            cout << "  " << BOLD_CYAN << "║" << RESET;
            cout << BOLD_WHITE << "     Terima kasih telah menggunakan SIPUTAR!      " << RESET;
            cout << BOLD_CYAN << "║" << RESET << endl;
            cout << "  " << BOLD_CYAN << "║" << RESET;
            cout << DIM <<       "          Semoga hari Anda menyenangkan            " << RESET;
            cout << BOLD_CYAN << "║" << RESET << endl;
            cout << "  " << BOLD_CYAN << "╚══════════════════════════════════════════════════╝" << RESET << endl;
            cout << endl;
            break;
        } else {
            printError("Pilihan tidak valid! Silakan masukkan angka 1-3.");
            pauseScreenNoIgnore();
        }
    }

    return 0;
}
