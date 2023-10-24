# Nome - Ricardo P. Silva Filho
# DRE - 118068055

class LE:
    def __init__(self):
        self.escritores = 0
        self.leitores = 0
        self.escritoresEsperando = 0

    def leitorBloqueado(self, id):
        if self.escritores > 0 or self.escritoresEsperando > 0:
            print("ERRO: leitor " + str(id) + " bloqueado quando há escritores!")

    def escritorBloqueado(self, id):
        if self.escritores > 0 or self.leitores > 0:
            print("ERRO: escritor " + str(id) + " bloqueado quando há leitores ou escritores!")

    def leitorLendo(self, id):
        if self.escritores > 0:
            print("ERRO: leitor " + str(id) + " está lendo quando há escritor escrevendo!")
        self.leitores += 1

    def escritorEscrevendo(self, id):
        if self.escritores > 0 or self.leitores > 0:
            print("ERRO: escritor " + str(id) + " está escrevendo quando há leitores ou escritores!")
        self.escritores += 1

    def leitorSaindo(self, id):
        self.leitores -= 1
        if self.leitores == 0:
            print("Todos os leitores saíram, liberando escritores.")
            self.notifyAll()

    def escritorSaindo(self, id):
        self.escritores -= 1
        if self.escritores == 0:
            print("Último escritor saiu, liberando leitores.")
            self.notifyAll()
        elif self.escritoresEsperando > 0:
            print("Escritor saindo, permitindo próximo escritor.")
            self.notify()

