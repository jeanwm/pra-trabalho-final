#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

// revisar contadores para ordem 5 e 10 (10 deve ser mais eficiente)

// contador
long int contadorB = 0;

typedef struct noB {
    struct noB** filhos;
    struct noB* pai;
    int* chaves;
    int total;
} NoB;

typedef struct arvoreB {
    NoB* raiz;
    int ordem;
} ArvoreB;

// assinaturas de funcoes
ArvoreB* criarArvoreB(int);
NoB* criaNoB(ArvoreB*);
int pesquisaBinariaB(NoB*, int);
NoB* localizaNoB(ArvoreB*, int);
void adicionaChaveNoB(NoB*, NoB*, int);
int transbordoB(ArvoreB*, NoB*);
NoB* divideNoB(ArvoreB*, NoB*);
void adicionaChaveRecursivoB(ArvoreB*, NoB*, NoB*, int);
void inserir(ArvoreB*, int);
void remover(ArvoreB*, int);
void removerChaveRecursivoB(ArvoreB*, NoB*, int);
void removerChaveNoB(NoB*, int);
void rodaInsercaoB(FILE *file, int ordem);
void rodaDelecaoB(FILE *file, int ordem);

ArvoreB* criarArvoreB(int ordem) {
    ArvoreB* a = malloc(sizeof(ArvoreB));

    a->ordem = ordem;
    a->raiz  = criaNoB(a);

    return a;
}

NoB* criaNoB(ArvoreB* arvore) {
    int max  = arvore->ordem * 2;
    NoB* noB = malloc(sizeof(NoB));

    noB->pai = NULL;

    noB->chaves = malloc(sizeof(int) * (max + 1));
    noB->filhos = malloc(sizeof(NoB) * (max + 2));
    noB->total  = 0;

    for (int i = 0; i < max + 2; i++) {
        contadorB++;

        noB->filhos[i] = NULL;
    }

    return noB;
}

int pesquisaBinariaB(NoB* noB, int chave) {
    int inicio = 0, fim = noB->total - 1, meio;

    while (inicio <= fim) {
        contadorB++;

        meio = (inicio + fim) / 2;

        contadorB++;
        if (noB->chaves[meio] == chave) {
            return meio;	
        }
        else if (noB->chaves[meio] > chave) {
            fim = meio - 1;
        } else {
            inicio = meio + 1;
        }
    }

    return inicio;
}

NoB* localizaNoB(ArvoreB* arvore, int chave) {
    NoB* noB = arvore->raiz;

    contadorB++;
    while (noB != NULL) {
        contadorB++;

        int i = pesquisaBinariaB(noB, chave);

        contadorB++;
        if (noB->filhos[i] == NULL) {
            return noB;
        } else {
            noB = noB->filhos[i];
        }
    }

    return NULL;
}

void adicionaChaveNoB(NoB* noB, NoB* novo, int chave) {
    int i = pesquisaBinariaB(noB, chave);

    contadorB++;
    for (int j = noB->total - 1; j >= i; j--) {
        contadorB++;

        noB->chaves[j + 1] = noB->chaves[j];
        noB->filhos[j + 2] = noB->filhos[j + 1];
    }

    noB->chaves[i]     = chave;
    noB->filhos[i + 1] = novo;

    noB->total++;
}

int transbordoB(ArvoreB* arvore, NoB* noB) {
    contadorB++;

    return noB->total > arvore->ordem * 2;
}

NoB* divideNoB(ArvoreB* arvore, NoB* noB) {
    int meio  = noB->total / 2;
    NoB* novo = criaNoB(arvore);
    novo->pai = noB->pai;

    contadorB++;
    for (int i = meio + 1; i < noB->total; i++) {
        contadorB++;

        novo->filhos[novo->total] = noB->filhos[i];
        novo->chaves[novo->total] = noB->chaves[i];

        contadorB++;
        if (novo->filhos[novo->total] != NULL) { 
            novo->filhos[novo->total]->pai = novo;
        }
        
        novo->total++;
    }

    novo->filhos[novo->total] = noB->filhos[noB->total];
    contadorB++;

    if (novo->filhos[novo->total] != NULL) {
        novo->filhos[novo->total]->pai = novo;
    }

    noB->total = meio;

    return novo;
}

void adicionaChaveRecursivoB(ArvoreB* arvore, NoB* noB, NoB* novo, int chave) {
    contadorB++;

    adicionaChaveNoB(noB, novo, chave);

    contadorB++;
    if (transbordoB(arvore, noB)) {
        int promovido = noB->chaves[arvore->ordem];
        NoB* novo     = divideNoB(arvore, noB);

        contadorB++;
        if (noB->pai == NULL) {
            NoB* pai       = criaNoB(arvore);
            pai->filhos[0] = noB;

            adicionaChaveNoB(pai, novo, promovido);

            noB->pai     = pai;
            novo->pai    = pai;
            arvore->raiz = pai;

        } else {
            adicionaChaveRecursivoB(arvore, noB->pai, novo, promovido);
        }
    }
}

void inserir(ArvoreB* arvore, int chave) {
    NoB* noB = localizaNoB(arvore, chave);

    adicionaChaveRecursivoB(arvore, noB, NULL, chave);
}

void remover(ArvoreB* arvore, int chave) {
    contadorB++;
    removerChaveRecursivoB(arvore, arvore->raiz, chave);
}

void removerChaveRecursivoB(ArvoreB* arvore, NoB* noB, int chave) {
    contadorB++;

    if (noB == NULL) {
        return;
    }

    int indice = pesquisaBinariaB(noB, chave);

    contadorB++;
    if (indice < noB->total && noB->chaves[indice] == chave) {
        contadorB++;

        if (noB->filhos[indice] != NULL) {
            NoB* noSubstituto = noB->filhos[indice + 1];

            contadorB++;
            while (noSubstituto->filhos[0] != NULL) {
                contadorB++;
                noSubstituto = noSubstituto->filhos[0];
            }

            noB->chaves[indice] = noSubstituto->chaves[0];

            removerChaveRecursivoB(arvore, noSubstituto, noSubstituto->chaves[0]);

        } else {
            removerChaveNoB(noB, indice);
        }
    } else {
        removerChaveRecursivoB(arvore, noB->filhos[indice], chave);
    }
}

void removerChaveNoB(NoB* noB, int indice) {
    contadorB++;
    for (int i = indice; i < noB->total - 1; i++) {
        contadorB++;

        noB->chaves[i] = noB->chaves[i + 1];
        noB->filhos[i + 1] = noB->filhos[i + 2];
    }

    noB->total--;
}

void rodaInsercaoB(FILE *file, int ordem) {
    ArvoreB* tree = criarArvoreB(ordem);
    int value;
    while (fscanf(file, "%d", &value) != EOF) {
        inserir(tree, value);
    }
}

void rodaDelecaoB(FILE *file, int ordem) {
    ArvoreB* tree = criarArvoreB(ordem);
    int value;

    while (fscanf(file, "%d", &value) != EOF) {
        inserir(tree, value);
    }

    while (fscanf(file, "%d", &value) != EOF) {
        remover(tree, value);
    }
}

int main(int argc, char *argv[]) {
    int ordem  = atoi(argv[1]);
    int rotina = atoi(argv[2]);

    FILE* file = fopen("build/valores.txt", "r");
    if (file == NULL) {
        return 1;
    }

    if (rotina == 1) {
        rodaInsercaoB(file, ordem);
    }

    if (rotina == 2) {
        rodaDelecaoB(file, ordem);
    }

    printf("%ld\n", contadorB);

    fclose(file);
    
    return 0;
}
