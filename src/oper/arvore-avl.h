#include <stdio.h>
#include <stdlib.h>

typedef struct noAVL {
    struct noAVL* pai;      // ponteiro para o nó pai
    struct noAVL* esquerda; // ponteiro para o nó filho a esquerda
    struct noAVL* direita;  // ponteiro para o nó filho a direita
    int valor;           // conteúdo de um nó arbitrário da árvore
    int altura;          // altura do nó
} NoAVL;

typedef struct arvoreAVL {
    struct noAVL* raiz;
} ArvoreAVL;

// Funções auxiliares
int alturaAVL(NoAVL* n) {
    return n ? n->altura : 0;
}

int maxAVL(int a, int b) {
    return (a > b) ? a : b;
}

// Função para criar uma nova árvore
ArvoreAVL* criarAVL() {
    ArvoreAVL *a = (ArvoreAVL*) malloc(sizeof(ArvoreAVL));
    a->raiz = NULL;
    return a;
}

// Função para criar um novo nó
NoAVL* novo_noAVL(int valor) {
    NoAVL* no = (NoAVL*) malloc(sizeof(NoAVL));
    no->valor = valor;
    no->esquerda = no->direita = NULL;
    no->pai = NULL;
    no->altura = 1; // novo nó é inicialmente adicionado na folha
    return no;
}

// Rotação à direita
NoAVL* rotacionar_direitaAVL(NoAVL* y) {
    NoAVL* x = y->esquerda;
    NoAVL* T2 = x->direita;

    // Realiza a rotação
    x->direita = y;
    y->esquerda = T2;

    // Atualiza pais
    if (T2) T2->pai = y;
    x->pai = y->pai;
    y->pai = x;

    // Atualiza alturas
    y->altura = maxAVL(alturaAVL(y->esquerda), alturaAVL(y->direita)) + 1;
    x->altura = maxAVL(alturaAVL(x->esquerda), alturaAVL(x->direita)) + 1;

    return x; // nova raiz
}

// Rotação à esquerda
NoAVL* rotacionar_esquerdaAVL(NoAVL* x) {
    NoAVL* y = x->direita;
    NoAVL* T2 = y->esquerda;

    // Realiza a rotação
    y->esquerda = x;
    x->direita = T2;

    // Atualiza pais
    if (T2) T2->pai = x;
    y->pai = x->pai;
    x->pai = y;

    // Atualiza alturas
    x->altura = maxAVL(alturaAVL(x->esquerda), alturaAVL(x->direita)) + 1;
    y->altura = maxAVL(alturaAVL(y->esquerda), alturaAVL(y->direita)) + 1;

    return y; // nova raiz
}

// Obtém o fator de balanceamento
int fator_balanceamentoAVL(NoAVL* n) {
    if (!n) return 0;
    return alturaAVL(n->esquerda) - alturaAVL(n->direita);
}

// Função para adicionar um nó
NoAVL* adicionarAVL(ArvoreAVL* arvore, NoAVL* no, int valor) {
    if (no == NULL) {
        return novo_noAVL(valor);
    }

    if (valor < no->valor) {
        no->esquerda = adicionarAVL(arvore, no->esquerda, valor);
        no->esquerda->pai = no;
    } else if (valor > no->valor) {
        no->direita = adicionarAVL(arvore, no->direita, valor);
        no->direita->pai = no;
    } else {
        // Valor já existe, não faz nada
        return no;
    }

    // Atualiza a altura do nó ancestral
    no->altura = maxAVL(alturaAVL(no->esquerda), alturaAVL(no->direita)) + 1;

    // Obtém o fator de balanceamento
    int balance = fator_balanceamentoAVL(no);

    // Se o nó ficar desbalanceado, há 4 casos

    // Caso 1: Rotação à direita
    if (balance > 1 && valor < no->esquerda->valor) {
        return rotacionar_direitaAVL(no);
    }

    // Caso 2: Rotação à esquerda
    if (balance < -1 && valor > no->direita->valor) {
        return rotacionar_esquerdaAVL(no);
    }

    // Caso 3: Rotação à esquerda-direita
    if (balance > 1 && valor > no->esquerda->valor) {
        no->esquerda = rotacionar_esquerdaAVL(no->esquerda);
        return rotacionar_direitaAVL(no);
    }

    // Caso 4: Rotação à direita-esquerda
    if (balance < -1 && valor < no->direita->valor) {
        no->direita = rotacionar_direitaAVL(no->direita);
        return rotacionar_esquerdaAVL(no);
    }

    return no;
}

