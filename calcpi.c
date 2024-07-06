#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> // biblioteca de manipulação de threads
#include <math.h> // operações matemáticas
#include <time.h> // medição de tempo

#define NUM_THREADS 32 // número de threads
#define NUM_TERMS 1000000000 // 1 bilhão de termos

// vetor para armazenar as somas parciais calculadas por cada thread
double partial_sums[NUM_THREADS];

void* leibniz(void* arg) {
    // leibniz(void* arg) - recebe arg do tipo void*, que é um ponteiro genérico
    
    int thread_id = *((int*)arg);
    // (int*)arg - arg é convertido para um ponteiro de int e, dereferenciado para obter um id, indicando a thread que está executando essa função
    
    double sum = 0.0;
    for (int i = thread_id; i < NUM_TERMS; i += NUM_THREADS) {
        sum += (i % 2 == 0 ? 1.0 : -1.0) / (2 * i + 1);
        // i % 2 == 0 ? 1.0 : -1.0 - determina se o termo atual deve ser adicionado ou subtraído da soma, alternando o sinal a cada novo termo
        // 2 * i + 1 - calcula o denominador da fração na série de Leibniz
    }
    partial_sums[thread_id] = sum;
    // cada posição do array partial_sums corresponde ao resultado parcial calculado pela respectiva thread
    
    pthread_exit(0);
}

int main() {
    pthread_t threads[NUM_THREADS]; // armazena os identificadores das threads
    int thread_ids[NUM_THREADS]; // armazena os IDs numéricos das threads
    struct timespec start, end; // estruturas para medir o tempo de execução
    /* struct timespec:
        time_t tv_sec -> armazena o número de segundos.
        long tv_nsec -> armazena o número de nanosegundos.*/ 
    // start - armazena o tempo antes do início do código
    // end - armazena o tempo após conclusão

    clock_gettime(CLOCK_MONOTONIC, &start);
    // Captura o tempo atual (em segundos e nanosegundos)
    // usando o relógio monotônico e o armazena na estrutura start
    
    // criar threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i; // atribuição de IDs (que é o índice)
        pthread_create(&threads[i], NULL, leibniz, &thread_ids[i]);
        // pthread_create - cria nova thread
        // &threads[i] - ponteiro para a posição do array onde o identificador da nova thread será armazenado
        // NULL - atributo padrão da thread
        // leibniz - função que será executada pela thread
        // &thread_ids[i]: ID da thread passado para a função leibniz 
    }

    // sincronizar threads criadas
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        // pthread_join - aguarda conclusão da thread específica
        // threads[i] - Identificador da thread 
        // NULL - ponteiro para o valor de retorno da thread
    }
    double pi = 0.0;

    // soma dos resultados parciais
    for (int i = 0; i < NUM_THREADS; i++) {
        pi += partial_sums[i]; // acumula todas as somas parciais em pi
        // partial_sums[] - array onde cada elemento possui a soma parcial calculada por uma thread específica
    }

    pi *= 4;

    clock_gettime(CLOCK_MONOTONIC, &end);
    // Captura o tempo atual (em segundos e nanosegundos)
    // usando o relógio monotônico e o armazena na estrutura end
    
    // calcula o tempo decorrido
    double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    // end.tv_sec - start.tv_sec - calcula a diferença entre os tempos em segundos
    // (end.tv_nsec - start.tv_nsec) / 1e9 - calcula a diferença entre os tempos em nanossegundos e converte para segundos

    printf("Aproximação de pi: %.15f\n", pi);
    printf("Tempo de execução: %.5f segundos\n", time_taken);
    
    return 0;
}
