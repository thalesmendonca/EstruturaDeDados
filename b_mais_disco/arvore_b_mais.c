#include <stdio.h>
#include "metadados.c"
#include "no_interno.c"

void imprime_arquivos() {
    int i;
    //Imprime arquivo de metadados
    FILE *arq_m = fopen("metadados.dat", "rb");
    TMetadados *m = le_metadados(arq_m);
    printf("*** Arquivo de Metadados ***\n");
    imprime_metadados(m);
    fclose(arq_m);

    //Imprime arquivo de índice (nós internos da árvore)
    //A leitura não segue ordem específica -- os nós são lidos na ordem em que foram gravados no arquivo
    FILE *arq_i = fopen("indice.dat", "rb");
    printf("\n\n*** Arquivo de Indice ***\n");
    i = 0;
    TNoInterno *ni = le_no_interno(m->d, arq_i);
    while (ni != NULL) {
        printf("(((Endereco %d)))\n", i * tamanho_no_interno(m->d));
        imprime_no_interno(m->d, ni);
        ni = le_no_interno(m->d, arq_i);
        i += 1;
    }
    fclose(arq_i);

    //Imprime arquivo de dados (nós folha da árvore)
    //A leitura não segue ordem específica -- os nós são lidos na ordem em que foram gravados no arquivo
    FILE *arq_d = fopen("clientes.dat", "rb");
    printf("\n*** Arquivo de Dados ***\n");
    i = 0;
    TNoFolha *nf = le_no_folha(m->d, arq_d);
    while (nf != NULL) {
        printf("(((Endereco %d)))\n", i * tamanho_no_folha(m->d));
        imprime_no_folha(m->d, nf);
        nf = le_no_folha(m->d, arq_d);
        i += 1;
    }
    fclose(arq_d);
}

/*
 * Executa busca em Arquivos utilizando Arvore B+
 * Assumir que ponteiros para NULL têm valor -1
 *
 * chave: chave do cliente que esta sendo buscado
 * nome_arquivo_metadados: nome do arquivo binário que contem os metadados
 * nome_arquivo_indice: nome do arquivo binário de indice (que contem os nohs internos da arvore B+)
 * nome_arquivo_dados: nome do arquivo binário de dados (que contem as folhas da arvore B+)
 *
 * Retorna ponteiro para nó em que a chave está ou deveria estar
 */

int buscaChave(TNoInterno *raiz, int chave, FILE* arqIndice, FILE* arqDados, int d){
    for (int i = 0; i < raiz->m; ++i) {
        if(raiz->chaves[i] == -1 || raiz->chaves[i] > chave){
            if(raiz->aponta_folha){
                fseek(arqDados, raiz->p[i], SEEK_SET);
                imprime_no_folha(d, le_no_folha(d, arqDados));
                return raiz->p[i];
            }
            else {
                fseek(arqIndice, raiz->p[i], SEEK_SET);
                imprime_no_interno(d, le_no_interno(d, arqIndice));
                fseek(arqIndice, raiz->p[i], SEEK_SET);
                return buscaChave(le_no_interno(d, arqIndice), chave, arqIndice, arqDados, d);
            }
        } else if(i == raiz->m-1){
            if(raiz->aponta_folha){
                fseek(arqDados, raiz->p[i+1], SEEK_SET);
                imprime_no_folha(d, le_no_folha(d, arqDados));
                return raiz->p[i+1];
            } else {
                fseek(arqIndice, raiz->p[i+1], SEEK_SET);
                imprime_no_interno(d, le_no_interno(d, arqIndice));
                fseek(arqIndice, raiz->p[i+1], SEEK_SET);
                return buscaChave(le_no_interno(d, arqIndice), chave, arqIndice, arqDados, d) ;
            }
        }
    }
    return 0;
}

int busca(int chave, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados)
{
    FILE *arqMeta = fopen(nome_arquivo_metadados, "r");
    FILE *arqIndice = fopen(nome_arquivo_indice, "r");
    FILE *arqDados = fopen(nome_arquivo_dados, "r");
    TMetadados *metaRaiz = le_metadados(arqMeta);
    int d = metaRaiz->d;
    if(!(metaRaiz->raiz_folha)){
        fseek(arqIndice, metaRaiz->pont_raiz, SEEK_SET);
        TNoInterno *noInterno = le_no_interno(d, arqIndice);
        imprime_no_interno(d, noInterno);
        return buscaChave(noInterno, chave, arqIndice, arqDados, d);
    } else {
        fseek(arqDados, metaRaiz->pont_raiz, SEEK_SET);
        imprime_no_folha(d, le_no_folha(d, arqDados));
        return metaRaiz->pont_raiz;
    }
}

int main () {
    //Descomente essa linha de código caso deseje imprimir o conteúdo dos arquivos de entrada para analisar
    //o comportamento da sua implementação

    //imprime_arquivos();

    /* Le chave a ser buscada */
    int chave;

    scanf("%d", &chave);

    //Chama função de busca
    int pont = busca(chave, "metadados.dat",
                     "indice.dat",
                     "clientes.dat");

    //Imprime resultado da função
    printf("RESULTADO DA BUSCA: %d", pont);
}