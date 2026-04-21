# Lesson Plan — Week 10: Concurrency Introduction
**Mata Kuliah:** Sistem Operasi  
**Durasi:** 100 menit  
**Referensi:** OSTEP Chapter 26 — Concurrency: An Introduction  
https://pages.cs.wisc.edu/~remzi/OSTEP/threads-intro.pdf

---

## Tujuan Pembelajaran

Setelah pertemuan selesai, mahasiswa mampu:
1. Menjelaskan apa itu thread dan bagaimana thread berbeda dari proses.
2. Mengidentifikasi sumber daya apa yang di-*share* dan apa yang bersifat private pada thread.
3. Membaca kode C sederhana dengan thread dan memprediksi kemungkinan outputnya.
4. Mendefinisikan *race condition* dan memberikan contoh konkretnya.
5. Menjelaskan mengapa `counter++` bisa menghasilkan hasil salah ketika dijalankan oleh dua thread.
6. Mendefinisikan *critical section* beserta tiga syarat solusinya (mutual exclusion, progress, bounded waiting).
7. Menjelaskan mengapa proteksi critical section diperlukan.

---

## Peta Waktu (100 Menit)

| # | Bagian | Durasi |
|---|--------|--------|
| 0 | Pembuka & motivasi | 10 menit |
| 1 | Thread Abstraction | 25 menit |
| 2 | Race Condition | 30 menit |
| 3 | Critical Section | 20 menit |
| 4 | Ringkasan & Latihan Penutup | 15 menit |

---

## Bagian 0: Pembuka & Motivasi (10 menit)

**Tujuan:** Membangun intuisi *mengapa* concurrency itu penting dan mengapa itu sulit.

### Poin utama:
- **Konteks:** Kita sudah belajar bagaimana satu proses berjalan. Tapi dunia nyata tidak sekuensial.
- **Motivasi nyata:**
  - Browser: download file + render halaman + handle input user → semuanya "bersamaan"
  - Server web: ratusan request masuk secara bersamaan
  - Program yang memanfaatkan multi-core CPU
- **Pertanyaan pemantik:**
  > "Jika dua orang chef memasak di dapur yang sama, apa yang bisa salah?"
- **Analogi dapur:**
  - Chef = thread
  - Dapur (bahan, alat) = shared memory
  - Resep masing-masing = program counter + stack masing-masing
  - Masalah: dua chef ambil telur terakhir di waktu yang sama

### Slide yang dibutuhkan:
- 1 slide: motivasi (browser, server)
- 1 slide: analogi dapur (ilustrasi)
- 1 slide: agenda/outline hari ini

---

## Bagian 1: Thread Abstraction (25 menit)

**Tujuan:** Mahasiswa memahami apa itu thread secara konseptual dan teknis.

### 1.1 — Thread vs Process (8 menit)

**Masalah dengan process murni:**
- Fork membuat *copy penuh* address space → mahal
- Komunikasi antar proses (IPC) kompleks
- Context switch antar proses lebih berat

**Thread sebagai solusi:**
- Thread = unit eksekusi *di dalam* proses
- Berbagi address space yang sama dengan thread lain di proses yang sama
- Lebih ringan: buat dan switch lebih cepat

**Tabel perbandingan:**

| Aspek | Process | Thread |
|-------|---------|--------|
| Address space | Masing-masing sendiri | Berbagi dengan thread lain |
| Overhead buat | Besar (fork + copy) | Kecil |
| Context switch | Berat (ganti page table) | Ringan |
| Komunikasi | IPC (pipe, socket, shm) | Langsung via shared memory |
| Crash satu | Tidak pengaruhi lain | Bisa crash seluruh proses |

### 1.2 — Apa yang Di-*share* dan Apa yang Private (7 menit)

**Di-*share* antar thread (dalam satu proses):**
- Code segment (teks program)
- Heap (memori dinamis)
- Global dan static variables
- File descriptors
- Signal handlers

**Private per thread:**
- Stack (variabel lokal, return address)
- Register (termasuk PC — program counter)
- Thread ID
- errno (di sistem modern)

**Analogi:**
- Tangan kanan dan kiri berbagi satu tubuh (otak, organ), tapi punya gerakan masing-masing yang independen.

**Visualisasi address space:**
```
+------------------+  <- Process Address Space
|   Code (shared)  |
+------------------+
|   Heap (shared)  |
+------------------+
|   Stack Thread 2 |  <- private Thread 2
+------------------+
|   Stack Thread 1 |  <- private Thread 1
+------------------+
```

