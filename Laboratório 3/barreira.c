// Nome - Ricardo P. Silva Filho
// DRE - 118068055

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int num_threads;
pthread_mutex_t mutex;
pthread_cond_t cond;
int counter = 0;

void barreira() {
    pthread_mutex_lock(&mutex);
    counter++;

    if (counter < num_threads){
        pthread_cond_wait(&cond, &mutex);
    }

    else {
        counter = 0;
        pthread_cond_broadcast(&cond);
    }

    pthread_mutex_unlock(&mutex);
}

void *thread_function(void *thread_id) {
    int id = *(int *)thread_id;

    // Etapa 1
    printf("Olá da thread %d\n", id);

    // Barreira para garantir que todas as threads terminem a Etapa 1 antes de prosseguir
    barreira();

    // Etapa 2
    printf("Que dia bonito %d\n", id);

    // Barreira para garantir que todas as threads terminem a Etapa 2 antes de prosseguir
    barreira();

    // Etapa 3
    printf("Até breve da thread %d\n", id);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <numero_de_threads>\n", argv[0]);
        return 1;
    }

    num_threads = atoi(argv[1]);
    if (num_threads < 1) {
        printf("Número de threads inválido. Use um número positivo.\n");
        return 1;
    }

    pthread_t threads[num_threads];
    int thread_ids[num_threads];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    for (int i = 0; i < num_threads; i++) {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
