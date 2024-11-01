#include <stdio.h>
#include <stdlib.h>

typedef struct noAVL {
    struct noAVL* pai;      // ponteiro para o nó pai
    struct noAVL* esquerda; // ponteiro para o nó filho a esquerda
    struct noAVL* direita;  // ponteiro para o nó filho a direita
    int valor;              // conteúdo de um nó arbitrário da árvore
    int altura;             // altura do nó
} NoAVL;

typedef struct arvoreAVL {
    struct noAVL* raiz;
} ArvoreAVL;

// assinatura das funções
int altura_avl(NoAVL* n);
int max_avl(int a, int b);
ArvoreAVL* criar_avl();
NoAVL* novo_no_avl(int valor);
NoAVL* rotacionar_direita_avl(NoAVL* y);
NoAVL* rotacionar_esquerda_avl(NoAVL* x);
int fator_balanceamento_avl(NoAVL* n);
NoAVL* adicionar_avl(ArvoreAVL* arvore, NoAVL* no, int valor);
NoAVL* remover_avl(ArvoreAVL* arvore, NoAVL* no, int valor);
int contador_adicao_avl(int* v, int n);
int contador_remocao_avl(int* v, int n);

// funções auxiliares
int altura_avl(NoAVL* n) {
    return n ? n->altura : 0;
}

int max_avl(int a, int b) {
    return (a > b) ? a : b;
}

// cria uma nova árvore
ArvoreAVL* criar_avl() {
    ArvoreAVL *a = (ArvoreAVL*) malloc(sizeof(ArvoreAVL));
    a->raiz = NULL;

    return a;
}

// cria um novo nó
NoAVL* novo_no_avl(int valor) {
    NoAVL* no    = (NoAVL*) malloc(sizeof(NoAVL));
    no->valor    = valor;
    no->esquerda = no->direita = NULL;
    no->pai      = NULL;
    no->altura   = 1;

    return no;
}

// rotaciona à direita
NoAVL* rotacionar_direita_avl(NoAVL* y) {
    NoAVL* x  = y->esquerda;
    NoAVL* T2 = x->direita;

    // realiza a rotação
    x->direita  = y;
    y->esquerda = T2;

    // atualiza pais
    if (T2) {
        T2->pai = y;
    }

    x->pai = y->pai;
    y->pai = x;

    // atualiza alturas
    y->altura = max_avl(altura_avl(y->esquerda), altura_avl(y->direita)) + 1;
    x->altura = max_avl(altura_avl(x->esquerda), altura_avl(x->direita)) + 1;

    return x;
}

// rotaciona à esquerda
NoAVL* rotacionar_esquerda_avl(NoAVL* x) {
    NoAVL* y  = x->direita;
    NoAVL* T2 = y->esquerda;

    // realiza a rotação
    y->esquerda = x;
    x->direita  = T2;

    // atualiza pais
    if (T2) {
        T2->pai = x;
    }

    y->pai = x->pai;
    x->pai = y;

    // atualiza alturas
    x->altura = max_avl(altura_avl(x->esquerda), altura_avl(x->direita)) + 1;
    y->altura = max_avl(altura_avl(y->esquerda), altura_avl(y->direita)) + 1;

    return y;
}

// fator de balanceamento
int fator_balanceamento_avl(NoAVL* n) {
    if (!n) {
        return 0;
    }

    return altura_avl(n->esquerda) - altura_avl(n->direita);
}