### 1.3 — Thread Control Block & State (5 menit)

**TCB (Thread Control Block):**
- Analogus dengan PCB untuk process
- Berisi: register state, stack pointer, program counter, thread ID, state

**Thread states:** Running → Ready → Blocked (sama seperti process)

**Context switch antar thread:**
- Simpan register thread lama ke TCB
- Muat register thread baru dari TCB
- Stack pointer ganti → stack berbeda

### 1.4 — API Thread Sederhana: pthreads (5 menit)

Contoh kode `hello_thread.c`:
```c
#include <stdio.h>
#include <pthread.h>

void *my_thread(void *arg) {
    char *name = (char *) arg;
    printf("Thread %s berjalan\n", name);
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, my_thread, "A");
    pthread_create(&t2, NULL, my_thread, "B");
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Main selesai\n");
    return 0;
}
```

**Pertanyaan:** Apakah urutan output "Thread A" dan "Thread B" selalu sama?  
**Jawaban:** Tidak — bergantung pada scheduler OS. Inilah yang disebut *non-determinism*.

### Slide yang dibutuhkan:
- 1 slide: Thread vs Process (tabel + perbedaan utama)
- 1 slide: Apa yang di-share vs private (diagram address space)
- 1 slide: TCB dan thread states
- 1 slide: Kode `pthread_create` + pertanyaan non-determinism

---

## Bagian 2: Race Condition (30 menit)

**Tujuan:** Mahasiswa memahami apa itu race condition dan mengapa ia berbahaya, melalui contoh konkret di level instruksi.

### 2.1 — Motivasi: Kode yang Tampak Benar (5 menit)

Kode `counter.c`:
```c
#include <stdio.h>
#include <pthread.h>

int counter = 0;  // shared!

void *increment(void *arg) {
    for (int i = 0; i < 1000000; i++) {
        counter++;   // <-- tampak sederhana
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("counter = %d\n", counter);  // Harusnya 2.000.000?
    return 0;
}
```

**Tanya ke kelas:** Berapa output yang diharapkan? Coba jalankan beberapa kali — apakah selalu sama?

**Hasil nyata:** Output bervariasi: 1.234.512, 1.876.023, dll. — TIDAK pernah 2.000.000!

### 2.2 — Membongkar di Level Instruksi (10 menit)

`counter++` bukan satu instruksi! Di assembly, ini terdiri dari **tiga langkah**:

```
; counter++ setara dengan:
MOV  eax, [counter]   ; (1) LOAD  — baca nilai counter dari memori ke register
ADD  eax, 1           ; (2) ADD   — tambahkan 1 di register
MOV  [counter], eax   ; (3) STORE — tulis hasil kembali ke memori
```

**Skenario interleaving yang bermasalah (Race):**

```
counter = 50  (awal)

Thread 1                    Thread 2
-----------                 -----------
LOAD  eax = 50
                            LOAD  eax = 50   ← baca nilai lama!
ADD   eax = 51
                            ADD   eax = 51
STORE counter = 51
                            STORE counter = 51  ← overwrite Thread 1!

Hasil akhir: counter = 51  (harusnya 52!)
```

**Kesimpulan:** Satu increment hilang! Jika ini terjadi jutaan kali, hasil sangat melenceng.

### 2.3 — Definisi Race Condition (5 menit)

> **Race Condition** adalah kondisi di mana hasil eksekusi program bergantung pada urutan atau timing eksekusi yang tidak dapat dikontrol dari dua atau lebih thread yang mengakses data yang sama.

**Karakteristik:**
- Non-deterministic: bisa benar kadang, salah kadang
- Sulit di-debug: bug tidak selalu dapat direproduksi
- Bergantung pada scheduler OS dan kecepatan CPU
- Sering muncul pada sistem yang kompleks atau beban tinggi

**Analogi:**
- Dua orang menulis di papan tulis yang sama secara bersamaan — tergantung siapa duluan yang selesai nulis, hasilnya berbeda.

### 2.4 — Latihan: Identifikasi Race Condition (10 menit)

**Soal 1:** Apakah kode ini mengandung race condition?
```c
// Thread 1 dan Thread 2 keduanya menjalankan fungsi ini:
void transfer(int amount) {
    balance_A -= amount;
    balance_B += amount;
}
```

