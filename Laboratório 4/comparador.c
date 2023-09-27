// Nome - Ricardo P. Silva Filho
// DRE - 118068055

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <quantidade de threads>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    
    // Executar a primeira versão do código
    printf("Executando main_list.c com %d threads:\n", num_threads);
    char command[256];
    snprintf(command, sizeof(command), "./main_list %d", num_threads);
    system(command);

    // Esperar um momento para a próxima execução
    sleep(2);

    // Executar a segunda versão do código
    printf("\nExecutando main_list_rwlock.c com %d threads:\n", num_threads);
    snprintf(command, sizeof(command), "./main_list_rwlock %d", num_threads);
    system(command);

    // Esperar um momento para a próxima execução
    sleep(2);

    // Executar a terceira versão do código
    printf("\nExecutando main_list_custom.c com %d threads:\n", num_threads);
    snprintf(command, sizeof(command), "./main_list_custom %d", num_threads);
    system(command);

    return 0;
}
