/*
 * 02_race_condition.c
 * Week 10 – Race Condition
 *
 * Demonstrasi: dua thread masing-masing melakukan 1.000.000 increment
 * pada satu variabel shared tanpa proteksi apapun.
 *
 * EKSPEKTASI: counter = 2000000
 * KENYATAAN : nilai akhir tidak deterministik (bisa < 2000000)
 *
 * Kenapa? Karena counter++ bukan satu instruksi mesin:
 *   1. LOAD  – baca nilai dari memori ke register
 *   2. ADD   – tambah 1 di register
 *   3. STORE – tulis balik ke memori
 * Dua thread bisa menyela satu sama lain di antara langkah-langkah ini
 * (lost update / interleaving).
 *
 * Compile:
 *   gcc -Wall -pthread -O0 -o 02_race_condition 02_race_condition.c
 * (-O0 mematikan optimasi agar race condition lebih mudah terlihat)
 *
 * Jalankan beberapa kali dan perhatikan nilainya berubah:
 *   ./02_race_condition
 */

#include <stdio.h>
#include <pthread.h>

#define LOOP 1000000

int counter = 0; /* shared variable */

void *increment(void *arg) {
    for (int i = 0; i < LOOP; i++) {
        counter++; /* LOAD, ADD, STORE — TIDAK atomik! */
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
        printf("STATUS: Kebetulan benar (coba jalankan lagi)\n");
    else
        printf("STATUS: Race condition terdeteksi! Kehilangan %d update.\n",
               LOOP * 2 - counter);

    return 0;
}
