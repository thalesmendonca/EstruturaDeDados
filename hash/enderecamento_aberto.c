#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cliente.c"

void inicializa(char *nome_arquivo, int m) {
    FILE *arq = fopen(nome_arquivo, "wb");
    if (arq != NULL) {
        int i;
        TCliente *c = cliente(-1, "", LIBERADO);
        for (i = 0; i < m; i++) {
            salva_cliente(c, arq);
        }
        fclose(arq);
    }
}

int hash_linha(int cod, int m) {
    return cod % m;
}

int hash(int cod, int m, int k) {
    return (hash_linha(cod, m) + k) % m;
}

int busca(int m, int mat, int *achou, char *nome_arquivo) {
    FILE* dados1 = fopen(nome_arquivo, "r");
    *achou = 0;
    int end;
    int pos_livre = -1;
    int k = 0;
    end = hash(mat, m, k);
    fseek(dados1, hash(mat, m, k)*tamanho_cliente(), SEEK_SET);
    TCliente* tab = le_cliente(dados1);
    while (k < m) {
        if (tab->cod != -1 && tab->cod == mat) {//encontrou chave
            *achou = 1;
            k = m; //força saída do loop
        }
        else {
            if (tab->cod == -1) {//encontrou endereço livre
                pos_livre = end;
                k = m; //força saída do loop
            }
            else {
                k = k + 1; //continua procurando//
                end = hash(mat, m, k);
                fseek(dados1, end*tamanho_cliente(), SEEK_SET);
                tab = le_cliente(dados1);

            }
        }
    }
    fclose(dados1);
    if (*achou)
        return end;
    else
        return pos_livre;
}

void insere(char *nome_arquivo, int m, int cod, char *nome) {
    FILE* dados = fopen(nome_arquivo, "r+b");
    int achou;
    int pont = busca(m, cod,&achou, nome_arquivo);
    if(!achou) {
        if (pont != -1) {
            fseek(dados, tamanho_cliente() * pont, SEEK_SET);
            TCliente *clienteASalvar = cliente(cod, nome, 1);
            salva_cliente(clienteASalvar, dados);
        }
    }
    fclose(dados);
}

void imprime(char *nome_arquivo, int m) {
    int i, h;
    FILE *arq = fopen(nome_arquivo, "rb");
    if (arq != NULL) {
        for (i = 0; i < m; i++) {
            h = i;
            printf("[%3d]", h);
            TCliente *c = le_cliente(arq);
            imprime_cliente(c);
        }
    }
    fclose(arq);
}


int main() {
    int n, m;
    int cod;
    char l[100];
    char nome[TAM_NOME];
    char delimitador[] = "-";
    char *ptr;
    //tamanho da tabela Hash
    scanf("%d", &m);
    //quantidade de clientes a serem inseridos
    scanf("%d", &n);

    if ((n < 1) || (m <= 1))
        return 0;

    //Caso você queira criar um arquivo novo zerado para testes, use essa função antes para inicializar o arquivo
    //inicializa("dados.dat", m);
    //imprime("dados.dat", m);


    //le os registros e insere na tabela
    //Informar 1 registro por linha, com código e nome, separados por traço
    //Exemplo para 2 registros:
    //3-ANA
    //6-MARIA
    for (int i = 0; i < n; i++) {
        scanf("%s", l);
        //quebra a string de entrada
        ptr = strtok(l, delimitador);
        cod = atoi(ptr);
        ptr = strtok(NULL, delimitador);
        strcpy(nome,ptr);
        insere("dados.dat", m, cod, nome);
    }
    imprime("dados.dat", m);
}