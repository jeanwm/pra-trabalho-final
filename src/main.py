# importacoes
import os
import random
import subprocess
import csv
from grafico import plot_grafico_esforco

# variaveis
ESFORCO_INSERCAO_CSV = 'auto/esforco_insercao.csv'
ESFORCO_DELECAO_CSV = 'auto/esforco_delecao.csv'
INSERCAO_JPG = 'auto/insercao.jpg'
DELECAO_JPG = 'auto/delecao.jpg'
SUAVIZACAO = 100

# classe
class Operacao:
    INSERCAO = 1
    DELECAO  = 2

# funcoes
def main():
    # verifica pastas
    os.makedirs('build', exist_ok=True)

    if os.path.exists('auto'):
        os.rmdir('auto')

    os.makedirs('auto', exist_ok=True)
    for filename in os.listdir('build'):
        file_path = os.path.join('build', filename)
        try:
            if os.path.isfile(file_path):
                os.remove(file_path)
        except Exception as e:
            print(e)

    # compila C
    subprocess.run(["gcc", "-o", "build/arvore-avl", "oper/arvore-avl.c"])
    subprocess.run(["gcc", "-o", "build/arvore-b", "oper/arvore-b.c"])
    subprocess.run(["gcc", "-o", "build/arvore-rubro-negra", "oper/arvore-rubro-negra.c"])

    with open(ESFORCO_INSERCAO_CSV, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(['Quantidade', 'Esforço AVL', 'Esforço Rubro-Negra', 'Esforço Árvore B(1)', 'Esforço Árvore B(5)', 'Esforço Árvore B(10)'])

        # insercoes
        for i in range(10):
            quantidade = (i + 1) * 1000
            criar_dataset(quantidade)
            roda_operacao(Operacao.INSERCAO, quantidade, writer)

    with open(ESFORCO_DELECAO_CSV, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(['Quantidade', 'Esforço AVL', 'Esforço Rubro-Negra', 'Esforço Árvore B(1)', 'Esforço Árvore B(5)', 'Esforço Árvore B(10)'])

        # delecoes
        for i in range(10):
            quantidade = (i + 1) * 1000
            criar_dataset(quantidade)
            roda_operacao(Operacao.DELECAO, quantidade, writer)


def roda_operacao(operacao, quantidade, writer):
    esforco_avl         = []
    esforco_rubro_negra = []
    esforco_b_1         = []
    esforco_b_5         = []
    esforco_b_10        = []

    for _ in range(SUAVIZACAO):
        esforco_avl.append(roda_arvore_avl(operacao))
        esforco_rubro_negra.append(roda_arvore_rubro_negra(operacao))
        esforco_b_1.append(roda_arvore_b(1, operacao))
        esforco_b_5.append(roda_arvore_b(5, operacao))
        esforco_b_10.append(roda_arvore_b(10, operacao))

    soma_esforco_avl         = sum(esforco_avl) / SUAVIZACAO
    soma_esforco_rubro_negra = sum(esforco_rubro_negra) / SUAVIZACAO
    soma_esforco_b_1         = sum(esforco_b_1) / SUAVIZACAO
    soma_esforco_b_5         = sum(esforco_b_5) / SUAVIZACAO
    soma_esforco_b_10        = sum(esforco_b_10) / SUAVIZACAO

    writer.writerow([quantidade, soma_esforco_avl, soma_esforco_rubro_negra, soma_esforco_b_1, soma_esforco_b_5, soma_esforco_b_10])

def criar_dataset(quantidade):
    with open('build/valores.txt', 'w') as file:
        for i in range(quantidade):
            file.write(f'{random.randint(1, 10000)}\n')

def roda_arvore_avl(rotina):
    output = subprocess.run(["./build/arvore-avl", str(rotina)], capture_output=True, text=True)
    return int(output.stdout.strip())

def roda_arvore_rubro_negra(rotina):
    output = subprocess.run(["./build/arvore-rubro-negra", str(rotina)], capture_output=True, text=True)
    return int(output.stdout.strip())

def roda_arvore_b(ordem, rotina):
    output = subprocess.run(["./build/arvore-b", str(ordem), str(rotina)], capture_output=True, text=True)
    return int(output.stdout.strip())

if __name__ == '__main__':
    main()
    plot_grafico_esforco(ESFORCO_INSERCAO_CSV, 'auto/insercao_esforco.jpg', 'Esforço de inserção')
    plot_grafico_esforco(ESFORCO_DELECAO_CSV, 'auto/delecao_esforco.jpg', 'Esforço de deleção')