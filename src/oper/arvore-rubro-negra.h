#include <stdlib.h>
#include <stdio.h>

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
    unsigned int contador_balanceamentos; // Contador de balanceamentos
} ArvoreRN;

NoRN* criarNoRN(ArvoreRN*, NoRN*, int);
void balancearRN(ArvoreRN*, NoRN*);
void rotacionarEsquerdaRN(ArvoreRN*, NoRN*);
void rotacionarDireitaRN(ArvoreRN*, NoRN*);

ArvoreRN* criarRN() {
    ArvoreRN *arvore = malloc(sizeof(ArvoreRN));
    arvore->nulo = NULL;
    arvore->raiz = NULL;
    arvore->nulo = criarNoRN(arvore, NULL, 0);
    arvore->nulo->cor = Preto;
    arvore->contador_balanceamentos = 0; // Inicializa o contador
    return arvore;
}

NoRN* criarNoRN(ArvoreRN* arvore, NoRN* pai, int valor) {
    NoRN* no = malloc(sizeof(NoRN));
    no->pai = pai;    
    no->valor = valor;
    no->direita = arvore->nulo;
    no->esquerda = arvore->nulo;
    return no;
}

NoRN* adicionarNoRN(ArvoreRN* arvore, NoRN* no, int valor) {
    if (valor > no->valor) {
        if (no->direita == arvore->nulo) {
            no->direita = criarNoRN(arvore, no, valor);     
            no->direita->cor = Vermelho;       
            return no->direita;
        } else {
            return adicionarNoRN(arvore, no->direita, valor);
        }
    } else {
        if (no->esquerda == arvore->nulo) {
            no->esquerda = criarNoRN(arvore, no, valor);
            no->esquerda->cor = Vermelho;
            return no->esquerda;
        } else {
            return adicionarNoRN(arvore, no->esquerda, valor);
        }
    }
}

NoRN* adicionarRN(ArvoreRN* arvore, int valor) {
    if (arvore->raiz == NULL) {
        arvore->raiz = criarNoRN(arvore, arvore->nulo, valor);
        arvore->raiz->cor = Preto;
        return arvore->raiz;
    } else {
        NoRN* no = adicionarNoRN(arvore, arvore->raiz, valor);
        balancearRN(arvore, no);
        return no;
    }
}

void balancearRN(ArvoreRN* arvore, NoRN* no) {
    while (no->pai != arvore->nulo && no->pai->cor == Vermelho) {
        if (no->pai == no->pai->pai->esquerda) {
            NoRN *tio = no->pai->pai->direita;
            if (tio->cor == Vermelho) {
                tio->cor = Preto; // Caso 1
                no->pai->cor = Preto; 
                no->pai->pai->cor = Vermelho; // Caso 1
                no = no->pai->pai; // Caso 1
                arvore->contador_balanceamentos++; // Incrementa o contador
            } else {
                if (no == no->pai->direita) {
                    no = no->pai; // Caso 2
                    rotacionarEsquerdaRN(arvore, no); // Caso 2
                }
                no->pai->cor = Preto; 
                no->pai->pai->cor = Vermelho; // Caso 3
                rotacionarDireitaRN(arvore, no->pai->pai); // Caso 3
                arvore->contador_balanceamentos++; // Incrementa o contador
            }
        } else {
            NoRN *tio = no->pai->pai->esquerda;
            if (tio->cor == Vermelho) {
                tio->cor = Preto; // Caso 1
                no->pai->cor = Preto; 
                no->pai->pai->cor = Vermelho; // Caso 1
                no = no->pai->pai; // Caso 1
                arvore->contador_balanceamentos++; // Incrementa o contador
            } else {
                if (no == no->pai->esquerda) {
                    no = no->pai; // Caso 2
                    rotacionarDireitaRN(arvore, no); // Caso 2
                }
                no->pai->cor = Preto; 
                no->pai->pai->cor = Vermelho; // Caso 3
                rotacionarEsquerdaRN(arvore, no->pai->pai); // Caso 3
                arvore->contador_balanceamentos++; // Incrementa o contador
            }
        }
    }
    arvore->raiz->cor = Preto; // Conserta possível quebra regra 2
}

void rotacionarEsquerdaRN(ArvoreRN* arvore, NoRN* no) {
    NoRN* direita = no->direita;
    no->direita = direita->esquerda; 

    if (direita->esquerda != arvore->nulo) {
        direita->esquerda->pai = no;
    }

    direita->pai = no->pai;
    
    if (no->pai == arvore->nulo) {
        arvore->raiz = direita;
    } else if (no == no->pai->esquerda) {
        no->pai->esquerda = direita;
    } else {
        no->pai->direita = direita;
    }

    direita->esquerda = no;
    no->pai = direita;
    arvore->contador_balanceamentos++; // Incrementa o contador
}

void rotacionarDireitaRN(ArvoreRN* arvore, NoRN* no) {
    NoRN* esquerda = no->esquerda;
    no->esquerda = esquerda->direita;
    
    if (esquerda->direita != arvore->nulo) {
        esquerda->direita->pai = no;
    }
    
    esquerda->pai = no->pai;
    
    if (no->pai == arvore->nulo) {
        arvore->raiz = esquerda;
    } else if (no == no->pai->esquerda) {
        no->pai->esquerda = esquerda;
    } else {
        no->pai->direita = esquerda;
    }
    
    esquerda->direita = no;
    no->pai = esquerda;
    arvore->contador_balanceamentos++; // Incrementa o contador
}

NoRN* removerNoRN(ArvoreRN* arvore, NoRN* no, int valor) {
    if (valor < no->valor) {
        return removerNoRN(arvore, no->esquerda, valor);
    } else if (valor > no->valor) {
        return removerNoRN(arvore, no->direita, valor);
    } else {
        if (no->esquerda == arvore->nulo) {
            NoRN* temp = no->direita;
            free(no);
            return temp;
        } else if (no->direita == arvore->nulo) {
            NoRN* temp = no->esquerda;
            free(no);
            return temp;
        } else {
            NoRN* temp = no->esquerda;
            while (temp->direita != arvore->nulo) {
                temp = temp->direita;
            }
            no->valor = temp->valor;
            no->esquerda = removerNoRN(arvore, no->esquerda, temp->valor);
            return no;
        }
    }
}

void removerRN(ArvoreRN* arvore, int valor) {
    arvore->raiz = removerNoRN(arvore, arvore->raiz, valor);
    balancearRN(arvore, arvore->raiz);
}

int contador_adicao_rubro_negra(int* v, int n) {
    ArvoreRN* arvore = criarRN();
    unsigned int contador = 0;

    for (int i = 0; i < n; i++) {
        adicionarRN(arvore, v[i]);
        contador += arvore->contador_balanceamentos;
        arvore->contador_balanceamentos = 0; // Reseta o contador
    }

    free(arvore);
    return contador;
}

int contador_remocao_rubro_negra(int* v, int n) {
    ArvoreRN* arvore = criarRN();
    unsigned int contador = 0;

    for (int i = 0; i < n; i++) {
        adicionarRN(arvore, v[i]);
    }

    for (int i = 0; i < n; i++) {
        removerRN(arvore, v[i]);
        contador += arvore->contador_balanceamentos;
        arvore->contador_balanceamentos = 0; // Reseta o contador
    }

    free(arvore);
    return contador;
}