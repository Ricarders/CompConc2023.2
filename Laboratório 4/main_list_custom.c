// Nome - Ricardo P. Silva Filho
// DRE - 118068055

#include <stdio.h>
#include <stdlib.h>
#include "list_int.h"
#include <pthread.h>
#include "timer.h"

#define QTDE_OPS 10000000 //quantidade de operacoes sobre a lista (insercao, remocao, consulta)
#define QTDE_INI 100 //quantidade de insercoes iniciais na lista
#define MAX_VALUE 100 //valor maximo a ser inserido

//lista compartilhada iniciada 
struct list_node_s* head_p = NULL; 
//qtde de threads no programa
int nthreads;

// Mutex para controle de acesso concorrente
pthread_mutex_t mutex;

// Variável de condição para leitores e escritores
pthread_cond_t cond_leit, cond_escr;

// Contadores para leitores e escritores
int leit = 0, escr = 0;

// Função de entrada para leitura
void EntraLeitura() {
    pthread_mutex_lock(&mutex);
    while (escr > 0) {
        pthread_cond_wait(&cond_leit, &mutex);
    }
    leit++;
    pthread_mutex_unlock(&mutex);
}

// Função de saída para leitura
void SaiLeitura() {
    pthread_mutex_lock(&mutex);
    leit--;
    if (leit == 0) {
        pthread_cond_signal(&cond_escr);
    }
    pthread_mutex_unlock(&mutex);
}

// Função de entrada para escrita
void EntraEscrita() {
    pthread_mutex_lock(&mutex);
    while ((leit > 0) || (escr > 0)) {
        pthread_cond_wait(&cond_escr, &mutex);
    }
    escr++;
    pthread_mutex_unlock(&mutex);
}

// Função de saída para escrita
void SaiEscrita() {
    pthread_mutex_lock(&mutex);
    escr--;
    pthread_cond_signal(&cond_escr);
    pthread_cond_broadcast(&cond_leit);
    pthread_mutex_unlock(&mutex);
}

// Tarefa das threads
void* tarefa(void* arg) {
    long int id = (long int) arg;
    int op;
    int in, out, read; 
    in=out=read = 0; 

    // Realiza operacoes de consulta (98%), insercao (1%) e remocao (1%)
    for(long int i=id; i<QTDE_OPS; i+=nthreads) {
        op = rand() % 100;
        if(op < 98) {
            EntraLeitura();
            Member(i % MAX_VALUE, head_p);   /* Ignore return value */
            SaiLeitura();
            read++;
        } else if(op >= 98 && op < 99) {
            EntraEscrita();
            Insert(i % MAX_VALUE, &head_p);  /* Ignore return value */
            SaiEscrita();
            in++;
        } else if(op >= 99) {
            EntraEscrita();
            Delete(i % MAX_VALUE, &head_p);  /* Ignore return value */
            SaiEscrita();
            out++;
        }
    }
    // Registra a quantidade de operacoes realizadas por tipo
    printf("Thread %ld: in=%d out=%d read=%d\n", id, in, out, read);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    pthread_t *tid;
    double ini, fim, delta;
   
    // Verifica se o numero de threads foi passado na linha de comando
    if(argc < 2) {
        printf("Digite: %s <numero de threads>\n", argv[0]);
        return 1;
    }
    nthreads = atoi(argv[1]);

    // Inicializa as variáveis de condição
    pthread_cond_init(&cond_leit, NULL);
    pthread_cond_init(&cond_escr, NULL);

    // Insere os primeiros elementos na lista
    for(int i = 0; i < QTDE_INI; i++)
        Insert(i % MAX_VALUE, &head_p);  /* Ignore return value */
   
    // Tomada de tempo inicial
    GET_TIME(ini);

    // Aloca espaco de memoria para o vetor de identificadores de threads no sistema
    tid = malloc(sizeof(pthread_t) * nthreads);
    if(tid == NULL) {  
        printf("--ERRO: malloc()\n");
        return 2;
    }

    // Inicializa a variavel mutex
    pthread_mutex_init(&mutex, NULL);
   
    // Cria as threads
    for(long int i = 0; i < nthreads; i++) {
        if(pthread_create(tid + i, NULL, tarefa, (void*) i)) {
            printf("--ERRO: pthread_create()\n");
            return 3;
        }
    }
   
    // Aguarda as threads terminarem
    for(int i = 0; i < nthreads; i++) {
        if(pthread_join(*(tid + i), NULL)) {
            printf("--ERRO: pthread_join()\n");
            return 4;
        }
    }

    // Tomada de tempo final
    GET_TIME(fim);
    delta = fim - ini;
    printf("Tempo: %lf\n", delta);

    // Libera o mutex
    pthread_mutex_destroy(&mutex);
    // Libera as variáveis de condição
    pthread_cond_destroy(&cond_leit);
    pthread_cond_destroy(&cond_escr);
    // Libera o espaco de memoria do vetor de threads
    free(tid);
    // Libera o espaco de memoria da lista
    Free_list(&head_p);

    return 0;
}
