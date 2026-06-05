#include "binary_search.h"
#include <string>

using namespace std;



// Merge dua subarray yang sudah terurut
static void merge(vector<Pasien> &arr, int left, int mid, int right) {
  int n1 = mid - left + 1;
  int n2 = right - mid;

  // Buat array temporary
  vector<Pasien> leftArr(n1);
  vector<Pasien> rightArr(n2);

  // Copy data ke array temporary
  for (int i = 0; i < n1; i++)
    leftArr[i] = arr[left + i];
  for (int j = 0; j < n2; j++)
    rightArr[j] = arr[mid + 1 + j];

  // Merge kembali ke array utama
  int i = 0, j = 0, k = left;

  while (i < n1 && j < n2) {
    // Bandingkan NIK secara leksikografis
    if (leftArr[i].nik <= rightArr[j].nik) {
      arr[k] = leftArr[i];
      i++;
    } else {
      arr[k] = rightArr[j];
      j++;
    }
    k++;
  }

  // Copy sisa elemen
  while (i < n1) {
    arr[k] = leftArr[i];
    i++;
    k++;
  }
  while (j < n2) {
    arr[k] = rightArr[j];
    j++;
    k++;
  }
}

// Merge Sort rekursif
static void mergeSort(vector<Pasien> &arr, int left, int right) {
  if (left < right) {
    int mid = left + (right - left) / 2;

    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
  }
}

// Sort data pasien berdasarkan NIK
void sortPasienByNIK(vector<Pasien> &daftar) {
  if (daftar.size() <= 1)
    return;
  mergeSort(daftar, 0, daftar.size() - 1);
}

// Return: index pasien jika ditemukan, -1 jika tidak ditemukan
int cariPasienByNIK(const vector<Pasien> &daftar, const string &nik) {
  int low = 0;
  int high = daftar.size() - 1;

  while (low <= high) {
    int mid = low + (high - low) / 2;

    if (daftar[mid].nik == nik) {
      return mid; // Ditemukan!
    } else if (daftar[mid].nik < nik) {
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }

  return -1; // Tidak ditemukan
}


// Linear search karena satu pasien bisa punya banyak riwayat
vector<RiwayatPenyakit> cariRiwayatByNIK(const vector<RiwayatPenyakit> &daftar,
                                         const string &nik) {
  vector<RiwayatPenyakit> hasil;

  for (int i = 0; i < (int)daftar.size(); i++) {
    if (daftar[i].nik == nik) {
      hasil.push_back(daftar[i]);
    }
  }

  return hasil;
}
