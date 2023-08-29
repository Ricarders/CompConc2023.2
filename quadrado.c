// Nome - Ricardo P. Silva Filho
// DRE - 118068055

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define VECTOR_SIZE 10000
#define NUM_THREADS 8

int vetor[VECTOR_SIZE];


// Função executada pelas threads para elevar ao quadrado os elementos do vetor
void *squareElements(void *arg) {
    int thread_id = *(int *)arg;

    int elements_per_thread = VECTOR_SIZE / NUM_THREADS;
    int start_index = thread_id * elements_per_thread;
    int end_index = start_index + elements_per_thread;

    // Eleva ao quadrado os elementos do vetor atribuídos a esta thread
    for (int i = start_index; i < end_index; i++) {
        vetor[i] = vetor[i] * vetor[i];
    }

    return NULL;
}

// Inicializa o vetor com valores de 1 a VECTOR_SIZE
void initializeVector() {
    for (int i = 0; i < VECTOR_SIZE; i++) {
        vetor[i] = i + 1;
    }
}

// Verifica se os valores no vetor estão corretos após a operação das threads
void checkResult() {
    for (int i = 0; i < VECTOR_SIZE; i++) {
        if (vetor[i] != (i + 1) * (i + 1)) {
            printf("Erro: Valor incorreto na posicao %d\n", i);
            return;
        }
    }
    printf("Todos os valores estao corretos!\n");
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    initializeVector();

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, squareElements, &thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    checkResult();

    return 0;
}