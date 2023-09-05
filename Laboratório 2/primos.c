// Nome - Ricardo P. Silva Filho
// DRE - 118068055

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "timer.h"

long long int N;
int num_threads;
int *primos;

// Função para verificar se um número é primo
int ehPrimo(long long int n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (long long int i = 3; i <= sqrt(n); i += 2)
        if (n % i == 0) return 0;
    return 1;
}

// Função para encontrar números primos em um intervalo e contar a quantidade de primos encontrados
void *encontraPrimos(void *arg) {
    int thread_id = *(int *)arg;
    long long int start = (thread_id * N) / num_threads + 1;
    long long int end = ((thread_id + 1) * N) / num_threads;
    int count = 0;

    for (long long int i = start; i <= end; i++) {
        if (ehPrimo(i)) {
            count++;
        }
    }

    primos[thread_id] = count;

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <N> <num_threads>\n", argv[0]);
        return 1;
    }

    double inicio, fim, delta;

    N = atoll(argv[1]);
    num_threads = atoi(argv[2]);

    primos = (int *)malloc(num_threads * sizeof(int));

    pthread_t *threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
    int *thread_ids = (int *)malloc(num_threads * sizeof(int));

    GET_TIME(inicio)

    for (int i = 0; i < num_threads; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, encontraPrimos, &thread_ids[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    int total_primos = 0;
    for (int i = 0; i < num_threads; i++) {
        total_primos += primos[i];
    }

    GET_TIME(fim)   
    delta = fim - inicio;
    printf("Tempo de processamento:%lf\n", delta);

    printf("Total de números primos de 1 a %lld: %d\n", N, total_primos);

    free(primos);
    free(threads);
    free(thread_ids);

    return 0;
}