// adiciona um nó
NoAVL* adicionar_avl(ArvoreAVL* arvore, NoAVL* no, int valor) {
    if (no == NULL) {
        return novo_no_avl(valor);
    }

    if (valor < no->valor) {
        no->esquerda      = adicionar_avl(arvore, no->esquerda, valor);
        no->esquerda->pai = no;

    } else if (valor > no->valor) {
        no->direita      = adicionar_avl(arvore, no->direita, valor);
        no->direita->pai = no;

    } else {
        return no;
    }

    // atualiza a altura do nó ancestral
    no->altura = max_avl(altura_avl(no->esquerda), altura_avl(no->direita)) + 1;

    // fator de balanceamento
    int balance = fator_balanceamento_avl(no);

    // se o nó ficar desbalanceado:

    // caso 1: rotação à direita
    if (balance > 1 && valor < no->esquerda->valor) {
        return rotacionar_direita_avl(no);
    }

    // caso 2: rotação à esquerda
    if (balance < -1 && valor > no->direita->valor) {
        return rotacionar_esquerda_avl(no);
    }

    // caso 3: rotação à esquerda-direita
    if (balance > 1 && valor > no->esquerda->valor) {
        no->esquerda = rotacionar_esquerda_avl(no->esquerda);

        return rotacionar_direita_avl(no);
    }

    // caso 4: rotação à direita-esquerda
    if (balance < -1 && valor < no->direita->valor) {
        no->direita = rotacionar_direita_avl(no->direita);

        return rotacionar_esquerda_avl(no);
    }

    return no;
}

// remove um nó
NoAVL* remover_avl(ArvoreAVL* arvore, NoAVL* no, int valor) {
    if (no == NULL) {
        return no;
    }

    if (valor < no->valor) {
        no->esquerda = remover_avl(arvore, no->esquerda, valor);

    } else if (valor > no->valor) {
        no->direita = remover_avl(arvore, no->direita, valor);

    } else {
        // se nó for encontrado:

        // caso 1: nó sem filhos
        if (no->esquerda == NULL && no->direita == NULL) {
            free(no);

            return NULL;
        }

        // caso 2: nó com um filho
        if (no->esquerda == NULL) {
            NoAVL* temp = no->direita;
            free(no);

            return temp;

        } else if (no->direita == NULL) {
            NoAVL* temp = no->esquerda;
            free(no);

            return temp;
        }

        // caso 3: nó com dois filhos
        NoAVL* temp = no->esquerda;
        while (temp->direita != NULL) {
            temp = temp->direita;
        }

        no->valor    = temp->valor;
        no->esquerda = remover_avl(arvore, no->esquerda, temp->valor);
    }

    // atualiza a altura do nó ancestral
    if (no) no->altura = max_avl(altura_avl(no->esquerda), altura_avl(no->direita)) + 1;

    // fator de balanceamento
    int balance = fator_balanceamento_avl(no);

    // se o nó ficar desbalanceado:

    // caso 1: rotação à direita
    if (balance > 1 && fator_balanceamento_avl(no->esquerda) >= 0) {
        return rotacionar_direita_avl(no);
    }

    // caso 2: rotação à esquerda
    if (balance < -1 && fator_balanceamento_avl(no->direita) <= 0) {
        return rotacionar_esquerda_avl(no);
    }

    // caso 3: rotação à esquerda-direita
    if (balance > 1 && fator_balanceamento_avl(no->esquerda) < 0) {
        no->esquerda = rotacionar_esquerda_avl(no->esquerda);

        return rotacionar_direita_avl(no);
    }

    // caso 4: rotação à direita-esquerda
    if (balance < -1 && fator_balanceamento_avl(no->direita) > 0) {
        no->direita = rotacionar_direita_avl(no->direita);

        return rotacionar_esquerda_avl(no);
    }

    return no;
}

// conta o número de operações de balanceamento
int contador_adicao_avl(int* v, int n) {
    ArvoreAVL* a = criar_avl();
    int contador = 0;

    for (int i = 0; i < n; i++) {
        a->raiz   = adicionar_avl(a, a->raiz, v[i]);
        contador += fator_balanceamento_avl(a->raiz) > 1 || fator_balanceamento_avl(a->raiz) < -1;
    }

    return contador;
}

int contador_remocao_avl(int* v, int n) {
    ArvoreAVL* a = criar_avl();
    int contador = 0;

    for (int i = 0; i < n; i++) {
        a->raiz = adicionar_avl(a, a->raiz, v[i]);
    }

    for (int i = 0; i < n; i++) {
        a->raiz   = remover_avl(a, a->raiz, v[i]);
        contador += fator_balanceamento_avl(a->raiz) > 1 || fator_balanceamento_avl(a->raiz) < -1;
    }

    return contador;
}