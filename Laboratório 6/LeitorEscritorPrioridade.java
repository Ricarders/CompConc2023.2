// Nome - Ricardo P. Silva Filho
// DRE - 118068055

class LE {
    private int leit, escr, escrEsperando;  

    public LE() {
        this.leit = 0;
        this.escr = 0;
        this.escrEsperando = 0;
    } 

    public synchronized void EntraLeitor(int id) {
        try { 
            while (this.escr > 0 || this.escrEsperando > 0) {
                System.out.println("le.leitorBloqueado("+id+")");
                wait();
            }
            this.leit++; 
            System.out.println("le.leitorLendo("+id+")");
        } catch (InterruptedException e) { }
    }

    public synchronized void SaiLeitor(int id) {
        this.leit--;
        if (this.leit == 0) {
            this.notifyAll(); // libera escritores (caso existam escritores esperando)
        }
        System.out.println("le.leitorSaindo("+id+")");
    }

    public synchronized void EntraEscritor(int id) {
        try { 
            this.escrEsperando++;
            while (this.leit > 0 || this.escr > 0) {
                System.out.println("le.escritorBloqueado("+id+")");
                wait();
            }
            this.escrEsperando--;
            this.escr++;
            System.out.println("le.escritorEscrevendo("+id+")");
        } catch (InterruptedException e) { }
    }

    public synchronized void SaiEscritor(int id) {
        this.escr--;
        notifyAll(); // libera leitores e escritores (caso existam leitores ou escritores esperando)
        System.out.println("le.escritorSaindo("+id+")");
    }
}

class Leitor extends Thread {
    int id;
    LE monitor;

    public Leitor(int id, LE m) {
        this.id = id;
        this.monitor = m;
    }

    public void run() {
        double j = 777777777.7, i;
        for (;;) {
            this.monitor.EntraLeitor(this.id);
            for (i = 0; i < 100000000; i++) { j = j / 2; }
            this.monitor.SaiLeitor(this.id);
        }
    }
}

class Escritor extends Thread {
    int id;
    LE monitor;

    public Escritor(int id, LE m) {
        this.id = id;
        this.monitor = m;
    }

    public void run() {
        double j = 777777777.7, i;
        for (;;) {
            this.monitor.EntraEscritor(this.id); 
            for (i = 0; i < 100000000; i++) { j = j / 2; }
            this.monitor.SaiEscritor(this.id); 
        }
    }
}

class LeitorEscritorPrioridade {
    static final int L = 4;
    static final int E = 3;

    public static void main(String[] args) {
        int i;
        LE monitor = new LE();
        Leitor[] l = new Leitor[L];
        Escritor[] e = new Escritor[E];

        System.out.println("import verificaLEPrioridade");
        System.out.println("le = verificaLEPrioridade.LE()");

        for (i = 0; i < L; i++) {
            l[i] = new Leitor(i + 1, monitor);
            l[i].start();
        }
        for (i = 0; i < E; i++) {
            e[i] = new Escritor(i + 1, monitor);
            e[i].start();
        }
    }
}
