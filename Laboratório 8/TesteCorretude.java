// Nome - Ricardo P. Silva Filho
// DRE - 118068055

import java.util.concurrent.atomic.AtomicInteger;

public class TesteCorretude {

    public static void main(String[] args) {
        // Defina diferentes valores de N para testar
        int[] valoresN = {1, 5, 10, 50, 100, 500, 1000, 5000, 10000};

        for (int n : valoresN) {
            // Execute a versão concorrente
            int primosConcorrente = encontrarPrimosConcorrente(n);

            // Execute a versão sequencial
            int primosSequencial = encontrarPrimosSequencial(n);

            // Verifique se os resultados coincidem
            if (primosConcorrente == primosSequencial) {
                System.out.println("Teste para N = " + n + " passou. Resultado: " + primosConcorrente);
            } else {
                System.out.println("Teste para N = " + n + " falhou. Resultado concorrente: " + primosConcorrente +
                        ", Resultado sequencial: " + primosSequencial);
            }
        }
    }

    private static int encontrarPrimosConcorrente(int n) {
        AtomicInteger contadorPrimos = new AtomicInteger(0);

        FilaTarefas pool = new FilaTarefas(4);

        for (int i = 1; i <= n; i++) {
            final int numero = i;

            pool.execute(() -> {
                if (ehPrimo(numero)) {
                    contadorPrimos.incrementAndGet();
                }
            });
        }

        pool.shutdown();

        return contadorPrimos.get();
    }

    private static int encontrarPrimosSequencial(int n) {
        int contadorPrimos = 0;

        for (int i = 1; i <= n; i++) {
            if (ehPrimo(i)) {
                contadorPrimos++;
            }
        }

        return contadorPrimos;
    }

    private static boolean ehPrimo(long n) {
        if (n <= 1) return false;
        if (n == 2) return true;
        if (n % 2 == 0) return false;
        for (long i = 3; i <= Math.sqrt(n); i += 2) {
            if (n % i == 0) return false;
        }
        return true;
    }
}
