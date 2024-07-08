#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

#define NUM_THREADS 16
#define NUM_TERMS 1000000000

double partial_sums[NUM_THREADS];

void* leibniz(void* arg) {
    int thread_id = *((int*) arg);   

    double sum = 0.0;
    for (int i = thread_id; i < NUM_TERMS; i += NUM_THREADS) {
        sum += (i % 2 == 0 ? 1.0 : -1.0) / (2 * i + 1);
    }
    partial_sums[thread_id] = sum;
        
    pthread_exit(0);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, leibniz, &thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    double pi = 0.0;

    for (int i = 0; i < NUM_THREADS; i++) {
        pi += partial_sums[i]; 
    }

    pi *= 4;

    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Número de Threads: %d\n", NUM_THREADS);
    printf("Aproximação de pi: %.15f\n", pi);
    printf("Tempo de execução: %.5f segundos\n", time_taken);
    
    return 0;
}
