#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// contador
long int contadorAVL = 0;

typedef struct noAVL {
    struct noAVL* pai;
    struct noAVL* esquerda;
    struct noAVL* direita;
    int valor;
    int altura;
} NoAVL;

typedef struct arvoreAVL {
    struct noAVL* raiz;
} ArvoreAVL;

// assinaturas de funcoes
int max(int a, int b);
int alturaAVL(NoAVL*);
ArvoreAVL* criarAVL();
void inserir(ArvoreAVL* arvore, int valor);
void remover(ArvoreAVL* arvore, int valor);
NoAVL* localizarAVL(NoAVL* no, int valor);
void balanceamentoAVL(ArvoreAVL*, NoAVL*);
int fator_balanceamentoAVL(NoAVL*);
NoAVL* rse(ArvoreAVL*, NoAVL*);
NoAVL* rsd(ArvoreAVL*, NoAVL*);
NoAVL* rde(ArvoreAVL*, NoAVL*);
NoAVL* rdd(ArvoreAVL*, NoAVL*);
void rodaInsercaoAVL(FILE *file);
void rodaDelecaoAVL(FILE *file);

int max(int a, int b) {
    contadorAVL++;

    return a > b ? a : b;
}

int alturaAVL(NoAVL* no) {
    contadorAVL++;

    return no != NULL ? no->altura : 0;
}

ArvoreAVL* criarAVL() {
    ArvoreAVL *arvore = malloc(sizeof(ArvoreAVL));
    arvore->raiz = NULL;
  
    return arvore;
}

void inserir(ArvoreAVL* arvore, int valor) {
    NoAVL* no = arvore->raiz;

    while (no != NULL) {
        contadorAVL++;

        if (valor > no->valor) {
            contadorAVL++;

            if (no->direita != NULL) {
                no = no->direita;
            } else {
                break;
            }

        } else {
            contadorAVL++;

            if (no->esquerda != NULL) {
                no = no->esquerda;
            } else {
                break;
            }
        }
    }

    NoAVL* novo    = malloc(sizeof(NoAVL));
    novo->valor    = valor;
    novo->pai      = no;
    novo->esquerda = NULL;
    novo->direita  = NULL;
    novo->altura   = 1;

    contadorAVL++;
    if (no == NULL) {    
        arvore->raiz = novo;
    } else {
        contadorAVL++;

        if (valor > no->valor) {
            no->direita = novo;
        } else {
            no->esquerda = novo;
        }
        
        balanceamentoAVL(arvore, no);
    }
}

void remover(ArvoreAVL* arvore, int valor) {
    NoAVL* no = localizarAVL(arvore->raiz, valor);

    contadorAVL++;
    while (no != NULL) {
        contadorAVL++;
        contadorAVL++;

        if (no->esquerda == NULL && no->direita == NULL) {
            contadorAVL++;

            if (no->pai == NULL) {
                arvore->raiz = NULL;
            } else {
                contadorAVL++;

                if (no->pai->esquerda == no) {
                    no->pai->esquerda = NULL;
                } else {
                    no->pai->direita = NULL;
                }
            }

            free(no);
            break;

        } else if (no->esquerda != NULL && no->direita != NULL) {
            NoAVL* sucessor = no->direita;
            contadorAVL++;

            while (sucessor->esquerda != NULL) {
                contadorAVL++;
                sucessor = sucessor->esquerda;
            }

            no->valor = sucessor->valor;
            no        = sucessor;

        } else {
            NoAVL* filho  = (no->esquerda != NULL) ? no->esquerda : no->direita;
            filho->pai = no->pai;

            contadorAVL++;
            if (no->pai == NULL) {
                arvore->raiz = filho;
                
            } else {
                if (no->pai->esquerda == no) {
                    no->pai->esquerda = filho;
                } else {
                    no->pai->direita = filho;
                }
            }

            free(no);
            break;
        }
    }

    if (no != NULL) {
        balanceamentoAVL(arvore, arvore->raiz);
    }
}

NoAVL* localizarAVL(NoAVL* no, int valor) {
    while (no != NULL) {
        contadorAVL++;
        contadorAVL++;

        if (no->valor == valor) {
            return no;
        }
        
        contadorAVL++;

        no = valor < no->valor ? no->esquerda : no->direita;
    }

    return NULL;
}

