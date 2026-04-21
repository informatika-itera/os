/*
 * 01_thread_basic.c
 * Week 10 – Thread Abstraction
 *
 * Demonstrasi: membuat dua thread dengan pthread_create dan menunggu
 * keduanya selesai dengan pthread_join.
 *
 * Compile:
 *   gcc -Wall -pthread -o 01_thread_basic 01_thread_basic.c
 *
 * Jalankan:
 *   ./01_thread_basic
 *
 * Perhatikan: urutan output "Thread A" dan "Thread B" bisa berubah
 * tiap kali dijalankan → ini adalah NON-DETERMINISM.
 */

#include <stdio.h>
#include <pthread.h>

void *my_thread(void *arg) {
    char *name = (char *) arg;
    printf("Thread %s berjalan\n", name);
    return NULL;
}

int main(void) {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, my_thread, "A");
    pthread_create(&t2, NULL, my_thread, "B");

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Main selesai\n");
    return 0;
}