**Soal 2:** Bagaimana jika kita gunakan variabel lokal?
```c
void calculate(int x) {
    int local = x * 2;   // Apakah ada race condition di sini?
    printf("%d\n", local);
}
```

**Jawaban:**
- Soal 1: **Ya**, race condition mungkin terjadi — `balance_A` dan `balance_B` adalah shared data.
- Soal 2: **Tidak**, variabel lokal ada di stack masing-masing thread (private).

### Slide yang dibutuhkan:
- 1 slide: Kode `counter.c` + pertanyaan output
- 1 slide: Assembly `counter++` → 3 instruksi
- 1 slide: Diagram interleaving thread 1 vs thread 2 (tabel step-by-step)
- 1 slide: Definisi race condition + karakteristik
- 1 slide: Latihan identifikasi (2 soal + pembahasan)

---

## Bagian 3: Critical Section (20 menit)

**Tujuan:** Mahasiswa memahami apa itu critical section dan syarat-syarat formal sebuah solusi yang benar.

### 3.1 — Definisi Critical Section (5 menit)

> **Critical Section** adalah bagian dari kode yang mengakses *shared resource* (variabel, struktur data, file, dll.) yang tidak boleh dieksekusi oleh lebih dari satu thread secara bersamaan.

**Contoh di kode counter:**
```c
// Ini adalah critical section:
counter++;     // mengakses shared variable 'counter'
```

**Bukan critical section:**
```c
// Ini BUKAN critical section (variabel lokal, tidak shared):
int temp = x * 2;
printf("%d\n", temp);
```

**Visualisasi timeline:**
```
Thread 1: [--- non-critical ---] [CRITICAL] [--- non-critical ---]
Thread 2: [--- non-critical ---]          [CRITICAL]
                                          ^--- tidak boleh bersamaan!
```

### 3.2 — Tiga Syarat Solusi Critical Section (10 menit)

Setiap solusi yang benar untuk critical section harus memenuhi **tiga syarat**:

**1. Mutual Exclusion (Eksklusivitas)**
- Hanya satu thread yang boleh berada di dalam critical section pada satu waktu.
- Jika Thread 1 sedang di CS, Thread 2 harus menunggu di luar.

**2. Progress (Kemajuan)**
- Jika tidak ada thread di dalam critical section dan ada thread yang ingin masuk, keputusan siapa yang masuk harus dibuat dalam waktu terbatas.
- Tidak boleh ada kebuntuan karena tidak ada yang mau "masuk duluan".

**3. Bounded Waiting (Penantian Terbatas)**
- Setelah thread meminta masuk ke critical section, ada batas berapa kali thread lain boleh masuk lebih dulu sebelum permintaan itu dilayani.
- Thread tidak boleh menunggu selamanya (*starvation*).

**Tabel ringkasan:**

| Syarat | Pertanyaan kunci | Jika dilanggar... |
|--------|-----------------|-------------------|
| Mutual Exclusion | Maks 1 thread di CS? | Race condition terjadi |
| Progress | Keputusan masuk CS dalam waktu terbatas? | Deadlock mungkin terjadi |
| Bounded Waiting | Thread tidak menunggu selamanya? | Starvation terjadi |

### 3.3 — Naive Attempts yang Gagal (5 menit)

**Percobaan 1 — Flag sederhana:**
```c
int flag[2] = {0, 0};  // flag[i] = 1 berarti Thread i ingin masuk

// Thread 0:
flag[0] = 1;           // nyatakan niat
while (flag[1]);       // tunggu Thread 1 selesai
// ... critical section ...
flag[0] = 0;
```

**Masalah:** Jika kedua thread set flag mereka secara bersamaan sebelum cek → keduanya menunggu selamanya (**deadlock**).

**Percobaan 2 — Turn variable:**
```c
int turn = 0;  // giliran siapa masuk CS

// Thread 0:
while (turn != 0);     // tunggu giliran
// ... critical section ...
turn = 1;              // serahkan giliran ke Thread 1
```

**Masalah:** Tidak memenuhi syarat *progress* — Thread 1 tidak bisa masuk CS dua kali berturut-turut tanpa Thread 0 "memberi giliran" terlebih dulu.

**Kesimpulan:** Solusi yang benar membutuhkan dukungan hardware atau OS → ini yang akan kita pelajari minggu depan: **Mutex / Lock**.

