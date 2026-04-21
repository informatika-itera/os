# AI Guidelines: OS Course Slide Project

Panduan ini digunakan oleh semua AI assistant yang bekerja di proyek ini.
Proyek berisi materi kuliah **Sistem Operasi** untuk Program Studi Teknik Informatika,
Institut Teknologi Sumatera (ITERA), dipublikasikan di **lectura.id**.

---

## Struktur Direktori

```
1_Slides/          — Slide kuliah per pertemuan (weekN_slide.tex)
2_Book_Translate/  — Terjemahan buku referensi OS
  figures/         — Gambar dari buku (dapat dipakai di slide)
3_Tugas/           — Tugas dan kuis
```

---

## Panduan Membuat Slide LaTeX Beamer

### Template Awal (Gunakan Selalu)

```latex
\documentclass[t,aspectratio=169,xcolor=dvipsnames]{beamer}
\usetheme{SimplePlusAIC}

\usepackage{graphicx}
\usepackage{booktabs}
\usepackage{svg}
\usepackage{tcolorbox}
\usepackage{tikz}
\usetikzlibrary{shapes.geometric, arrows, positioning, fit, calc}
\usepackage{makecell}
\usepackage{listings}   % tambahkan jika ada kode program

\newcommand*{\defeq}{\stackrel{\text{def}}{=}}
\usepackage{setspace}
\usepackage[T1]{fontenc}
\usepackage{helvet}
\usepackage{amsmath}
\usepackage{ragged2e}
\usepackage{gensymb}

\usepackage[svgnames,table]{xcolor}
\arrayrulecolor{black}
\setlength{\arrayrulewidth}{0.20mm}
\renewcommand{\arraystretch}{1.35}

% Highlight istilah penting: bold + merah
\newcommand{\tem}[1]{\textbf{\textcolor{red}{#1}}}

% Konfigurasi listings (jika ada kode)
\lstset{
    basicstyle=\ttfamily\small,
    breaklines=true,
    columns=fullflexible,
    frame=none
}
```

### Halaman Judul

```latex
\title[Judul Pendek]{Judul Lengkap Topik}
\subtitle{Week N: Subjudul Deskriptif}
\author[lectura.id/course/os]{lectura.id/course/os}
\institute[ITERA]{Program Studi Teknik Informatika \\ Institut Teknologi Sumatera}
\date{\textcolor{nyublue}{2026}}
```

### Struktur Wajib per File Slide

Setiap file slide harus memiliki frame-frame berikut secara berurutan:

1. **Title frame** — `\titlepage`
2. **Outline frame** — `\tableofcontents`
3. **Capaian Pembelajaran** — daftar tujuan + `exampleblock{Fokus Kelas Ini}`
4. **Section header frame** — satu frame `[plain]` per section (teks besar di tengah)
5. **Frame konten** — isi materi per section
6. **Checkpoint** — cek pemahaman di akhir setiap section besar
7. **Penutup** — frame "Ada Pertanyaan?"

Contoh section header:

```latex
\section{Nama Section}

\begin{frame}[plain]
    \begin{center}
        \vspace{3cm}
        \Huge\textbf{Nama Section}
    \end{center}
\end{frame}
```

Atau gunakan `\AtBeginSection` jika section banyak:

```latex
\AtBeginSection[]{
    \begin{frame}[plain]
        \vfill
        \centering
        {\usebeamerfont{title}\color{nyublue}\Large \insertsectionhead\par}
        \vspace{0.3cm}
        {\large Week N}
        \vfill
    \end{frame}
}
```

---

## Aturan Utama (WAJIB DIIKUTI)

### 1. Slide Tidak Boleh Terlalu Penuh

- Satu slide = satu ide utama.
- Gunakan ukuran font lebih kecil jika konten banyak: `\small` → `\footnotesize` → `\scriptsize` → `\tiny`.
- Beri jarak antar elemen: `\vspace{0.2cm}` atau `\vspace{0.3cm}`.
- Jangan buat bullet list lebih dari 6–7 item; pecah ke slide berikutnya atau ringkas.

### 2. Overfull Hbox

- Overfull hbox kecil (< 10pt) dapat **dibiarkan**.
- Perbaiki hanya jika teks keluar dari batas frame secara visual nyata.
- Jangan memaksakan `\linebreak` atau `\hspace{-...}` hanya untuk menghilangkan warning kecil.

### 3. Bahasa Indonesia yang Mudah Dipahami

- Gunakan bahasa kasual: "kita", "bayangkan", "coba lihat".
- **Wajib sertakan analogi sehari-hari** untuk setiap konsep abstrak.
  - Contoh analogi yang sudah dipakai: dapur restoran (scheduling), apartemen (virtual memory), perpustakaan & catatan saku (TLB), dua chef satu dapur (thread).
- Istilah teknis bahasa Inggris ditulis miring: `\textit{page fault}`, `\textit{context switch}`.
- Urutan penyampaian: **masalah → analogi → konsep → mekanisme → contoh → checkpoint**.

### 4. Block Environments

| Tipe | Warna | Kapan Digunakan |
|------|-------|-----------------|
| `\begin{block}{Judul}` | Biru | Definisi, konsep, rumus |
| `\begin{alertblock}{Judul}` | Merah | Peringatan, tantangan, bahaya |
| `\begin{exampleblock}{Judul}` | Hijau | Contoh, analogi, kesimpulan positif |

### 5. Layout Dua Kolom

Gunakan `\begin{columns}[T]` untuk membagi konten agar slide tidak penuh:

```latex
\begin{columns}[T]
    \column{0.48\textwidth}
    % Konten kiri

    \column{0.48\textwidth}
    % Konten kanan
\end{columns}
```

