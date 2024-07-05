#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 4 // número de threads para calcular PI
#define NUM_POINTS 1000000 // total de pontos gerados para estimar PI

// estrutura para armazenar os dados passados de cada thread
typedef struct {
    int points_inside_circle; // número de pontos que caem dentro do círculo
    int total_points; // número total de pontos que a thread deve processar
} ThreadData;

// função que cada thread executa para calcular pi
void* calculate_pi(void* arg) {
    ThreadData* data = (ThreadData*)arg; // arg = ponteiro para os dados da thread
    int points_inside_circle = 0;
    unsigned int seed = time(NULL) ^ pthread_self(); // gerar semente única para a função de gerar números aleatórios em um ambiente multithread

    // em cada iteração, um ponto é gerado e verificado se está dentro do círculo
    for (int i = 0; i < data->total_points; ++i) {
        // rand_r(&seed) - gera um número pseudo-aleatório usando a semente
        // converte para um ponto flutuante (double) e é dividido pelo valor máximo que rand_r pode gerar
        double x = (double)rand_r(&seed) / RAND_MAX; // gera a coordenada X - intervalo [0, 1]
        double y = (double)rand_r(&seed) / RAND_MAX; // gera a coordenada Y - intervalo [0, 1]
        
        // verifica se o ponto está dentro do círculo
        if (x * x + y * y <= 1.0) { // verifica se a distância ao quadrado é <= 1 (significa que está dentro do círculo)
            points_inside_circle++;
        }
    }

    // armazena o número de pontos dentro do círculo
    data->points_inside_circle = points_inside_circle;
    pthread_exit(NULL); // encerra a thread
}

int main() {
    pthread_t threads[NUM_THREADS]; // array de identificadores de threads
    ThreadData thread_data[NUM_THREADS]; // array de ED para cada thread
    int total_points_per_thread = NUM_POINTS / NUM_THREADS; // número de pontos que cada thread deve processar
    int total_points_inside_circle = 0; // total de pontos que caem dentro do círculo para todas as threads

    // criação das threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_data[i].total_points = total_points_per_thread; // atribuição do nº total de pontos por thread a 'total_points' em 'thread_data' na posição 'i'
        pthread_create(&threads[i], NULL, calculate_pi, (void*)&thread_data[i]);
        // &threads[i] - endereço da variável onde o identificador da thread será armazenado
        // NULL - atributo padrão
        // calculate_pi - função que a thread executará
        // (void*)&thread_data[i] - ponteiro dos dados que serão passados para a função chamada
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL); // espera pela conclusão da thread
        total_points_inside_circle += thread_data[i].points_inside_circle; // acumula todos os pontos dentro do círculo de todas as threads
    }

    // estimativa de pi,  multiplica-se por 4 para obter uma stimativa direta 
    double pi_estimate = 4.0 * total_points_inside_circle / NUM_POINTS;
    printf("Estimated value PI = %f\n", pi_estimate);

    return 0;
}
