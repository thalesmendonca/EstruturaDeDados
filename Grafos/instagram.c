#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct vizinho {
    char nome[10];
    struct vizinho *prox;
} TVizinho;

typedef struct grafo {
    char nome[10];
    int idade;
    TVizinho *prim_vizinho;
    struct grafo *prox;
} TGrafo;

TGrafo *insere_vertice(TGrafo *g, char *nome, int idade) {
    TGrafo *vertice = (TGrafo *) malloc(sizeof(TGrafo));
    strcpy(vertice->nome, nome);
    vertice->idade = idade;
    vertice->prox = g;
    vertice->prim_vizinho = NULL;
    return vertice;
}

TGrafo *busca_vertice(TGrafo *vertice, char *nome) {
    while ((vertice != NULL) && (strcmp(vertice->nome, nome) != 0)) {
        vertice = vertice->prox;
    }
    return vertice;
}

TVizinho *busca_vizinho(TVizinho *vizinho, char *nome) {
    while ((vizinho != NULL) && (strcmp(vizinho->nome, nome) != 0)) {
        vizinho = vizinho->prox;
    }
    return vizinho;
}

void insere_aresta(TGrafo *g, char *nome_origem, char *nome_destino) {
    TGrafo *vertice = busca_vertice(g, nome_origem);
    TVizinho *vizinho = (TVizinho *) malloc(sizeof(TVizinho));
    strcpy(vizinho->nome, nome_destino);
    vizinho->prox = vertice->prim_vizinho;
    vertice->prim_vizinho = vizinho;
}

void imprime(TGrafo *vertice) {
    while (vertice != NULL) {
        printf("Pessoa: %s - %d anos\n", vertice->nome, vertice->idade);
        printf("Segue: ");
        TVizinho *vizinho = vertice->prim_vizinho;
        while (vizinho != NULL) {
            printf("%s ", vizinho->nome);
            vizinho = vizinho->prox;
        }
        printf("\n\n");
        vertice = vertice->prox;
    }
}

int numero_seguidos(TGrafo *g, char *nome) {
    TGrafo *pessoa = busca_vertice(g, nome);
    if(pessoa == NULL) return 0;
    TVizinho *aux;
    if(pessoa->prim_vizinho != NULL){
        aux = pessoa->prim_vizinho;
    }else return 0;
    int qntdSeguidos = 1;
    aux = aux->prox;
    while(aux != NULL){
        qntdSeguidos++;
        aux = aux->prox;
    }
    return qntdSeguidos;
}

TVizinho *insere_lse(TVizinho *lista, char *nome){
    TVizinho *novo = (TVizinho*) malloc(sizeof(TVizinho));
    novo->prox = NULL;
    strcpy(novo->nome, nome);
    if(lista == NULL){
        lista = novo;
        return lista;
    }
    TVizinho *aux = lista;
    while(aux->prox != NULL){
        aux = aux->prox;
    }
    aux->prox = novo;
    return lista;
}

int seguidores(TGrafo *vertice, char *nome, int imprime) {
    int qntdSeguidores = 0;
    TGrafo *aux = vertice;
    TVizinho *seguido;
    TVizinho *listaNomes = NULL;
    while(aux != NULL){
        seguido = busca_vizinho(aux->prim_vizinho, nome);
        if(seguido != NULL){
            qntdSeguidores++;
            listaNomes = insere_lse(listaNomes, aux->nome);
        }

        aux = aux->prox;
    }
    if(imprime){
        while(listaNomes != NULL){
            printf("%s ", listaNomes->nome);
            listaNomes = listaNomes->prox;
        }
        printf("\n");
    }
    return qntdSeguidores;
}

TGrafo *mais_popular(TGrafo *g) {
    int maior = 0;
    TGrafo *maisPopular, *aux = g;
    while(aux != NULL){
        int numeroSeguidores = seguidores(g,aux->nome,0);
        if(numeroSeguidores > maior){
            maior = numeroSeguidores;
            maisPopular = aux;
        }
        aux = aux->prox;
    }
    if(maior == 0) return g;
    return maisPopular;
}

int segue_mais_velho(TGrafo *g, int imprime) {
    TGrafo *auxGrafo = g;
    TVizinho *auxVizinhos;
    TVizinho *listaNomes = NULL;
    int qntdSegueVelhos = 0;
    while(auxGrafo != NULL){
        auxVizinhos = auxGrafo->prim_vizinho;
        while(auxVizinhos != NULL){
            if(busca_vertice(g, auxVizinhos->nome)->idade < auxGrafo->idade) break;
            auxVizinhos = auxVizinhos->prox;
            if(auxVizinhos == NULL) {
                listaNomes = insere_lse(listaNomes, auxGrafo->nome);
            }
        }

        auxGrafo = auxGrafo->prox;
    }
    if(imprime){
        for (; listaNomes != NULL ; listaNomes=listaNomes->prox) {
            printf("%s ", listaNomes->nome);
        }
        printf("\n");
    }
    return qntdSegueVelhos;
}


void libera_vizinho(TVizinho *vizinho) {
    if (vizinho != NULL) {
        libera_vizinho(vizinho->prox);
        free(vizinho);
    }
}

void libera_vertice(TGrafo *vertice) {
    if (vertice != NULL) {
        libera_vizinho(vertice->prim_vizinho);
        libera_vertice(vertice->prox);
        free(vertice);
    }
}

int main() {
    /* A função main lê os dados de entrada, cria o grafo e chama as funções solicitadas no problema
    * depois imprime os resultados solicitados
    * ELA NÃO DEVE SER MODIFICADA
    * */
    int num_vertices, num_arestas;
    char nome[30];
    char origem[30], destino[30];
    char l[100];
    char delimitador[] = "-";
    char *ptr;
    int idade;
    int i;
    TGrafo *g = NULL;

    //le numero de vertices
    scanf("%d", &num_vertices);
    //le e cria os vertices
    for (i = 0; i < num_vertices; i++) {
        scanf("%s", l);
        //quebra a string de entrada
        ptr = strtok(l, delimitador);
        strcpy(nome,ptr);
        //printf("%s", nome);
        ptr = strtok(NULL, delimitador);
        idade = atoi(ptr);
        g = insere_vertice(g, nome, idade);
    }

    //Le numero de arestas e depois le os dados de cada aresta
    //Cria as arestas no grafo
    scanf("%d", &num_arestas);
    for (i = 0; i < num_arestas; i++) {
        scanf("%s", l);
        //quebra a string de entrada
        ptr = strtok(l, delimitador);
        strcpy(origem,ptr);
        //printf("%s", nome);
        ptr = strtok(NULL, delimitador);
        strcpy(destino,ptr);
        insere_aresta(g, origem, destino);
    }

    //Le nome de pessoa
    scanf("%s", nome);

    //Encontra o número de seguidos dessa pessoa
    printf("SEGUIDOS por %s: %d\n", nome, numero_seguidos(g, nome));

//Encontra os seguidores de uma determinada pessoa
    printf("SEGUIDORES de %s:\n", nome);
    seguidores(g, nome, 1);

    TGrafo *p;

    //Encontra mais popular
    p = mais_popular(g);
    printf("MAIS POPULAR: %s\n", p->nome);
//
    //Encontra as pessoas que seguem apenas pessoas mais velhas
    printf("SEGUEM APENAS PESSOAS MAIS VELHAS:\n");
    segue_mais_velho(g,1);

    libera_vertice(g);
}