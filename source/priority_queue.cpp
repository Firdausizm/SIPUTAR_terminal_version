#include "priority_queue.h"
#include "utils.h"
#include <iostream>
#include <string>

using namespace std;

// ---- Constructor ----
PriorityQueue::PriorityQueue() {
    ukuran = 0;
    counterUrut = 0;
}

// ---- Swap dua elemen ----
void PriorityQueue::swap(int i, int j) {
    AntreanNode temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

// ---- Komparator: a lebih prioritas dari b? ----
// Return true jika a harus dilayani sebelum b
bool PriorityQueue::lebihPrioritas(const AntreanNode& a, const AntreanNode& b) {
    // Prioritas lebih kecil = lebih penting
    if (a.prioritas != b.prioritas) {
        return a.prioritas < b.prioritas;
    }
    // Jika prioritas sama, yang datang lebih dulu (nomorUrut lebih kecil) lebih dulu
    return a.nomorUrut < b.nomorUrut;
}

// ---- Sift Up: Perbaiki heap ke atas setelah insert ----
void PriorityQueue::siftUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (lebihPrioritas(heap[index], heap[parent])) {
            swap(index, parent);
            index = parent;
        } else {
            break;
        }
    }
}

// ---- Sift Down: Perbaiki heap ke bawah setelah delete ----
void PriorityQueue::siftDown(int index) {
    while (true) {
        int smallest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < ukuran && lebihPrioritas(heap[left], heap[smallest])) {
            smallest = left;
        }
        if (right < ukuran && lebihPrioritas(heap[right], heap[smallest])) {
            smallest = right;
        }

        if (smallest != index) {
            swap(index, smallest);
            index = smallest;
        } else {
            break;
        }
    }
}

// ---- Enqueue: Tambah pasien ke antrean ----
void PriorityQueue::enqueue(const Pasien& pasien, int prioritas, const string& keluhan) {
    if (isFull()) {
        cout << BOLD_RED << "  ✗ Antrean penuh! Kapasitas maksimum " << MAX_ANTREAN << " pasien." << RESET << endl;
        return;
    }

    AntreanNode node;
    node.pasien = pasien;
    node.prioritas = prioritas;
    node.nomorUrut = ++counterUrut;
    node.keluhan = keluhan;

    heap[ukuran] = node;
    siftUp(ukuran);
    ukuran++;
}

// ---- Dequeue: Keluarkan pasien dengan prioritas tertinggi ----
AntreanNode PriorityQueue::dequeue() {
    AntreanNode top = heap[0];

    ukuran--;
    heap[0] = heap[ukuran];
    if (ukuran > 0) {
        siftDown(0);
    }

    return top;
}

// ---- Peek: Lihat pasien berikutnya tanpa mengeluarkan ----
AntreanNode PriorityQueue::peek() const {
    return heap[0];
}

// ---- isEmpty ----
bool PriorityQueue::isEmpty() const {
    return ukuran == 0;
}

// ---- isFull ----
bool PriorityQueue::isFull() const {
    return ukuran >= MAX_ANTREAN;
}

// ---- Get ukuran ----
int PriorityQueue::getUkuran() const {
    return ukuran;
}

// ---- Tampilkan semua antrean (sorted by priority) ----
void PriorityQueue::displayAll() const {
    if (ukuran == 0) {
        cout << "    " << DIM << "(Antrean kosong)" << RESET << endl;
        return;
    }

    // Buat salinan heap dan extract satu per satu untuk tampilan terurut
    // (Kita tidak bisa iterasi heap secara terurut tanpa extract)
    AntreanNode tempHeap[MAX_ANTREAN];
    for (int i = 0; i < ukuran; i++) {
        tempHeap[i] = heap[i];
    }

    int tempUkuran = ukuran;

    // Header tabel
    cout << "    " << BOLD_WHITE;
    cout << "No  ";
    cout << "Nama                 ";
    cout << "NIK              ";
    cout << "Prioritas       ";
    cout << RESET << endl;
    cout << "    ";
    for (int i = 0; i < 68; i++) cout << "─";
    cout << endl;

    int nomor = 1;

    // Extract satu per satu dari temp heap (simulasi dequeue)
    while (tempUkuran > 0) {
        // Cari elemen dengan prioritas tertinggi (index 0 di heap)
        AntreanNode current = tempHeap[0];

        // Tampilkan
        string warna = getWarnaPrioritas(current.prioritas);
        string nama = current.pasien.nama.empty() ? "(Tanpa Nama)" : current.pasien.nama;
        string nik = current.pasien.nik.empty() ? "(Tanpa NIK)" : current.pasien.nik;

        cout << "    ";
        cout << BOLD_WHITE << nomor << ".  " << RESET;

        // Pad nama to 20 chars
        cout << nama;
        for (int i = nama.length(); i < 21; i++) cout << " ";

        // Pad NIK to 17 chars
        cout << nik;
        for (int i = nik.length(); i < 17; i++) cout << " ";

        // Prioritas dengan warna
        cout << warna << getLabelPrioritas(current.prioritas) << RESET;
        cout << endl;

        // Hapus root dari temp heap
        tempUkuran--;
        tempHeap[0] = tempHeap[tempUkuran];

        // Sift down di temp heap
        int idx = 0;
        while (true) {
            int smallest = idx;
            int left = 2 * idx + 1;
            int right = 2 * idx + 2;

            if (left < tempUkuran) {
                // Inline comparator
                if (tempHeap[left].prioritas < tempHeap[smallest].prioritas ||
                    (tempHeap[left].prioritas == tempHeap[smallest].prioritas &&
                     tempHeap[left].nomorUrut < tempHeap[smallest].nomorUrut)) {
                    smallest = left;
                }
            }
            if (right < tempUkuran) {
                if (tempHeap[right].prioritas < tempHeap[smallest].prioritas ||
                    (tempHeap[right].prioritas == tempHeap[smallest].prioritas &&
                     tempHeap[right].nomorUrut < tempHeap[smallest].nomorUrut)) {
                    smallest = right;
                }
            }

            if (smallest != idx) {
                AntreanNode tmp = tempHeap[idx];
                tempHeap[idx] = tempHeap[smallest];
                tempHeap[smallest] = tmp;
                idx = smallest;
            } else {
                break;
            }
        }

        nomor++;
    }
}
