#include <stdio.h>
#include <stdlib.h>

typedef struct sNoA {
    int chave;
    struct sNoA *esq;
    struct sNoA *dir;
} TNoA;

void imprime(TNoA *nodo, int tab) {
    for (int i = 0; i < tab; i++) {
        printf("-");
    }
    if (nodo != NULL) {
        printf("%d\n", nodo->chave);
        imprime(nodo->esq, tab + 2);
        printf("\n");
        imprime(nodo->dir, tab + 2);
    } else printf("vazio");
}

TNoA *busca(TNoA *no, int chave) {
    while (no != NULL) {
        if (no->chave == chave)
            return no; //achou retorna o ponteiro para o nó
        else if (no->chave > chave)
            no = no->esq;
        else
            no = no->dir;
    }
    return NULL; //não achou, retorna null
}

TNoA *buscaRecursiva(TNoA *no, int chave) {
    if (no == NULL)
        return NULL;
    else if (no->chave == chave)
        return no;
    else if (no->chave > chave)
        return buscaRecursiva(no->esq, chave);
    else
        return buscaRecursiva(no->dir, chave);
}

TNoA *insere(TNoA *no, int chave) {
    if (no == NULL) {
        no = (TNoA *) malloc(sizeof(TNoA));
        no->chave = chave;
        no->esq = NULL;
        no->dir = NULL;
    } else if (chave < (no->chave))
        no->esq = insere(no->esq, chave);
    else if (chave > (no->chave)) {
        no->dir = insere(no->dir, chave);
    }
    else {
        printf("Inserção inválida! ");
        exit(1);
    }
    return no;
}

void criArvoreBalanceada(TNoA* raiz, int v[], int inicio, int fim){
    if(inicio <= fim) {
        int meio = (inicio + fim) / 2;
        raiz = insere(raiz, v[meio]);
        //constroi subarvore esq e dir
        criArvoreBalanceada(raiz, v, inicio, meio - 1);
        criArvoreBalanceada(raiz, v, meio + 1, fim);
    }
}

int main(void) {
//    TNoA *raiz, *no;
//    raiz = NULL;
//    raiz = insere(raiz, 500);
//    insere(raiz, 600);
//    insere(raiz, 550);
//    insere(raiz, 400);
//    imprime(raiz, 0);
//
//    no = busca(raiz, 550);
//    if (no != NULL) {
//        printf("\n\nEncontrou nó de chave %d.", no->chave);
//    } else printf("Não encontrou!");
//
//    no = busca(raiz, 400);
//    if (no != NULL) {
//        printf("\n\nEncontrou nó de chave %d.\n", no->chave);
//    } else printf("Não encontrou!\n");
//
//    imprime(raiz, 0);

    int tam = 7;
    int v[] = {150, 300, 400, 500, 600, 800, 900};
    TNoA *raiz;
    raiz = NULL;
    criArvoreBalanceada(raiz, v, 0, tam-1);
    imprime(raiz, 0);
};