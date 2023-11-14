// Nome - Ricardo P. Silva Filho
// DRE - 118068055

import java.util.LinkedList;

// Classe que define um pool de threads
class FilaTarefas {
    private final int nThreads;
    private final MyPoolThreads[] threads;
    private final LinkedList<Runnable> queue;
    private boolean shutdown;
    private int count;

    // Construtor
    public FilaTarefas(int nThreads) {
        this.shutdown = false;
        this.nThreads = nThreads;
        queue = new LinkedList<>();
        threads = new MyPoolThreads[nThreads];
        for (int i = 0; i < nThreads; i++) {
            threads[i] = new MyPoolThreads();
            threads[i].start();
        }
        count = 0;
    }

    // Método para incrementar a contagem de números primos encontrados
    public synchronized void incrementCount() {
        count++;
    }

    // Método para obter a contagem total de números primos encontrados
    public int getCount() {
        return count;
    }

    // Método para obter o número de threads no pool
    public int getNThreads() {
        return nThreads;
    }

    // Método para adicionar uma tarefa à fila para execução
    public void execute(Runnable r) {
        synchronized (queue) {
            if (this.shutdown) return;
            queue.addLast(r);
            queue.notify();
        }
    }

    // Método para encerrar o pool de threads
    public void shutdown() {
        synchronized (queue) {
            this.shutdown = true;
            queue.notifyAll();
        }
        for (int i = 0; i < nThreads; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                return;
            }
        }
    }

    // Classe interna que define uma thread do pool
    private class MyPoolThreads extends Thread {
        public void run() {
            Runnable r;
            while (true) {
                synchronized (queue) {
                    while (queue.isEmpty() && (!shutdown)) {
                        try {
                            queue.wait();
                        } catch (InterruptedException ignored) {
                        }
                    }
                    if (queue.isEmpty() && shutdown) return;
                    r = queue.removeFirst();
                }
                try {
                    r.run();
                } catch (RuntimeException e) {
                }
            }
        }
    }
}

// Classe principal
public class Main {
    private static final int NTHREADS = 4;

    public static void main(String[] args) {
        // Obtém o valor final a partir do argumento da linha de comando
        int valorFinal = Integer.parseInt(args[0]);

        // Cria um pool de threads
        FilaTarefas pool = new FilaTarefas(NTHREADS);

        // Calcula o tamanho de cada bloco para dividir entre as threads
        long blockSize = (valorFinal + NTHREADS - 1) / NTHREADS;

        // Cria e executa tarefas para cada bloco de números
        for (int i = 0; i < NTHREADS; i++) {
            long blockStart = i * blockSize + 1;
            long blockEnd = Math.min((i + 1) * blockSize, valorFinal);

            Runnable task = new EncontraPrimos(blockStart, blockEnd, pool);
            pool.execute(task);
        }

        // Aguarda o término das threads
        pool.shutdown();

        // Exibe a quantidade total de números primos encontrados
        System.out.println("Quantidade de numeros primos entre 1 e " + valorFinal + ": " + pool.getCount());
    }
}

// Classe que implementa a interface Runnable para encontrar números primos em um intervalo
class EncontraPrimos implements Runnable {
    private final long start;
    private final long end;
    private final FilaTarefas pool;

    // Construtor
    public EncontraPrimos(long start, long end, FilaTarefas pool) {
        this.start = start;
        this.end = end;
        this.pool = pool;
    }

    // Método executado pela thread
    public void run() {
        // Itera sobre os números no intervalo e incrementa a contagem se for primo
        for (long i = start; i <= end; i++) {
            if (ehPrimo(i)) {
                pool.incrementCount();
            }
        }
    }

    // Método para verificar se um número é primo
    private boolean ehPrimo(long n) {
        if (n <= 1) return false;
        if (n == 2 || n == 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;

        // Verifica a primalidade usando a propriedade dos números primos
        for (long i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) return false;
        }

        return true;
    }
}
