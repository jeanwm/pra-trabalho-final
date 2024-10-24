#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "oper/arvore-binaria-adicao.h"
#include "oper/arvore-rubro-negra-adicao.h"
#include "oper/arvore-b-adicao.h"
#include "oper/arvore-binaria-remocao.h"
#include "oper/arvore-rubro-negra-remocao.h"
#include "oper/arvore-b-remocao.h"


int* novo_vetor(int n) {
    int* v = malloc(sizeof(int) * n);

    if (v == NULL){
        exit(EXIT_FAILURE);
        return NULL;
    }

    static int contador = 0;

    srand(time(NULL)+ contador++);

    for (int i = 0; i < n; i++) {
        v[i] = rand();
    }

    return v;
}

int* copiar_vetor(int v[], int n) {
    int* copia = malloc(sizeof(int) * n);

    if (copia == NULL) {
        exit(EXIT_FAILURE);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        copia[i] = v[i];
    }

    return copia;
}

int main() {
    int limite = 1000;
	int vezes = 30;

    FILE *fptr = fopen("output.csv", "w");

    if (fptr == NULL){
        printf("Falha ao abrir o arquivo");

        return EXIT_FAILURE;
    }

    fprintf(fptr, "Iterações, Árvore binária (adição), Árvore rubro-negra (adição), Árvore B (adição), Árvore binária (remoção), Árvore rubro-negra (remoção), Árvore B (remoção)\n");

    for (int n = 1; n <= limite; n++){
        unsigned int 
        arvoreBinariaAdicao     = 0,
        arvoreRubroNegraAdicao  = 0,
        arvoreBAdicao           = 0,
        arvoreBinariaRemocao    = 0,
        arvoreRubroNegraRemocao = 0,
        arvoreBRemocao          = 0;

        for (int i = 1; i <= vezes; i++){
            int* v = novo_vetor(n);

            int* v1 = copiar_vetor(v, n);
            int* v2 = copiar_vetor(v, n);
            int* v3 = copiar_vetor(v, n);
            int* v4 = copiar_vetor(v, n);
            int* v5 = copiar_vetor(v, n);

            arvoreBinariaAdicao     += bubbleSort(v, n);
            arvoreRubroNegraAdicao  += insertionSort(v1, n);
            arvoreBAdicao 	        += heapSort(v2, n);
            arvoreBinariaRemocao 	+= mergeSort(v3, n);
            arvoreRubroNegraRemocao += quickSort(v4, n);
            arvoreBRemocao 	        += radixSort(v5, n);

            free(v);
            free(v1);
            free(v2);
            free(v3);
            free(v4);
            free(v5);
        }

        unsigned int arvoreBinariaAdicaoMedia     = arvoreBinariaAdicao / vezes;
        unsigned int arvoreRubroNegraAdicaoMedia  = arvoreRubroNegraAdicao / vezes;
        unsigned int arvoreBAdicaoMedia 	      = arvoreBAdicao / vezes;
        unsigned int arvoreBinariaRemocaoMedia 	  = arvoreBinariaRemocao / vezes;
        unsigned int arvoreRubroNegraRemocaoMedia = arvoreRubroNegraRemocao / vezes;
        unsigned int arvoreBRemocaoMedia	      = arvoreBRemocao / vezes;

        fprintf(fptr, "%d, %u, %u, %u, %u, %u, %u\n", n,
            arvoreBinariaAdicaoMedia,
            arvoreRubroNegraAdicaoMedia,
            arvoreBAdicaoMedia,
            arvoreBinariaRemocaoMedia,
            arvoreRubroNegraRemocaoMedia,
            arvoreBRemocaoMedia
        );
    }

    fclose(fptr);
    
    return EXIT_SUCCESS;
}