### Slide yang dibutuhkan:
- 1 slide: Definisi critical section + visualisasi timeline
- 1 slide: Tiga syarat (mutual exclusion, progress, bounded waiting)
- 1 slide: Tabel syarat + konsekuensi jika dilanggar
- 1 slide: Naive attempts (flag & turn) + kenapa gagal
- 1 slide: Preview solusi: Lock/Mutex (teaser minggu 11)

---

## Bagian 4: Ringkasan & Latihan Penutup (15 menit)

### 4.1 — Ringkasan Poin Kunci (5 menit)

**Thread Abstraction:**
- Thread = unit eksekusi dalam proses, berbagi address space
- Share: code, heap, global vars | Private: stack, register, PC
- Context switch thread lebih ringan dari process

**Race Condition:**
- Terjadi ketika hasil bergantung pada timing/urutan thread
- `counter++` adalah 3 instruksi di level assembly
- Interleaving bisa membuat increment hilang (lost update)
- Non-deterministic, sulit di-debug

**Critical Section:**
- Kode yang akses shared resource
- Harus memenuhi: Mutual Exclusion, Progress, Bounded Waiting
- Naive attempts dengan flag/turn tidak memenuhi semua syarat

### 4.2 — Checklist Keterampilan (5 menit)

Mahasiswa dapat:
- [ ] Menjelaskan perbedaan thread vs process dengan tabel
- [ ] Menyebutkan apa yang di-share dan private dalam thread
- [ ] Menulis dan membaca kode sederhana dengan pthread
- [ ] Menjelaskan mengapa `counter++` tidak aman secara concurrent
- [ ] Menggambar diagram interleaving yang menyebabkan lost update
- [ ] Mendefinisikan race condition dan critical section
- [ ] Menyebutkan dan menjelaskan 3 syarat solusi critical section

### 4.3 — Kuis Penutup (5 menit)

**Soal 1:** Sebutkan 3 hal yang di-*share* dan 2 hal yang *private* per thread!

**Soal 2:** Thread 1 dan Thread 2 keduanya menjalankan `x = x + 1` dimana `x` adalah global variable yang mulai dari 0. Jika masing-masing thread melakukannya satu kali, nilai `x` setelah keduanya selesai bisa berapa saja? Jelaskan!

**Soal 3:** Sebuah solusi critical section memastikan hanya 1 thread yang bisa masuk, namun jika Thread 1 terus-menerus keluar-masuk, Thread 2 tidak pernah bisa masuk. Syarat mana yang dilanggar?

---

## Catatan Pengajaran

- **Demonstrasi live:** Sangat direkomendasikan untuk menjalankan kode `counter.c` di depan kelas dan menunjukkan output yang berbeda-beda setiap kali run. Efek "wow" ini sangat efektif untuk membangun intuisi.
- **Visualisasi interleaving:** Diagram timeline thread adalah alat bantu terpenting di pertemuan ini. Gambarkan step-by-step di papan.
- **Analogi:** Gunakan analogi dapur dan papan tulis untuk memperkuat intuisi sebelum masuk ke istilah teknis.
- **Teaser minggu depan:** Akhiri dengan memotivasi perlunya Lock/Mutex — buat mahasiswa penasaran.

---

## Referensi

- OSTEP Chapter 26: https://pages.cs.wisc.edu/~remzi/OSTEP/threads-intro.pdf
- POSIX Threads Tutorial: https://computing.llnl.gov/tutorials/pthreads/

---

## Slide Outline (38 Slide)

> **Prinsip desain:** Setiap slide hanya membawa **satu ide**. Transisi antar slide dibuat kecil sehingga mahasiswa yang kesulitan tidak kehilangan konteks.

---

### BAGIAN 0 — Pembuka & Motivasi (Slide 1–5)

---

**Slide 1 — Judul**
- Judul: *Concurrency: An Introduction*
- Mata kuliah, pertemuan ke-10, tanggal
- Referensi: OSTEP Chapter 26

---

**Slide 2 — Agenda Hari Ini**
- Daftar topik dengan ikon/nomor:
  1. Mengapa concurrency penting?
  2. Apa itu thread?
  3. Race condition
  4. Critical section
- Estimasi waktu tiap bagian

---

**Slide 3 — Dunia Nyata Tidak Sekuensial**
- Visual: tiga kotak berjalan "bersamaan":
  - Browser → download file | render halaman | handle input
  - Web server → request A | request B | request C
  - Program → core 1 | core 2 | core 3
