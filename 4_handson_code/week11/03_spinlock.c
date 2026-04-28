/*
 * 03_spinlock.c – Spinlock dengan instruksi atomik Test-and-Set
 * Compile: gcc -Wall -pthread -O0 -o spin 03_spinlock.c
 *
 * Bandingkan kecepatan:
 *   time ./mutex
 *   time ./spin
 *
 * Pantau CPU saat spinlock berjalan:
 *   top   (amati CPU usage mendekati 100% per core)
 */
#include <stdio.h>
#include <pthread.h>

#define LOOP 1000000

int counter = 0;
int flag = 0; /* 0 = bebas, 1 = terkunci */

void spin_lock(int *f) {
    /* __sync_lock_test_and_set: baca nilai lama, lalu tulis 1 — atomik */
    while (__sync_lock_test_and_set(f, 1) == 1)
        ; /* putar terus sampai mendapat 0 (berhasil ambil lock) */
}

void spin_unlock(int *f) {
    __sync_lock_release(f); /* tulis 0 secara atomik */
}

void *increment(void *arg) {
    for (int i = 0; i < LOOP; i++) {
        spin_lock(&flag);
        counter++;
        spin_unlock(&flag);
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Ekspektasi : %d\n", LOOP * 2);
    printf("Hasil nyata: %d\n", counter);

    if (counter == LOOP * 2)
        printf("STATUS: Benar! Spinlock (Test-and-Set) berhasil.\n");
    else
        printf("STATUS: Salah — ada bug di kode ini.\n");

    return 0;
}
