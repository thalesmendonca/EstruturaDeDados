#include <limits.h>
#include <stdio.h>

#include "cliente.c"
#include "compartimento_hash.c"

#define M 7

void imprime_arquivos(char *nome_arquivo_hash, char *nome_arquivo_dados) {
    //Imprime arquivo de hash
    printf("TABELA HASH:\n");
    imprime_arquivo_hash(nome_arquivo_hash);

    //Imprime arquivo de dados
    printf("\nDADOS:\n");
    imprime_arquivo_dados(nome_arquivo_dados);
}

/* Executa busca em Arquivos por Encadeamento Exterior (Hash)
 * Assumir que ponteiro para proximo noh eh igual a -1 quando nao houver proximo noh
 * Ponteiros são lógicos (é preciso multiplicar pelo tamanho do registro para calcular o valor do deslocamento
 * Use a função tamanho_compartimento() e tamanho_cliente() dependendo do arquivo que está sendo manipulado
 *
 * Parametros:
 * cod_cli: chave do cliente que esta sendo buscado
 * nome_arquivo_hash: nome do arquivo que contem a tabela hash
 * nome_arquivo_dados: nome do arquivo onde os dados estao armazenados
 * m: tamanho da tabela hash (a função de hash é h(x) = x mod m
 *
 * Retorna o endereco (lógico) onde o cliente foi encontrado, ou -1 se nao for encontrado
 */
int busca(int cod_cli, char *nome_arquivo_hash, char *nome_arquivo_dados, int m) {
    int end = cod_cli % m;
    FILE *arqHash = fopen(nome_arquivo_hash, "r");
    FILE *arqDados = fopen(nome_arquivo_dados, "r");

    fseek(arqHash, end*tamanho_compartimento(), SEEK_SET);
    TCompartimento* compartimento =  le_compartimento(arqHash);
    if(compartimento->prox == -1){return -1;}

    fseek(arqDados, compartimento->prox*tamanho_cliente(), SEEK_SET);
    TCliente* clienteLido = le_cliente(arqDados);
    int endLogico = -1;
    if(clienteLido->cod == cod_cli){
        endLogico = compartimento->prox;
    }
    imprime_cliente(clienteLido);
    while(clienteLido->prox != -1){
        int guardaEndLogico = clienteLido->prox;
        fseek(arqDados, tamanho_cliente()*clienteLido->prox, SEEK_SET);
        clienteLido = le_cliente(arqDados);
        if(clienteLido->cod == cod_cli){
            endLogico = guardaEndLogico;
        }
        imprime_cliente(clienteLido);
    }
    if(endLogico == -1){
        return -1;
    } else {
        fseek(arqDados, endLogico*tamanho_cliente(), SEEK_SET);
        clienteLido = le_cliente(arqDados);
        if(clienteLido->ocupado == 0){
            return -1;
        } else {
            return endLogico;
        }
    }
}

int main() {
    /* Essa função gera a saída que é usada no teste do run.codes. Ela NÃO DEVE SER MODIFICADA */
    int cod;
    int pont;

    //Imprime arquivos de entrada
    //imprime_arquivos("tabHash.dat", "clientes.dat");

    //le chave a ser buscada
    scanf("%d", &cod);
    printf("REGISTROS ACESSADOS:\n");
    pont = busca(cod, "tabHash.dat", "clientes.dat", M);

    //Imprime resultado da função
    printf("RESULTADO: %d", pont);
}