- Pesan: *"Program kita harus bisa melakukan banyak hal sekaligus."*

---

**Slide 4 — Analogi Dapur**
- Ilustrasi: dua chef, satu dapur
  - Chef = **thread**
  - Dapur & bahan = **shared memory**
  - Resep masing-masing = **stack & register private**
- Pertanyaan pemantik: *"Apa yang bisa salah kalau dua chef ambil telur terakhir di waktu yang sama?"*

---

**Slide 5 — Pertanyaan Kunci Hari Ini**
- Tiga pertanyaan besar (satu per baris, besar):
  1. Apa itu thread dan bedanya dari proses?
  2. Apa yang dimaksud race condition?
  3. Apa itu critical section dan bagaimana melindunginya?

---

### BAGIAN 1 — Thread Abstraction (Slide 6–16)

---

**Slide 6 — Satu Kalimat Definisi Thread**
- Satu kalimat besar di tengah slide:
  > *"Thread adalah unit eksekusi di dalam sebuah proses."*
- Sub-teks: bisa ada banyak thread dalam satu proses; semuanya berjalan "bersama".

---

**Slide 7 — Masalah dengan Pure Process**
- Tiga poin sederhana:
  - `fork()` → copy **seluruh** address space → mahal
  - Komunikasi antar proses (IPC) → kompleks
  - Context switch antar proses → berat
- Pesan: *"Kita butuh sesuatu yang lebih ringan."*

---

**Slide 8 — Thread Sebagai Solusi**
- Visualisasi sederhana:
  ```
  [Proses A]
    ├── Thread 1  ──►  berjalan
    ├── Thread 2  ──►  berjalan
    └── Thread 3  ──►  berjalan
  ```
- Satu address space, banyak jalur eksekusi.

---

**Slide 9 — Process vs Thread: Tabel Perbandingan**
- Tampilkan tabel (5 baris, highlight perbedaan utama):

  | Aspek | Process | Thread |
  |-------|---------|--------|
  | Address space | Sendiri | Berbagi |
  | Biaya buat | Besar | Kecil |
  | Context switch | Berat | Ringan |
  | Komunikasi | IPC | Shared memory |
  | Crash | Isolasi | Bisa crash semua |

---

**Slide 10 — Apa yang Di-Share Antar Thread?**
- Judul: *"Yang BERBAGI (shared)"*
- Visual address space, bagian yang disorot:
  - Code segment
  - Heap
  - Global & static variables
  - File descriptors
- Analogi: *"Buku resep, kompor, kulkas — dipakai bersama."*

---

**Slide 11 — Apa yang Private per Thread?**
- Judul: *"Yang MILIK SENDIRI (private)"*
- Visual address space, bagian lain yang disorot:
  - Stack (variabel lokal, return address)
  - Register (termasuk PC)
  - Thread ID
- Analogi: *"Tangan masing-masing chef — gerakannya independen."*

---

**Slide 12 — Visualisasi Address Space**
```
+-------------------------+
|   Code Segment (shared) |
+-------------------------+
|   Heap (shared)         |
+-------------------------+
|   Stack Thread 2        |  ← private
+-------------------------+
|   Stack Thread 1        |  ← private
+-------------------------+
```
- Highlight warna: shared = biru, private = oranye

---

**Slide 13 — Thread Control Block (TCB)**
- TCB = "KTP" sebuah thread, analogus dengan PCB pada proses
- Isi TCB:
  - Program Counter (PC)
  - Stack Pointer
  - Register state
  - Thread ID
  - State (Running/Ready/Blocked)

---

**Slide 14 — Thread States**
- Diagram state sederhana (sama seperti proses):
  ```
  READY ──► RUNNING ──► BLOCKED
    ▲           │
    └───────────┘ (preempt / yield)
  ```
- Context switch antar thread: simpan TCB lama, muat TCB baru.

---

**Slide 15 — Kode: hello_thread.c**
- Tampilkan kode lengkap dengan highlight pada:
  - `pthread_create` → membuat thread
  - `pthread_join` → menunggu thread selesai
- Font besar, maksimal satu layar.

---

**Slide 16 — Non-Determinism: Pertanyaan**
- Pertanyaan di tengah slide:
  > *"Apakah urutan output 'Thread A' dan 'Thread B' selalu sama?"*
- Jawaban: **Tidak.** Bergantung pada scheduler OS.
- Istilah baru: **non-determinism** — program yang sama bisa menghasilkan urutan berbeda.

