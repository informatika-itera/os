/*
 * 02_mutex.c – Perbaikan dengan pthread_mutex
 * Compile: gcc -Wall -pthread -O0 -o mutex 02_mutex.c
 * Hasilnya harus selalu tepat 2.000.000.
 */
#include <stdio.h>
#include <pthread.h>

#define LOOP 1000000

int counter = 0;
pthread_mutex_t lock; /* variabel mutex */

void *increment(void *arg) {
    for (int i = 0; i < LOOP; i++) {
        pthread_mutex_lock(&lock);   /* kunci sebelum masuk */
        counter++;                   /* critical section — aman */
        pthread_mutex_unlock(&lock); /* buka setelah selesai */
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2;

    pthread_mutex_init(&lock, NULL);

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
        printf("STATUS: Salah — ada bug di kode ini.\n");

    return 0;
}
