import csv
import matplotlib.pyplot as plt

def plot_grafico_esforco(csv_file, jpg_file, title):
    plt.clf()
    quantidade          = []
    esforco_avl         = []
    esforco_rubro_negra = []
    esforco_b_1         = []
    esforco_b_5         = []
    esforco_b_10        = []

    with open(csv_file, 'r') as csvfile:
        reader = csv.reader(csvfile)
        next(reader)

        for row in reader:
            quantidade.append(int(row[0]))
            esforco_avl.append(float(row[1]))
            esforco_rubro_negra.append(float(row[2]))
            esforco_b_1.append(float(row[3]))
            esforco_b_5.append(float(row[4]))
            esforco_b_10.append(float(row[5]))

    plt.plot(quantidade, esforco_avl, label='Árvore AVL')
    plt.plot(quantidade, esforco_rubro_negra, label='Árvore Rubro-Negra')
    plt.plot(quantidade, esforco_b_1, label='Árvore B (ordem 1)')
    plt.plot(quantidade, esforco_b_5, label='Árvore B (ordem 5)')
    plt.plot(quantidade, esforco_b_10, label='Árvore B (ordem 10)')

    plt.xlabel('Tamanho do conjunto de dados')
    plt.yscale('log')
    plt.ylabel('Esforço')
    plt.title(title)
    plt.legend()
    plt.savefig(jpg_file)