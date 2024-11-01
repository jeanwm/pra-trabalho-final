#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "oper/arvore-avl.h"         // arquivo de cabeçalho da árvore AVL
#include "oper/arvore-rubro-negra.h" // arquivo de cabeçalho da árvore rubro-negra
#include "oper/arvore-b.h"           // arquivo de cabeçalho da árvore B

int* novo_vetor(int n) {
    int* v = malloc(sizeof(int) * n);
    if (v == NULL) exit(EXIT_FAILURE);

    // para geração de números aleatórios
    srand(time(NULL)); 

    for (int i = 0; i < n; i++) {
        v[i] = rand() % 10000;
    }

    return v;
}

void contador_adicao_remocao(int n, FILE *fptr, int vezes) {
    unsigned int
        arvoreAVLAdicao         = 0,
        arvoreRubroNegraAdicao  = 0,
        arvoreBAdicao           = 0,
        arvoreAVLRemocao        = 0,
        arvoreRubroNegraRemocao = 0,
        arvoreBRemocao          = 0;

    for (int i = 0; i < vezes; i++) {
        int* v = novo_vetor(n);

        // contador para adição
        arvoreAVLAdicao        += contador_adicao_avl(v, n);
        arvoreRubroNegraAdicao += contador_adicao_rubro_negra(v, n);
        arvoreBAdicao          += contador_adicao_b(v, n);

        // contador para remoção
        arvoreAVLRemocao        += contador_remocao_avl(v, n);
        arvoreRubroNegraRemocao += contador_remocao_rubro_negra(v, n);
        arvoreBRemocao          += contador_remocao_b(v, n);

        free(v);
    }

    fprintf(fptr, "%d, %u, %u, %u, %u, %u, %u\n",
        n,
        arvoreAVLAdicao / vezes,
        arvoreRubroNegraAdicao / vezes,
        arvoreBAdicao / vezes,
        arvoreAVLRemocao / vezes,
        arvoreRubroNegraRemocao / vezes,
        arvoreBRemocao / vezes);
}

int main() {
    int limite = 10000;
    int vezes = 10;

    FILE *fptr = fopen("output.csv", "w");

    if (fptr == NULL) {
        printf("Falha ao abrir o arquivo");
        return EXIT_FAILURE;
    }

    fprintf(fptr, "Iterações, AVL Adição, Rubro-Negra Adição, B Adição, AVL Remoção, Rubro-Negra Remoção, B Remoção\n");

    for (int n = 1; n <= limite; n++) {
        contador_adicao_remocao(n, fptr, vezes);
    }

    fclose(fptr);
    return EXIT_SUCCESS;
}
