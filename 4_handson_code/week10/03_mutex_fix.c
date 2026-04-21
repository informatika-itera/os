/*
 * 03_mutex_fix.c
 * Week 10 – Critical Section (teaser ke minggu depan)
 *
 * Masalah yang sama dengan 02_race_condition.c, tapi diselesaikan
 * menggunakan pthread_mutex untuk melindungi critical section.
 *
 * Mutex memastikan hanya SATU thread yang mengeksekusi counter++
 * pada satu waktu → mutual exclusion terpenuhi.
 *
 * Compile:
 *   gcc -Wall -pthread -O0 -o 03_mutex_fix 03_mutex_fix.c
 *
 * Jalankan:
 *   ./03_mutex_fix
 *
 * Hasilnya harus selalu 2000000.
 */

#include <stdio.h>
#include <pthread.h>

#define LOOP 1000000

int counter = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *increment(void *arg) {
    for (int i = 0; i < LOOP; i++) {
        pthread_mutex_lock(&lock);   /* masuk critical section */
        counter++;                   /* ← protected! */
        pthread_mutex_unlock(&lock); /* keluar critical section */
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock);

    printf("Ekspektasi : %d\n", LOOP * 2);
    printf("Hasil nyata: %d\n", counter);

    if (counter == LOOP * 2)
        printf("STATUS: Benar! Mutex melindungi critical section.\n");
    else
        printf("STATUS: Bug — seharusnya tidak bisa terjadi.\n");

    return 0;
}