// Função para remover um nó
NoAVL* removerAVL(ArvoreAVL* arvore, NoAVL* no, int valor) {
    if (no == NULL) return no;

    if (valor < no->valor) {
        no->esquerda = removerAVL(arvore, no->esquerda, valor);
    } else if (valor > no->valor) {
        no->direita = removerAVL(arvore, no->direita, valor);
    } else {
        // Nó encontrado, há 3 casos

        // Caso 1: Nó sem filhos
        if (no->esquerda == NULL && no->direita == NULL) {
            free(no);
            return NULL;
        }

        // Caso 2: Nó com um filho
        if (no->esquerda == NULL) {
            NoAVL* temp = no->direita;
            free(no);
            return temp;
        } else if (no->direita == NULL) {
            NoAVL* temp = no->esquerda;
            free(no);
            return temp;
        }

        // Caso 3: Nó com dois filhos
        NoAVL* temp = no->esquerda;
        while (temp->direita != NULL) {
            temp = temp->direita;
        }

        no->valor = temp->valor;
        no->esquerda = removerAVL(arvore, no->esquerda, temp->valor);
    }

    // Atualiza a altura do nó ancestral
    if (no) no->altura = maxAVL(alturaAVL(no->esquerda), alturaAVL(no->direita)) + 1;

    // Obtém o fator de balanceamento
    int balance = fator_balanceamentoAVL(no);

    // Se o nó ficar desbalanceado, há 4 casos

    // Caso 1: Rotação à direita
    if (balance > 1 && fator_balanceamentoAVL(no->esquerda) >= 0) {
        return rotacionar_direitaAVL(no);
    }

    // Caso 2: Rotação à esquerda
    if (balance < -1 && fator_balanceamentoAVL(no->direita) <= 0) {
        return rotacionar_esquerdaAVL(no);
    }

    // Caso 3: Rotação à esquerda-direita
    if (balance > 1 && fator_balanceamentoAVL(no->esquerda) < 0) {
        no->esquerda = rotacionar_esquerdaAVL(no->esquerda);
        return rotacionar_direitaAVL(no);
    }

    // Caso 4: Rotação à direita-esquerda
    if (balance < -1 && fator_balanceamentoAVL(no->direita) > 0) {
        no->direita = rotacionar_direitaAVL(no->direita);
        return rotacionar_esquerdaAVL(no);
    }

    return no;
}

// Função para contar o número de operações de balanceamento
int contador_adicao_avl(int* v, int n) {
    ArvoreAVL* a = criarAVL();
    int contador = 0;

    for (int i = 0; i < n; i++) {
        a->raiz = adicionarAVL(a, a->raiz, v[i]);
        contador += fator_balanceamentoAVL(a->raiz) > 1 || fator_balanceamentoAVL(a->raiz) < -1;
    }

    return contador;
}

int contador_remocao_avl(int* v, int n) {
    ArvoreAVL* a = criarAVL();
    int contador = 0;

    for (int i = 0; i < n; i++) {
        a->raiz = adicionarAVL(a, a->raiz, v[i]);
    }

    for (int i = 0; i < n; i++) {
        a->raiz = removerAVL(a, a->raiz, v[i]);
        contador += fator_balanceamentoAVL(a->raiz) > 1 || fator_balanceamentoAVL(a->raiz) < -1;
    }

    return contador;
}