---

### BAGIAN 2 — Race Condition (Slide 17–28)

---

**Slide 17 — Kode: counter.c**
- Tampilkan kode (font besar):
  - `int counter = 0;` → sorot "shared!"
  - Loop `counter++` 1.000.000 kali
  - Dua thread, lalu `printf("counter = %d")`
- Jangan beri jawaban dulu.

---

**Slide 18 — Pertanyaan: Berapa Outputnya?**
- Hanya satu pertanyaan besar:
  > *"Dua thread, masing-masing increment 1.000.000 kali. Hasil counter = ?"*
- Beri waktu kelas menjawab.

---

**Slide 19 — Hasil Nyata (Demo)**
- Tampilkan beberapa output nyata:
  - Run 1: `counter = 1.234.512`
  - Run 2: `counter = 1.876.023`
  - Run 3: `counter = 1.543.781`
- Catatan merah: **TIDAK pernah 2.000.000!**
- Pertanyaan: *"Mengapa?"*

---

**Slide 20 — counter++ Bukan Satu Instruksi**
- Dua kolom:
  - Kiri (kode C): `counter++`
  - Kanan (assembly): tiga baris
- Pesan utama: **satu baris C = banyak instruksi mesin.**

---

**Slide 21 — Assembly: Langkah 1 — LOAD**
```assembly
MOV eax, [counter]   ; baca nilai counter dari memori → register
```
- Ilustrasi: nilai dari RAM dipindah ke "kantong" (register) thread.
- Highlight langkah ini saja; langkah lain abu-abu.

---

**Slide 22 — Assembly: Langkah 2 — ADD**
```assembly
ADD eax, 1           ; tambahkan 1 di dalam register
```
- Ilustrasi: isi kantong ditambah 1.
- Catatan: *nilai di memori belum berubah!*

---

**Slide 23 — Assembly: Langkah 3 — STORE**
```assembly
MOV [counter], eax   ; tulis hasil kembali ke memori
```
- Ilustrasi: hasil dari kantong disimpan kembali ke RAM.
- Baru sekarang memori berubah.

---

**Slide 24 — Interleaving: Setup Skenario**
- `counter = 50` (nilai awal)
- Thread 1 dan Thread 2 keduanya akan melakukan `counter++`
- Pertanyaan: *"Apa yang terjadi kalau OS menyela Thread 1 di tengah-tengah?"*

---

**Slide 25 — Interleaving: Step-by-Step**
- Tabel dua kolom (Thread 1 | Thread 2), animasi per baris:

  | Thread 1 | Thread 2 | Memori |
  |----------|----------|--------|
  | LOAD eax=50 | | counter=50 |
  | *(di-preempt)* | LOAD eax=50 | counter=50 |
  | | ADD eax=51 | counter=50 |
  | | STORE counter=51 | **counter=51** |
  | ADD eax=51 | | counter=51 |
  | STORE counter=51 | | **counter=51** ← salah! |

---

**Slide 26 — Hasil: Lost Update**
- Highlight merah: counter seharusnya **52**, hasilnya **51**.
- Satu increment **hilang**!
- Kalau ini terjadi jutaan kali → hasil sangat melenceng.

---

**Slide 27 — Definisi Race Condition**
- Kotak definisi besar:
  > *"Race condition terjadi ketika hasil eksekusi program bergantung pada timing atau urutan thread yang tidak bisa dikontrol."*
- Empat karakteristik (bullet pendek):
  - Non-deterministic
  - Sulit direproduksi
  - Bergantung pada scheduler & kecepatan CPU
  - Sering muncul di beban tinggi

---

**Slide 28 — Latihan: Race Condition atau Tidak?**
- Soal 1: `balance_A -= amount; balance_B += amount;` (shared variables)
  - Jawaban: **Ya**, race condition.
- Soal 2: `int local = x * 2;` (local variable)
  - Jawaban: **Tidak**, variabel lokal ada di stack private.
- Kunci: *shared atau private?*

---

### BAGIAN 3 — Critical Section (Slide 29–37)

---

**Slide 29 — Definisi Critical Section**
- Kotak definisi:
  > *"Critical section adalah bagian kode yang mengakses shared resource dan tidak boleh dijalankan oleh lebih dari satu thread secara bersamaan."*

---

**Slide 30 — Contoh: CS vs Bukan CS**
- Dua kolom:
  - Kiri (Critical Section): `counter++` → akses shared variable
  - Kanan (Bukan Critical Section): `int temp = x * 2;` → variabel lokal
