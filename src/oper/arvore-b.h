#include <stdio.h>
#include <stdlib.h>

typedef struct noB {
    int total;
    int* chaves;
    struct noB** filhos;
    struct noB* pai; 
} NoB;

typedef struct arvoreB {
    NoB* raiz;
    int ordem;
} ArvoreB;

ArvoreB* criaArvoreB(int);
NoB* criaNoB(ArvoreB*);
void adicionaChaveB(ArvoreB*, int);
void removeChaveB(ArvoreB*, int);
int transbordoB(ArvoreB*, NoB*);
NoB* divideNoB(ArvoreB*, NoB*);
void adicionaChaveRecursivoB(ArvoreB*, NoB*, NoB*, int);
void removeChaveRecursivoB(ArvoreB*, NoB*, int);
void balanceiaNoB(ArvoreB*, NoB*);
void emprestaChaveB(NoB*, NoB*);
void fundeNosB(ArvoreB*, NoB*, NoB*);
int pesquisaBinariaB(NoB*, int);
NoB* localizaNoB(ArvoreB*, int);
NoB* localizaChaveB(ArvoreB*, int);

// Variável global para contagem
int contador = 0;

ArvoreB* criaArvoreB(int ordem) {
    ArvoreB* a = malloc(sizeof(ArvoreB));
    a->ordem = ordem;
    a->raiz = criaNoB(a);
    return a;
}

NoB* criaNoB(ArvoreB* arvore) {
    int max = arvore->ordem * 2;
    NoB* no = malloc(sizeof(NoB));
    no->pai = NULL;
    no->chaves = malloc(sizeof(int) * (max + 1));
    no->filhos = malloc(sizeof(NoB*) * (max + 2));
    no->total = 0;

    for (int i = 0; i < max + 2; i++)
        no->filhos[i] = NULL;

    return no;
}

int pesquisaBinariaB(NoB* no, int chave) {
    int inicio = 0, fim = no->total - 1, meio;		
    
    while (inicio <= fim) {	
        contador++;
        meio = (inicio + fim) / 2;
        
        if (no->chaves[meio] == chave) {	
            return meio; // encontrou	
        } else if (no->chaves[meio] > chave) {
            fim = meio - 1;	
        } else {
            inicio = meio + 1;
        }
    }
    return inicio; // não encontrou	
}

NoB* localizaNoB(ArvoreB* arvore, int chave) {	
    NoB *no = arvore->raiz;
    
    while (no != NULL) {
        contador++;
        int i = pesquisaBinariaB(no, chave);
        if (no->filhos[i] == NULL)
            return no; // encontrou nó
        else
            no = no->filhos[i];
    }
    return NULL; // não encontrou nenhum nó
}

void adicionaChaveNoB(NoB* no, NoB* novo, int chave) {
    int i = pesquisaBinariaB(no, chave);
    contador++;

    for (int j = no->total - 1; j >= i; j--) {
        no->chaves[j + 1] = no->chaves[j];
        no->filhos[j + 2] = no->filhos[j + 1];
    }
    
    no->chaves[i] = chave;
    no->filhos[i + 1] = novo;
    no->total++;
}

int transbordoB(ArvoreB* arvore, NoB* no) {
    contador++;
    return no->total > arvore->ordem * 2;
}

NoB* divideNoB(ArvoreB* arvore, NoB* no) {
    int meio = no->total / 2;
    NoB* novo = criaNoB(arvore);
    novo->pai = no->pai;

    contador++;
    
    for (int i = meio + 1; i < no->total; i++) {
        novo->filhos[novo->total] = no->filhos[i];
        novo->chaves[novo->total] = no->chaves[i];
        if (novo->filhos[novo->total] != NULL) novo->filhos[novo->total]->pai = novo;
        novo->total++;
    }

    novo->filhos[novo->total ] = no->filhos[no->total];
    if (novo->filhos[novo->total] != NULL) novo->filhos[novo->total]->pai = novo;    
    no->total = meio;
    return novo;
}

void adicionaChaveRecursivoB(ArvoreB* arvore, NoB* no, NoB* novo, int chave) {
    contador++;
    adicionaChaveNoB(no, novo, chave);
    
    if (transbordoB(arvore, no)) {
        int promovido = no->chaves[arvore->ordem]; 
        NoB* novo = divideNoB(arvore, no);

        if (no->pai == NULL) {
            contador++;
            NoB* pai = criaNoB(arvore);            
            pai->filhos[0] = no;
            adicionaChaveNoB(pai, novo, promovido);
            
            no->pai = pai;
            novo->pai = pai;
            arvore->raiz = pai;
        } else
            adicionaChaveRecursivoB(arvore, no->pai, novo, promovido);
    }
}

