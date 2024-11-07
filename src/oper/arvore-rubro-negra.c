#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

// contador
long int contadorRN = 0;

enum coloracao { Vermelho, Preto };
typedef enum coloracao Cor;

typedef struct noRN {
    struct noRN* pai;
    struct noRN* esquerda;
    struct noRN* direita;
    Cor cor;
    int valor;
} NoRN;

typedef struct arvoreRN {
    struct noRN* raiz;
    struct noRN* nulo; 
} ArvoreRN;

// assinaturas de funcoes
ArvoreRN* criarArvoreRN();
int vaziaRN(ArvoreRN*);
NoRN* criarNoRN(ArvoreRN*, NoRN*, int);
NoRN* adicionarNoRN(ArvoreRN* arvoreRN, NoRN* noRN, int valor);
NoRN* inserir(ArvoreRN*, int);
void remover(ArvoreRN* arvoreRN, int valor);
NoRN* localizarRN(ArvoreRN*, int);
void balanceamentoRN(ArvoreRN*, NoRN*);
void reRN(ArvoreRN*, NoRN*);
void rdRN(ArvoreRN*, NoRN*);
void rodaInsercaoRN(FILE *file);
void rodaDelecaoRN(FILE *file);

ArvoreRN* criarArvoreRN() {
    ArvoreRN *arvoreRN = malloc(sizeof(ArvoreRN));
    arvoreRN->nulo     = NULL;
    arvoreRN->raiz     = NULL;

    arvoreRN->nulo      = criarNoRN(arvoreRN, NULL, 0);
    arvoreRN->nulo->cor = Preto;

    return arvoreRN;
}

int vaziaRN(ArvoreRN* arvoreRN) {
    return arvoreRN->raiz == NULL;
}

NoRN* criarNoRN(ArvoreRN* arvoreRN, NoRN* pai, int valor) {
    NoRN* noRN = malloc(sizeof(NoRN));

    noRN->pai      = pai;    
    noRN->valor    = valor;
    noRN->direita  = arvoreRN->nulo;
    noRN->esquerda = arvoreRN->nulo;

    return noRN;
}

NoRN* adicionarNoRN(ArvoreRN* arvoreRN, NoRN* noRN, int valor) {
    contadorRN++;

    if (valor > noRN->valor) {
        contadorRN++;

        if (noRN->direita == arvoreRN->nulo) {
            noRN->direita = criarNoRN(arvoreRN, noRN, valor);     
            noRN->direita->cor = Vermelho;       
        		
            return noRN->direita;

        } else {
            return adicionarNoRN(arvoreRN, noRN->direita, valor);
        }
    } else {
        contadorRN++;

        if (noRN->esquerda == arvoreRN->nulo) {
            noRN->esquerda = criarNoRN(arvoreRN, noRN, valor);
            noRN->esquerda->cor = Vermelho;
            
            return noRN->esquerda;

        } else {
            return adicionarNoRN(arvoreRN, noRN->esquerda, valor);
        }
    }
}

NoRN* inserir(ArvoreRN* arvoreRN, int valor) {
    contadorRN++;

    if (vaziaRN(arvoreRN)) {
        arvoreRN->raiz      = criarNoRN(arvoreRN, arvoreRN->nulo, valor);
        arvoreRN->raiz->cor = Preto;
        	
        return arvoreRN->raiz;

    } else {
        NoRN* noRN = adicionarNoRN(arvoreRN, arvoreRN->raiz, valor);
        balanceamentoRN(arvoreRN, noRN);
        
        return noRN;
    }
}

void remover(ArvoreRN* arvoreRN, int valor) {
    NoRN* no = localizarRN(arvoreRN, valor);

    contadorRN++;
    if (no != NULL) {
        while (1) {
            contadorRN++;

            if (no->esquerda == arvoreRN->nulo && no->direita == arvoreRN->nulo) {
                contadorRN++;

                if (no->pai == arvoreRN->nulo) {
                    arvoreRN->raiz = arvoreRN->nulo;
                } else if (no == no->pai->esquerda) {
                    no->pai->esquerda = arvoreRN->nulo;
                } else {
                    no->pai->direita = arvoreRN->nulo;
                }

                free(no);
                break;

            } else if (no->esquerda != arvoreRN->nulo && no->direita != arvoreRN->nulo) {
                NoRN* sucessor = no->direita;

                contadorRN++;

                while (sucessor->esquerda != arvoreRN->nulo) {
                    contadorRN++;
                    sucessor = sucessor->esquerda;
                }

                no->valor = sucessor->valor;
                no = sucessor;

            } else {
                contadorRN++;

                NoRN* filho = (no->esquerda != arvoreRN->nulo) ? no->esquerda : no->direita;
                filho->pai  = no->pai;

                contadorRN++;
                if (no->pai == arvoreRN->nulo) {
                    arvoreRN->raiz = filho;
                } else if (no == no->pai->esquerda) {
                    no->pai->esquerda = filho;
                } else {
                    no->pai->direita = filho;
                }

                free(no);
                break;
            }
        }

        balanceamentoRN(arvoreRN, arvoreRN->raiz);
    }
}

