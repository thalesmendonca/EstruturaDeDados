#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sNoA {
    int chave;
    struct sNoA *esq;
    struct sNoA *dir;
} TNoA;

TNoA *menor(TNoA *raiz){
    while (raiz->dir != NULL) raiz = raiz->dir;
    return raiz;
}

TNoA *exclui(TNoA *raiz, int chave) {
    if (raiz == NULL)
        return NULL;
    else if (raiz->chave > chave){
        raiz->esq =  exclui(raiz->esq, chave);
    } else if (raiz->chave < chave){
        raiz->dir = exclui(raiz->dir, chave);
    } else {
        if(raiz->esq == NULL && raiz->dir == NULL){
            free(raiz);
            raiz = NULL;
        } else if(raiz->esq == NULL){
            TNoA *temp = raiz;
            raiz = raiz->dir;
            free(temp);
        } else if(raiz->dir == NULL){
            TNoA *temp = raiz;
            raiz = raiz->esq;
            free(temp);
        } else {
            TNoA *substituto = menor(raiz->esq);
            raiz->chave = substituto->chave;
            substituto->chave = chave;
            exclui(raiz->esq, chave);
        }
    }
    return raiz;
}



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
    } else {
        printf("Inserção inválida! ");
        exit(1);
    }
    return no;
}

int main(void) {

    /* A função main lê os dados de entrada, cria a árvore e chama a função solicitada no problema
     * depois imprime o resultado solicitado
     * ELA NÃO DEVE SER MODIFICADA
     * */
    TNoA *raiz;
    raiz = NULL;

    char l[100];
    char delimitador[] = "-";
    char *ptr;
    int valor;

    /* lê valores para criar a arvore
     * valores devem ser informados separados por traço
     * exemplo: 1-3-5-2-7-9-21-6 */
    scanf("%s", l);
    //quebra a string de entrada
    ptr = strtok(l, delimitador);
    while(ptr != NULL) {
        valor = atoi(ptr);
        raiz = insere(raiz, valor);
        ptr = strtok(NULL, delimitador);
    }

    //le valor a ser excluido
    scanf("%d", &valor);
    //Chama função
    raiz = exclui(raiz, valor);
    imprime(raiz, 0);
};