void adicionaChaveB(ArvoreB* arvore, int chave) {
    NoB* no = localizaNoB(arvore, chave);
    adicionaChaveRecursivoB(arvore, no, NULL, chave);
}

void removeChaveRecursivoB(ArvoreB* arvore, NoB* no, int chave) {
    contador++;
    int i = pesquisaBinariaB(no, chave);
    
    if (i < no->total && no->chaves[i] == chave) {
        if (no->filhos[i] == NULL) {
            // Remoção de chave folha
            for (int j = i; j < no->total - 1; j++) {
                no->chaves[j] = no->chaves[j + 1];
                no->filhos[j + 1] = no->filhos[j + 2];
            }
            no->total--;
        } else {
            // Remoção de chave não-folha
            NoB* filhoEsquerdo = no->filhos[i];
            NoB* filhoDireito = no->filhos[i + 1];
            int chaveSubstituta;

            if (filhoEsquerdo->total > filhoDireito->total) {
                chaveSubstituta = filhoEsquerdo->chaves[filhoEsquerdo->total - 1];
                removeChaveRecursivoB(arvore, filhoEsquerdo, chaveSubstituta);
                no->chaves[i] = chaveSubstituta;
            } else {
                chaveSubstituta = filhoDireito->chaves[0];
                removeChaveRecursivoB(arvore, filhoDireito, chaveSubstituta);
                no->chaves[i] = chaveSubstituta;
            }
        }
    } else {
        NoB* filho = no->filhos[i];
        removeChaveRecursivoB(arvore, filho, chave);
    }

    if (no->total < arvore->ordem) {
        balanceiaNoB(arvore, no);
    }
}

void removeChaveB(ArvoreB* arvore, int chave) {
    NoB* no = localizaChaveB(arvore, chave);
    if (no != NULL) {
        removeChaveRecursivoB(arvore, no, chave);
    }
}

void balanceiaNoB(ArvoreB* arvore, NoB* no) {
    if (no->pai == NULL) return;

    NoB* pai = no->pai;
    int i;
    for (i = 0; i <= pai->total; i++) {
        if (pai->filhos[i] == no) break;
    }

    if (i == 0) {
        if (pai->filhos[1]->total > arvore->ordem) {
            emprestaChaveB(no, pai->filhos[1]);
        } else {
            fundeNosB(arvore, no, pai->filhos[1]);
        }
    } else if (i == pai->total) {
        if (pai->filhos[i - 1]->total > arvore->ordem) {
            emprestaChaveB(no, pai->filhos[i - 1]);
        } else {
            fundeNosB(arvore, pai->filhos[i - 1], no);
        }
    } else {
        if (pai->filhos[i - 1]->total > pai->filhos[i + 1]->total) {
            emprestaChaveB(no, pai->filhos[i - 1]);
        } else {
            emprestaChaveB(no, pai->filhos[i + 1]);
        }
    }
}

void emprestaChaveB(NoB* no, NoB* irmao) {
    contador++;
    int chave = irmao->chaves[irmao->total - 1];
    adicionaChaveNoB(no, NULL, chave);
    irmao->total--;
}

void fundeNosB(ArvoreB* arvore, NoB* no, NoB* irmao) {
    contador++;
    adicionaChaveNoB(no, irmao, irmao->chaves[0]);
    no->total += irmao->total;
    free(irmao);
}

NoB* localizaChaveB(ArvoreB* arvore, int chave) {	
    NoB *no = arvore->raiz;
    
    while (no != NULL) {
        contador++;
        int i = pesquisaBinariaB(no, chave);

        if (i < no->total && no->chaves[i] == chave) {
            return no; // encontrou
        } else {
            no = no->filhos[i];
        }
    }

    return NULL; // não encontrou
}

int contador_adicao_b(int n, int* v) {
    ArvoreB* arvore = criaArvoreB(1);
    contador = 0;

    for (int i = 0; i < n; i++) {
        adicionaChaveB(arvore, v[i]);
    }

    int resultado = contador;
    free(arvore->raiz);
    free(arvore);
    return resultado;
}

int contador_remocao_b(int n, int* v) {
    ArvoreB* arvore = criaArvoreB(1);
    contador = 0;

    for (int i = 0; i < n; i++) {
        adicionaChaveB(arvore, v[i]);
    }

    for (int i = 0; i < n; i++) {
        removeChaveB(arvore, v[i]);
    }

    int resultado = contador;
    free(arvore->raiz);
    free(arvore);
    return resultado;
}