NoRN* localizarRN(ArvoreRN* arvoreRN, int valor) {
    contadorRN++;

    if (!vaziaRN(arvoreRN)) {
        NoRN* noRN = arvoreRN->raiz;

        contadorRN++;
        while (noRN != arvoreRN->nulo) {
            contadorRN++;
            contadorRN++;

            if (noRN->valor == valor) {
                return noRN;
            } else {
                noRN = valor < noRN->valor ? noRN->esquerda : noRN->direita;
            }
        }
    }

    return NULL;
}

void balanceamentoRN(ArvoreRN* arvoreRN, NoRN* noRN) {
    contadorRN++;

    while (noRN->pai != NULL && noRN->pai->cor == Vermelho) {
        contadorRN++;
        contadorRN++;

        if (noRN->pai == noRN->pai->pai->esquerda) {
            NoRN *tio = noRN->pai->pai->direita;
            
            contadorRN++;
            if (tio->cor == Vermelho) {
                tio->cor       = Preto;
                noRN->pai->cor = Preto; 

                noRN->pai->pai->cor = Vermelho;
                noRN = noRN->pai->pai;

            } else {
                contadorRN++;

                if (noRN == noRN->pai->direita) {
                    noRN = noRN->pai;

                    reRN(arvoreRN, noRN);

                } else {
                    noRN->pai->cor      = Preto; 
                    noRN->pai->pai->cor = Vermelho;

                    rdRN(arvoreRN, noRN->pai->pai);
                }
            }
        } else {
            NoRN *tio = noRN->pai->pai->esquerda;
            
            contadorRN++;
            if (tio->cor == Vermelho) {
                tio->cor = Preto;
                noRN->pai->cor = Preto; 

                noRN->pai->pai->cor = Vermelho;
                noRN = noRN->pai->pai;

            } else {
                contadorRN++;

                if (noRN == noRN->pai->esquerda) {
                    noRN = noRN->pai;
                    rdRN(arvoreRN, noRN);

                } else {
                    noRN->pai->cor = Preto; 
                    noRN->pai->pai->cor = Vermelho;
                    reRN(arvoreRN, noRN->pai->pai);
                }
            }
        }
    }

    arvoreRN->raiz->cor = Preto;
}

void reRN(ArvoreRN* arvoreRN, NoRN* noRN) {
    NoRN* direita = noRN->direita;
    noRN->direita = direita->esquerda; 

    contadorRN++;
    if (direita->esquerda != arvoreRN->nulo) {
        direita->esquerda->pai = noRN;
    }

    direita->pai = noRN->pai;
    
    contadorRN++;
    if (noRN->pai == arvoreRN->nulo) {
        arvoreRN->raiz = direita;
    } else if (noRN == noRN->pai->esquerda) {
        noRN->pai->esquerda = direita;
    } else {
        noRN->pai->direita = direita;
    }

    direita->esquerda = noRN;
    noRN->pai         = direita;
}

void rdRN(ArvoreRN* arvoreRN, NoRN* noRN) {
    NoRN* esquerda = noRN->esquerda;
    noRN->esquerda = esquerda->direita;
    
    contadorRN++;
    if (esquerda->direita != arvoreRN->nulo) {
        esquerda->direita->pai = noRN;
    }
    
    esquerda->pai = noRN->pai;
    
    contadorRN++;
    if (noRN->pai == arvoreRN->nulo) {
        arvoreRN->raiz = esquerda;
    } else if (noRN == noRN->pai->esquerda) {
        noRN->pai->esquerda = esquerda;
    } else {
        noRN->pai->direita = esquerda;
    }
    
    esquerda->direita = noRN;
    noRN->pai         = esquerda;
}

void rodaInsercaoRN(FILE *file) {
    ArvoreRN* tree = criarArvoreRN();
    int value;

    while (fscanf(file, "%d", &value) != EOF) {
        inserir(tree, value);
    }
}

void rodaDelecaoRN(FILE *file) {
    ArvoreRN* tree = criarArvoreRN();
    int value;

    while (fscanf(file, "%d", &value) != EOF) {
        inserir(tree, value);
    }

    while (fscanf(file, "%d", &value) != EOF) {
        remover(tree, value);
    }
}

int main(int argc, char *argv[]) {
    FILE *file = fopen("build/valores.txt", "r");
    if (file == NULL) {
        return 1;
    }

    int rotina = atoi(argv[1]);

    if (rotina == 1) {
        rodaInsercaoRN(file);
    }

    if (rotina == 2) {
        rodaDelecaoRN(file);
    }

    printf("%ld\n", contadorRN);

    fclose(file);

    return 0;
}