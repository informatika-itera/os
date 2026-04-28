/*
 * 01_race.c – Race Condition pada counter++
 * Compile: gcc -Wall -pthread -O0 -o race 01_race.c
 * Jalankan beberapa kali dan amati: hasilnya tidak selalu 2.000.000.
 */
#include <stdio.h>
#include <pthread.h>

#define LOOP 1000000

int counter = 0; /* shared, tidak dilindungi */

void *increment(void *arg) {
    for (int i = 0; i < LOOP; i++) {
        counter++; /* LOAD, ADD, STORE — tidak atomik! */
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
        printf("STATUS: Kebetulan benar — coba jalankan lagi.\n");
    else
        printf("STATUS: Race condition! Kehilangan %d update.\n",
               LOOP * 2 - counter);

    return 0;
}
