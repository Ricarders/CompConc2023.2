# Nome - Ricardo P. Silva Filho
# DRE - 118068055

import concurrent.futures
import sys

#funcao que sera executada de forma assincrona
def task(n):
    if (n <= 1):
        return 0
    if (n == 2):
        return 1
    if (n % 2 == 0):
        return 0
    for i in range(3, int(n**0.5) + 1, 2):
        if (n % i == 0):
            return 0
    return 1
        

if __name__ == '__main__':

    if len(sys.argv) != 2:
        print("Uso correto: python3 verifica.py <Valor de N>")
        sys.exit(1)


    else:
        max = int(sys.argv[1]) + 1
        #cria um pool de threads OU de processos com no maximo 5 intancias 
        with concurrent.futures.ProcessPoolExecutor(max_workers=5) as executor:
            futures = []
            
            #submete a tarefa para execucao assincrona
            for aux in range(max):
                futures.append(executor.submit(task, aux))
            #recebe os resultados
            total_c = 0
            for future in futures:
                result = future.result()
                total_c = total_c + result

        total_s = 0
        for aux in range(max):
            total_s = total_s + task(aux)

        if total_c == total_s:
            print("Resultado correto")

        else:
            print("Resultado incorreto")        