Rasio yang umum dipakai: `0.48/0.48`, `0.50/0.46`, `0.44/0.52`, `0.40/0.56`.

---

## Figure dan Diagram

### Prioritas Penggunaan

1. **TikZ (utama)** — Buat diagram langsung di LaTeX. Flowchart, state diagram, struktur memori, perbandingan timeline, dll. semua bisa dibuat dengan TikZ.
2. **Gambar dari buku** — Cek `2_Book_Translate/figures/` jika ada gambar relevan.
3. **Placeholder** — Jika butuh foto/ilustrasi eksternal yang tidak tersedia, buat placeholder.

### Pola TikZ yang Konsisten

```latex
% Node styles
box/.style={draw, rounded corners=3pt, minimum width=2.5cm,
            minimum height=0.6cm, align=center}
arr/.style={->, thick}
state/.style={draw, rounded corners=8pt, minimum width=2cm,
              minimum height=0.8cm, align=center}
decision/.style={draw, diamond, aspect=2.5, minimum width=3cm,
                 minimum height=0.7cm, fill=blue!15, align=center}
terminal/.style={draw, rounded corners=8pt, minimum width=2.8cm,
                 minimum height=0.55cm, align=center}

% Palet warna (gunakan konsisten)
% fill=blue!20    → proses/entitas utama
% fill=green!20   → kondisi sukses/baik
% fill=orange!25  → kondisi transisi/menengah
% fill=red!15     → kondisi error/buruk
% fill=yellow!25  → input/user
% fill=gray!10    → elemen netral/background
```

Selalu tambahkan `scale=0.8, transform shape` jika diagram terlalu besar agar tidak meluap.

### Placeholder Figure

Jika butuh gambar eksternal, buat placeholder yang informatif:

```latex
\begin{tikzpicture}
    \draw[dashed, gray, thick] (0,0) rectangle (7, 3.5);
    \node[gray, align=center] at (3.5, 1.75) {
        \small [PLACEHOLDER]\\
        \scriptsize Deskripsi gambar yang dibutuhkan di sini
    };
\end{tikzpicture}
```

### Gambar dari Buku

```latex
\includegraphics[width=\textwidth, height=0.5\textheight, keepaspectratio]
    {../2_Book_Translate/figures/6_1.png}
```

Untuk slide full-image (tanpa frametitle):

```latex
\begin{frame}[plain]
    \vfill
    \centering
    \includegraphics[width=0.93\paperwidth, keepaspectratio]{figures/nama_gambar.png}
    \vfill
\end{frame}
```

---

## Tabel

Gunakan `booktabs` untuk tabel rapi:

```latex
\begin{tabular}{p{0.20\textwidth}p{0.33\textwidth}p{0.37\textwidth}}
    \toprule
    \textbf{Aspek} & \textbf{Opsi A} & \textbf{Opsi B} \\
    \midrule
    Baris 1 & ... & ... \\
    Baris 2 & ... & ... \\
    \bottomrule
\end{tabular}
```

Gunakan `\footnotesize` atau `\scriptsize` pada tabel agar tidak meluap dari slide.

---

## Kode Program

Gunakan `lstlisting` dalam frame `[fragile]`:

```latex
\begin{frame}[fragile]
    \frametitle{Contoh Kode}
    \tiny
    \begin{columns}[T]
        \column{0.54\textwidth}
        \begin{lstlisting}[basicstyle=\ttfamily\scriptsize, breaklines=true]
// kode di sini
        \end{lstlisting}

        \column{0.42\textwidth}
        \begin{block}{Penjelasan}
            Penjelasan kode...
        \end{block}
    \end{columns}
\end{frame}
```

---

## Frame Checkpoint (Wajib di Setiap Section Besar)

```latex
\begin{frame}
    \frametitle{Checkpoint N}
    \framesubtitle{Cek pemahaman sebelum lanjut}
    \small
    \begin{enumerate}
        \item Pertanyaan 1?
        \item Pertanyaan 2?
        \item Pertanyaan 3?
    \end{enumerate}
    \begin{block}{Kunci Jawaban Singkat}
        Ringkasan jawaban...
    \end{block}
\end{frame}
```

---

## Frame Capaian Pembelajaran

```latex
\begin{frame}
    \frametitle{Capaian Pembelajaran}
    \framesubtitle{Tujuan setelah pertemuan selesai}
    \small
    \begin{enumerate}
        \item Tujuan spesifik 1...
        \item Tujuan spesifik 2...
        \item Tujuan spesifik 3...
    \end{enumerate}
    \begin{exampleblock}{Fokus Kelas Ini}
        Kita fokus pada \tem{konsep A}, \tem{konsep B}, dan \tem{konsep C}.
    \end{exampleblock}
\end{frame}
```

---

## Frame Penutup

```latex
\begin{frame}
    \frametitle{Terima Kasih}
    \begin{center}
        \Huge \textbf{Ada Pertanyaan?}

        \vspace{0.8cm}
        \normalsize
        Pesan penutup singkat untuk mahasiswa.

        \vspace{0.8cm}
        \small
        \textit{``Kutipan inspiratif (opsional).''} \\
        \vspace{0.1cm}
        --- Nama Tokoh
    \end{center}
\end{frame}
```

---

## Compile

```bash
# Standar
pdflatex -interaction=nonstopmode 1_Slides/weekN_slide.tex

# Jika pakai minted (syntax highlight)
pdflatex -shell-escape -interaction=nonstopmode 1_Slides/weekN_slide.tex
```

Output PDF disimpan di direktori yang sama dengan file `.tex`.
