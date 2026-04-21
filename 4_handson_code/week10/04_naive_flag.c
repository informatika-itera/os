/*
 * 04_naive_flag.c
 * Week 10 – Percobaan Naif: Flag Sederhana
 *
 * Ini adalah upaya "naif" untuk melindungi critical section
 * menggunakan dua variabel flag. Pendekatan ini GAGAL memenuhi
 * syarat PROGRESS: jika kedua thread men-set flag hampir bersamaan,
 * keduanya akan saling menunggu selamanya (deadlock/livelock).
 *
 * PERINGATAN: Program ini BISA HANG. Tekan Ctrl+C untuk keluar.
 * Jalankan berulang kali — terkadang selesai, terkadang hang.
 *
 * Compile:
 *   gcc -Wall -pthread -O0 -o 04_naive_flag 04_naive_flag.c
 *
 * Jalankan:
 *   ./04_naive_flag   ← mungkin hang!
 *
 * Bandingkan dengan 03_mutex_fix.c yang selalu selesai dengan benar.
 */

#include <stdio.h>
#include <pthread.h>

#define LOOP 500000

int counter = 0;

/* Dua flag: flag[i] = 1 berarti thread i ingin masuk critical section */
volatile int flag[2] = {0, 0};

void *thread0(void *arg) {
    for (int i = 0; i < LOOP; i++) {
        flag[0] = 1;                /* nyatakan niat masuk */
        while (flag[1]);            /* tunggu kalau thread1 di dalam */
        /* ------ critical section ------ */
        counter++;
        /* ------ akhir critical section ------ */
        flag[0] = 0;                /* selesai, beri akses lain */
    }
    return NULL;
}

void *thread1(void *arg) {
    for (int i = 0; i < LOOP; i++) {
        flag[1] = 1;
        while (flag[0]);            /* masalah: jika keduanya set flag
                                       hampir bersamaan → saling tunggu */
        counter++;
        flag[1] = 0;
    }
    return NULL;
}

int main(void) {
    pthread_t t0, t1;

    printf("Memulai... (program mungkin hang, tekan Ctrl+C jika perlu)\n");

    pthread_create(&t0, NULL, thread0, NULL);
    pthread_create(&t1, NULL, thread1, NULL);

    pthread_join(t0, NULL);
    pthread_join(t1, NULL);

    printf("Ekspektasi : %d\n", LOOP * 2);
    printf("Hasil nyata: %d\n", counter);

    if (counter == LOOP * 2)
        printf("STATUS: Nilai benar, tapi flag naif tidak menjamin ini selalu!\n");
    else
        printf("STATUS: Race condition — nilai salah!\n");

    return 0;
}
