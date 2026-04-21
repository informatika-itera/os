# Command: /make-slide

Buat slide kuliah Sistem Operasi baru sesuai dengan panduan di CLAUDE.md proyek ini.

## Cara Pakai

```
/make-slide Week 11: Semaphore dan Mutex
```

atau

```
/make-slide
```

(Claude akan menanyakan topik dan nomor minggu jika tidak diberikan.)

---

## Instruksi untuk Claude

Saat command ini dipanggil, ikuti langkah berikut:

### Langkah 1 — Kumpulkan Informasi

Tanyakan kepada pengguna jika belum jelas:
- Nomor minggu (Week N)
- Topik/judul utama
- Subtopik atau section yang ingin dicakup
- Ada kode program yang perlu ditampilkan? (ya/tidak)
- Ada gambar eksternal yang dibutuhkan? (jika ya, akan dibuat placeholder)

### Langkah 2 — Baca Referensi

Sebelum menulis, baca satu slide yang sudah ada (misal `1_Slides/week9_slide.tex` atau `week10_slide.tex`) sebagai referensi gaya terkini. Juga baca `CLAUDE.md` untuk memastikan tidak ada panduan yang terlewat.

### Langkah 3 — Rancang Struktur

Tentukan daftar section dan frame sebelum menulis kode. Pastikan:
- Ada section yang cukup (biasanya 4–6 section per pertemuan)
- Ada frame checkpoint di akhir setiap section utama
- Ada analogi/contoh sehari-hari untuk setiap konsep abstrak
- Ada frame aktivitas kelas jika topiknya cocok untuk diskusi
- Slide penutup "Ada Pertanyaan?"

### Langkah 4 — Tulis File LaTeX

Tulis file ke `1_Slides/weekN_slide.tex` dengan mengikuti SEMUA panduan di CLAUDE.md:

**Checklist wajib:**
- [ ] Paket dan tema sesuai template standar
- [ ] `\tem{}` untuk semua istilah kunci
- [ ] Density slide tidak terlalu penuh (gunakan `\small` jika perlu)
- [ ] Block environments dipakai dengan tepat (block/alertblock/exampleblock)
- [ ] Kolom dua (`\begin{columns}`) dipakai untuk konten yang bisa displit
- [ ] Diagram TikZ untuk visualisasi (bukan placeholder) kecuali memang butuh gambar foto
- [ ] Frame checkpoint per section
- [ ] Bahasa Indonesia yang mudah dimengerti dengan analogi

**Catatan figure:**
- Jika butuh diagram/flowchart → buat dengan TikZ
- Jika butuh ilustrasi dari buku → cek `2_Book_Translate/figures/`
- Jika butuh foto/gambar eksternal → buat placeholder TikZ dengan deskripsi

### Langkah 5 — Review

Setelah menulis, review sendiri:
- Apakah ada frame yang terlalu penuh (lebih dari ~15 bullet atau teks sangat kecil)?
- Apakah overfull hbox sangat parah? (Perbaiki jika teks keluar frame secara visual)
- Apakah sudah ada analogi untuk setiap konsep abstrak?
- Apakah urutan logis: masalah → konsep → mekanisme → contoh?

Laporkan hasilnya ke pengguna: nama file yang dibuat, daftar section, dan jika ada placeholder figure yang perlu dicarikan gambarnya.