void balanceamentoAVL(ArvoreAVL* arvore, NoAVL* no) {
    contadorAVL++;

    while (no != NULL) {
        contadorAVL++;

        no->altura = max(alturaAVL(no->esquerda), alturaAVL(no->direita)) + 1;
        int fator  = fator_balanceamentoAVL(no);

        contadorAVL++;
        if (fator > 1) {
            contadorAVL++;

            if (fator_balanceamentoAVL(no->esquerda) > 0) {
                rsd(arvore, no); 
            } else {
                rdd(arvore, no);
            }

        } else if (fator < -1) {
            contadorAVL++;

            if (fator_balanceamentoAVL(no->direita) < 0) {
                rse(arvore, no);
            } else {
                rde(arvore, no);
            }
        }

        no = no->pai; 
    }
}

int fator_balanceamentoAVL(NoAVL* no) {
    int esquerda = 0, direita = 0;
  
    contadorAVL++;
    if (no->esquerda != NULL) {
        esquerda = no->esquerda->altura;
    }

    contadorAVL++;
    if (no->direita != NULL) {
        direita = no->direita->altura;
    }
  
    return esquerda - direita;
}

NoAVL* rse(ArvoreAVL* arvore, NoAVL* no) {
    NoAVL* pai     = no->pai;
    NoAVL* direita = no->direita;

    contadorAVL++;
    if (direita->esquerda != NULL) {
        direita->esquerda->pai = no;
    } 
  
    no->direita = direita->esquerda;
    no->pai     = direita;

    direita->esquerda = no;
    direita->pai      = pai;

    contadorAVL++;
    if (pai == NULL) {
        arvore->raiz = direita;
    } else {
        contadorAVL++;

        if (pai->esquerda == no) {
            pai->esquerda = direita;
        } else {
            pai->direita = direita;
        }
    }

    no->altura      = max(alturaAVL(no->esquerda), alturaAVL(no->direita)) + 1;
    direita->altura = max(alturaAVL(direita->esquerda), alturaAVL(direita->direita)) + 1;

    return direita;
}

NoAVL* rsd(ArvoreAVL* arvore, NoAVL* no) {
    NoAVL* pai      = no->pai;
    NoAVL* esquerda = no->esquerda;

    contadorAVL++;
    if (esquerda->direita != NULL) {
        esquerda->direita->pai = no;
    } 
  
    no->esquerda = esquerda->direita;
    no->pai      = esquerda;
  
    esquerda->direita = no;
    esquerda->pai     = pai;

    contadorAVL++;
    if (pai == NULL) {
        arvore->raiz = esquerda;
    } else {
        contadorAVL++;

        if (pai->esquerda == no) {
            pai->esquerda = esquerda;
        } else {
            pai->direita = esquerda;
        }
    }

    no->altura       = max(alturaAVL(no->esquerda), alturaAVL(no->direita)) + 1;
    esquerda->altura = max(alturaAVL(esquerda->esquerda), alturaAVL(esquerda->direita)) + 1;

    return esquerda;
}

NoAVL* rde(ArvoreAVL* arvore, NoAVL* no) {
    no->direita = rsd(arvore, no->direita);

    return rse(arvore, no);
}

NoAVL* rdd(ArvoreAVL* arvore, NoAVL* no) {
    no->esquerda = rse(arvore, no->esquerda);

    return rsd(arvore, no);
}

void rodaInsercaoAVL(FILE *file) {
    ArvoreAVL *arvore = criarAVL();
    int value = 0;

    while (fscanf(file, "%d", &value) != EOF) {
       inserir(arvore, value);
    }
}

void rodaDelecaoAVL(FILE *file) {
    ArvoreAVL *arvore = criarAVL();
    int value;

    while (fscanf(file, "%d", &value) != EOF) {
        inserir(arvore, value);
    }

    while (fscanf(file, "%d", &value) != EOF) {
        remover(arvore, value);
    }
}

int main(int argc, char *argv[]) {
    FILE *file = fopen("build/valores.txt", "r");
    if (file == NULL) {
        printf("Uma falha ocorreu ao tentar abrir o arquivo.\n");
        return 1;
    }

    int rotina = atoi(argv[1]);

    if (rotina == 1) {
        rodaInsercaoAVL(file);
    }

    if (rotina == 2) {
        rodaDelecaoAVL(file);
    }

    printf("%ld\n", contadorAVL);

    fclose(file);

    return 0;
}