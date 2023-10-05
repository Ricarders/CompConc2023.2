// Nome - Ricardo P. Silva Filho
// DRE - 118068055

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#define MAX_LINES 100 // Tamanho máximo de linhas no buffer
#define MAX_LINE_LENGTH 1024 // Tamanho máximo de uma linha
char buffer[MAX_LINES][MAX_LINE_LENGTH]; // Buffer compartilhado
int line_count = 0;  // Contador de linhas no buffer
sem_t mutex, linha, espaco; // Semáforos para controle de acesso ao buffer


// Função para inserir linha no buffer
void into_buffer(char buffer[][MAX_LINE_LENGTH], char *item) {
    sem_wait(&mutex);  // Exclusão mútua no buffer
    strcpy(buffer[line_count], item);  // Buffer recebe a linha lida
    line_count++;
    sem_post(&mutex);  // Libera exclusão mútua
}

// Função para ler linhas do arquivo
void read_lines(FILE *file) {
    char item[MAX_LINE_LENGTH];
    while (fgets(item, sizeof(item), file)) {
        sem_wait(&espaco);  // Espera buffer ter espaço
        into_buffer(buffer, item);
        sem_post(&linha);   // Libera linha pra ser consumida
    }
    
}

void *producer(void *filename) {
    FILE *file = fopen((char *)filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    read_lines(file);  //le linhas do arquivo

    fclose(file);
    sem_post(&linha);  // Sinaliza que o produtor terminou de inserir todas as linhas
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int thread_id = *((int *)arg);
    while (1) {
        sem_wait(&linha);  // Espera o Buffer não estar vazio
        sem_wait(&mutex);  // exclusão mútua no buffer
        
        if (line_count == 0 && sem_trywait(&espaco) == 0) {
            // Se o buffer estiver vazio e o produtor já terminou, encerra a thread
            sem_post(&mutex);
            sem_post(&linha);
            pthread_exit(NULL);
        }
        
        printf("Thread %d: %s", thread_id, buffer[0]);  // Printa a proxima linha no buffer
        for (int i = 0; i < line_count - 1; i++) {
            strcpy(buffer[i], buffer[i + 1]);  // atualiza o buffer
        }
        line_count--;
        sem_post(&mutex);  // Libera exclusão mútua
        sem_post(&espaco);  // Buffer tem espaço
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <nome_do_arquivo> <numero_de_threads>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[2]);

    sem_init(&linha, 0, 0);
    sem_init(&espaco, 0, MAX_LINES);
    sem_init(&mutex, 0, 1);

    pthread_t producer_thread;
    pthread_create(&producer_thread, NULL, producer, (void *)argv[1]);
    pthread_t consumer_threads[num_threads];
    int thread_ids[num_threads];
    for (int i = 0; i < num_threads; i++) {
        thread_ids[i] = i + 1;
        pthread_create(&consumer_threads[i], NULL, consumer, (void *)&thread_ids[i]);
    }

    pthread_join(producer_thread, NULL);
    for (int i = 0; i < num_threads; i++) {
        pthread_join(consumer_threads[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&linha);
    sem_destroy(&espaco);

    return 0;
}
