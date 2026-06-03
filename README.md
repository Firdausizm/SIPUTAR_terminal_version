Ini adalah tugas akhir matakuliah algoritma pemrograman pada semester 2. segala bentuk file dan kode disini hanya untuk bahan pembelajaran semata.

SIPUTAR adalah platform untuk mengelola antrean pasien dan melakuan pencarian riwayat penyakit pasien. fitur utama SIPUTAR menggunakan algoritma priority queue dan binary search sehngga sangat cocok digunakan dalam ekosistem puskesmas.


Cara menjalankan di local :

- clone repository ini
- buka folder
- compile source code: `g++ -std=c++17 -o siputar source/main.cpp source/csv_handler.cpp source/priority_queue.cpp source/binary_search.cpp`
- jalankan `./siputar` di terminal


Cara untuk kontribusi :

- clone repository ini

- bikin branch baru

- kerjakan fitur dan test di branch itu

- comit dan push di branch yang telah dibuat

- bikin pull request

- pull request akan di review oleh atmin

- jika acc maka atmin akan merge ke main branch

- (Note) laukan rebase jika ketinggalan versi