- Visual highlight: CS = merah, bukan CS = hijau

---

**Slide 31 — Visualisasi Timeline Critical Section**
```
Thread 1: [── non-critical ──] [■ CRITICAL ■] [── non-critical ──]
Thread 2: [── non-critical ──]               [■ CRITICAL ■]
                                ↑
                         tidak boleh overlap!
```

---

**Slide 32 — Tiga Syarat Solusi: Overview**
- Satu slide, tiga kotak besar:
  1. **Mutual Exclusion** — hanya 1 thread di CS
  2. **Progress** — keputusan masuk CS tidak mandek
  3. **Bounded Waiting** — tidak ada yang menunggu selamanya

---

**Slide 33 — Syarat 1: Mutual Exclusion**
- Definisi sederhana: *"Kalau Thread 1 ada di CS, Thread 2 harus menunggu di luar."*
- Visual: pintu CS hanya boleh dibuka satu thread.
- Jika dilanggar → **race condition terjadi**.

---

**Slide 34 — Syarat 2: Progress**
- Definisi sederhana: *"Kalau CS kosong dan ada yang mau masuk, harus ada yang dipilih masuk — tidak boleh semua menunggu terus."*
- Jika dilanggar → **deadlock**: semua thread saling menunggu, tidak ada yang maju.

---

**Slide 35 — Syarat 3: Bounded Waiting**
- Definisi sederhana: *"Setelah thread minta masuk, ada batas maksimal berapa kali thread lain bisa 'mendahului'."*
- Jika dilanggar → **starvation**: satu thread tidak pernah dapat giliran.

---

**Slide 36 — Naive Attempt 1: Flag Variable**
```c
flag[0] = 1;           // Thread 0: nyatakan niat
while (flag[1]);       // tunggu Thread 1
// ... critical section ...
flag[0] = 0;
```
- Masalah: kedua thread set flag → keduanya menunggu selamanya → **deadlock**.
- Syarat yang gagal: **Progress**.

---

**Slide 37 — Naive Attempt 2: Turn Variable**
```c
while (turn != 0);    // Thread 0: tunggu giliran
// ... critical section ...
turn = 1;             // serahkan giliran ke Thread 1
```
- Masalah: Thread 1 tidak bisa masuk dua kali berturut-turut tanpa Thread 0 memberi giliran.
- Syarat yang gagal: **Progress**.

---

### BAGIAN 4 — Ringkasan & Penutup (Slide 38–40 + bonus)

---

**Slide 38 — Rangkuman: Tiga Tema Hari Ini**
- Tiga kolom atau tiga baris ringkas:

  | Topik | Inti |
  |-------|------|
  | Thread | Unit eksekusi dalam proses; share heap/code, private stack/register |
  | Race Condition | Hasil program bergantung timing; `counter++` = 3 instruksi |
  | Critical Section | Kode akses shared resource; butuh ME + Progress + Bounded Waiting |

---

**Slide 39 — Preview Minggu Depan: Lock / Mutex**
- Pertanyaan pemantik:
  > *"Kalau flag dan turn tidak cukup, apa yang kita butuhkan?"*
- Jawaban: **dukungan dari hardware dan OS** → **Lock / Mutex**
- Teaser: atomic instructions, `pthread_mutex_lock`, `pthread_mutex_unlock`

---

**Slide 40 — Kuis Penutup (3 Soal)**
- **Soal 1:** Sebutkan 3 hal yang di-share dan 2 hal yang private per thread!
- **Soal 2:** `x = x + 1` dijalankan dua thread, `x` mulai dari 0. Nilai `x` bisa berapa saja setelah keduanya selesai? Jelaskan!
- **Soal 3:** Solusi CS menjamin hanya 1 thread di CS, tapi Thread 2 tidak pernah bisa masuk karena Thread 1 terus keluar-masuk. Syarat mana yang dilanggar?

---

### Ringkasan Distribusi Slide

| Bagian | Slide | Jumlah |
|--------|-------|--------|
| Pembuka & Motivasi | 1–5 | 5 slide |
| Thread Abstraction | 6–16 | 11 slide |
| Race Condition | 17–28 | 12 slide |
| Critical Section | 29–37 | 9 slide |
| Ringkasan & Penutup | 38–40 | 3 slide |
| **Total** | | **40 slide** |
