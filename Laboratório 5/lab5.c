// Nome - Ricardo P. Silva Filho
// DRE - 118068055

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#define MAX_LINES 100 // Tamanho máximo de linhas
#define MAX_LINE_LENGTH 1024 // Tamanho máximo de uma linha

char buffer[MAX_LINES][MAX_LINE_LENGTH]; // Buffer compartilhado
int line_count = 0; // Contador de linhas no buffer
int eof = 0; // Indica se o arquivo de entrada chegou ao fim
sem_t empty, full; // Semáforos para controle de acesso ao buffer

// Função para o produtor (thread principal)
void* producer(void* filename) {
    FILE* file = fopen((char*)filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        sem_wait(&empty); // Aguarda espaço vazio no buffer
        strcpy(buffer[line_count], line); // Insere a linha no buffer
        line_count++;
        sem_post(&full); // Sinaliza que o buffer não está vazio
    }

    eof = 1; // Indica que o arquivo de entrada chegou ao fim
    fclose(file);
    pthread_exit(NULL);
}

// Função para os consumidores
void* consumer(void* tid) {
    int thread_id = *(int*)tid;
    while (1) {
        sem_wait(&full); // Aguarda o buffer não estar vazio
        if (eof && line_count == 0) {
            break; // Se o arquivo de entrada acabou e o buffer está vazio, a thread pode encerrar
        }
        printf("Thread %d: %s", thread_id, buffer[0]); // Imprime a linha
        for (int i = 0; i < line_count - 1; i++) {
            strcpy(buffer[i], buffer[i + 1]); // Move as linhas restantes para cima
        }
        line_count--;
        sem_post(&empty); // Sinaliza que há espaço vazio no buffer
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <nome_do_arquivo> <num_threads>\n", argv[0]);
        return 1;
    }

    char* filename = argv[1];
    int num_threads = atoi(argv[2]);

    pthread_t producer_thread;
    pthread_t consumer_threads[num_threads];
    int thread_ids[num_threads];

    sem_init(&empty, 0, MAX_LINES); // Inicializa o semáforo empty com o número máximo de linhas no buffer
    sem_init(&full, 0, 0); // Inicializa o semáforo full como 0 (buffer vazio)

    // Cria a thread produtora
    if (pthread_create(&producer_thread, NULL, producer, (void*)filename)) {
        perror("Erro ao criar a thread produtora");
        return 2;
    }

    // Cria as threads consumidoras
    for (int i = 0; i < num_threads; i++) {
        thread_ids[i] = i + 1;
        if (pthread_create(&consumer_threads[i], NULL, consumer, (void*)&thread_ids[i])) {
            perror("Erro ao criar as threads consumidoras");
            return 3;
        }
    }

    // Espera a thread produtora terminar
    if (pthread_join(producer_thread, NULL)) {
        perror("Erro ao aguardar a thread produtora");
        return 4;
    }

    // Aguarda as threads consumidoras terminarem
    for (int i = 0; i < num_threads; i++) {
        if (pthread_join(consumer_threads[i], NULL)) {
            perror("Erro ao aguardar as threads consumidoras");
            return 5;
        }
    }

    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
