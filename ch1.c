#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

// Spin for 'howlong' seconds using busy-waiting
void Spin(int howlong) {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    double elapsed = 0.0;
    while (elapsed < (double)howlong) {
        gettimeofday(&end, NULL);
        elapsed = (double)(end.tv_sec - start.tv_sec) + 
                  (double)(end.tv_usec - start.tv_usec) / 1000000.0;
    }
}

// Thread argument structure
typedef struct {
    int thread_id;
    char *message;
} thread_arg_t;

// Thread function
void *worker(void *arg) {
    thread_arg_t *targ = (thread_arg_t *)arg;
    while (1) {
        Spin(1);
        printf("[PID: %d | Thread-%d] %s\n", getpid(), targ->thread_id, targ->message);
        fflush(stdout);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: cpu <string>\n");
        exit(1);
    }
    
    printf("=== Multi-threaded Demo ===\n");
    printf("PID: %d\n", getpid());
    printf("Creating 2 threads...\n\n");
    fflush(stdout);
    
    pthread_t t1, t2;
    
    // Setup arguments for each thread
    thread_arg_t arg1 = { .thread_id = 1, .message = argv[1] };
    thread_arg_t arg2 = { .thread_id = 2, .message = argv[1] };
    
    // Create threads
    pthread_create(&t1, NULL, worker, &arg1);
    pthread_create(&t2, NULL, worker, &arg2);
    
    // Wait for threads (will never return since threads loop forever)
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    return 0;
}