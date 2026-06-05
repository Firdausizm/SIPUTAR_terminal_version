#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "patient.h"

const int MAX_ANTREAN = 50; // Kapasitas maksimum antrean

class PriorityQueue {
private:
  AntreanNode heap[MAX_ANTREAN];
  int ukuran;
  int counterUrut; // Auto-increment untuk nomor urut

  // Fungsi heap internal (manual, tanpa library)
  void siftUp(int index);
  void siftDown(int index);
  void swap(int i, int j);

  // return true jika a lebih prioritas dari b
  bool lebihPrioritas(const AntreanNode &a, const AntreanNode &b);

public:
  PriorityQueue();

  // Operasi utama
  void enqueue(const Pasien &pasien, int prioritas, const string &keluhan);
  AntreanNode dequeue();
  AntreanNode peek() const;

  // Utilitas
  bool isEmpty() const;
  bool isFull() const;
  int getUkuran() const;

  // Tampilkan semua antrean
  void displayAll() const;
};

#endif // PRIORITY_QUEUE_H
