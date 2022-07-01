#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <limits.h>
#include <dirent.h>
#include <string.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define TAM_NOME 100

typedef struct Cliente {
    int cod_cliente;
    char nome[TAM_NOME];
} TCliente;

typedef struct vetor {
    TCliente *cli;
} TVet;

void imprime_cliente(TCliente *cliente)
{
    printf("%d, %s\n", cliente->cod_cliente, cliente->nome);
}

TCliente *cliente(int cod, char *nome)
{
    TCliente *cliente = (TCliente *) malloc(sizeof(TCliente));
    if (cliente) memset(cliente, 0, sizeof(TCliente));
    cliente->cod_cliente = cod;
    strcpy(cliente->nome, nome);
    return cliente;
}

void salva_cliente(TCliente *cliente, FILE *out)
{
    fprintf(out, "%d", cliente->cod_cliente);
    fprintf(out, "%c", ';');
    fprintf(out, "%s", cliente->nome);
    fprintf(out, "%c", ';');
    fprintf(out, "%s", "\n");
}

TCliente *le_cliente(FILE *in)
{
    TCliente *cliente = (TCliente *) malloc(sizeof(TCliente));
    char linha[150];
    if (fgets (linha, 150, in) == NULL ) {
        free(cliente);
        return NULL;
    }

    char delimitador[] = ";";
    char *ptr;
    int cod;

    //quebra a linha
    ptr = strtok(linha, delimitador);
    cod = atoi(ptr);
    cliente->cod_cliente = cod;
    ptr = strtok(NULL, delimitador);
    strcpy(cliente->nome, ptr);

    return cliente;
}

void imprime_arquivo(char *nome) {
    FILE *arq; //declara ponteiro para arquivo
    // abre arquivo para leitura
    arq = fopen(nome, "r");
    if (arq != NULL) {// checa se não deu erro na abertura do arquivo
        char linha[150];
        fgets(linha, 150, arq);
        while (!feof(arq)) {//testa se chegou ao final do arquivo
            printf("%s", linha);
            fgets(linha, 150, arq);
        }
        fclose(arq); //fecha arquivo
    } else printf("Erro ao abrir arquivo\n");
}

char *gera_nome_particao(int num_part) {
    char *nome_particao = (char *) malloc(sizeof(char[10]));
    char num_particao[10];

    strcpy(nome_particao, "p");
    sprintf(num_particao, "%d", num_part);
    strcat(nome_particao, num_particao);
    strcat(nome_particao, ".txt");

    return nome_particao;
}



int selecao_natural(char *nome_arquivo_entrada, int tam_memoria) {
    int numeroParticoes = 0;

    FILE *arquivoEntrada;
    if((arquivoEntrada = fopen(nome_arquivo_entrada, "r")) == NULL){
        printf("Erro ao abrir arquivo");
        return  numeroParticoes;
    }

    TCliente *vetorDeCliente[tam_memoria], *vetorReservatorio[tam_memoria];

    //preenche vetor pela primeira vez
    for (int i = 0; i < tam_memoria; ++i) {
        vetorDeCliente[i] = (TCliente *) malloc(sizeof(TCliente));
        vetorDeCliente[i] = le_cliente(arquivoEntrada);
    }

    FILE *particaoDaVez = fopen(gera_nome_particao(++numeroParticoes), "w");
    int indexReservatorio = 0, indexMenorCliente = 0, flag = 1;

        while(flag){
            int codMenorCliente = vetorDeCliente[indexMenorCliente]->cod_cliente;
            //encontra menor cliente do vetor
            for (int i = 1; i < tam_memoria; ++i) {
                if(vetorDeCliente[i]->cod_cliente < vetorDeCliente[indexMenorCliente]->cod_cliente){
                    indexMenorCliente = i;
                    codMenorCliente = vetorDeCliente[i]->cod_cliente;
                }
            }
            salva_cliente(vetorDeCliente[indexMenorCliente], particaoDaVez);
            vetorDeCliente[indexMenorCliente] = le_cliente(arquivoEntrada);
            if(vetorDeCliente[indexMenorCliente] != NULL){
                while((vetorDeCliente[indexMenorCliente]->cod_cliente < codMenorCliente) && indexReservatorio < tam_memoria){
                    vetorReservatorio[indexReservatorio] = vetorDeCliente[indexMenorCliente];
                    vetorDeCliente[indexMenorCliente] = NULL;
                    indexReservatorio++;
                    if(indexReservatorio >= tam_memoria){
                        break;
                    }
                    vetorDeCliente[indexMenorCliente] = le_cliente(arquivoEntrada);
                    if(vetorDeCliente[indexMenorCliente] == NULL){
                        flag = 0;
                        break;
                    }
                }
            } else flag = 0;

            if(indexReservatorio == tam_memoria){
                for (int i = 0; i < tam_memoria; ++i) {
                    if(vetorDeCliente[i] != NULL){
                        for (int j = i+1; j < tam_memoria; ++j) {
                            if(vetorDeCliente[j] != NULL){
                                if(vetorDeCliente[j]->cod_cliente < vetorDeCliente[i]->cod_cliente){
                                    TCliente *temp = vetorDeCliente[i];
                                    vetorDeCliente[i] = vetorDeCliente[j];
                                    vetorDeCliente[j] = temp;
                                }

                            }
                        }
                        salva_cliente(vetorDeCliente[i], particaoDaVez);
                        vetorDeCliente[i] = NULL;
                    }

                }
                fclose(particaoDaVez);
                particaoDaVez = fopen(gera_nome_particao(++numeroParticoes), "w");
                for (int i = 0; i < tam_memoria; ++i) {
                    vetorDeCliente[i] = vetorReservatorio[i];
                    vetorReservatorio[i] = NULL;
                }
                indexReservatorio = 0;
            }
            indexMenorCliente = 0;
        }

    for (int i = 0; i < tam_memoria; ++i) {
        if(vetorDeCliente[i] != NULL){
            for (int j = i+1; j < tam_memoria; ++j) {
                if(vetorDeCliente[j] != NULL){
                    if(vetorDeCliente[j]->cod_cliente < vetorDeCliente[i]->cod_cliente){
                        TCliente *temp = vetorDeCliente[i];
                        vetorDeCliente[i] = vetorDeCliente[j];
                        vetorDeCliente[j] = temp;
                    }

                }
            }
            salva_cliente(vetorDeCliente[i], particaoDaVez);
            vetorDeCliente[i] = NULL;
        }

    }

    for (int i = 0; i < tam_memoria; ++i) {
        vetorDeCliente[i] = vetorReservatorio[i];
        vetorReservatorio[i] = NULL;
    }

    for (int i = 0; i < tam_memoria; ++i) {
        if(vetorDeCliente[i] != NULL){
            for (int j = i+1; j < tam_memoria; ++j) {
                if(vetorDeCliente[j] != NULL){
                    if(vetorDeCliente[j]->cod_cliente < vetorDeCliente[i]->cod_cliente){
                        TCliente *temp = vetorDeCliente[i];
                        vetorDeCliente[i] = vetorDeCliente[j];
                        vetorDeCliente[j] = temp;
                    }

                }
            }
            fclose(particaoDaVez);
            particaoDaVez = fopen(gera_nome_particao(++numeroParticoes), "w");
            salva_cliente(vetorDeCliente[i], particaoDaVez);
            vetorDeCliente[i] = NULL;
        }

    }

    fclose(particaoDaVez);

    return numeroParticoes;
}

int main() {
    int tam_memoria;
    scanf("%d", &tam_memoria);
    int num_part = selecao_natural("D:\\projetos-c\\EstruturaDeDados\\selecao-natural\\entrada.txt", tam_memoria);
    for (int i = 1; i < num_part+1; i++) {
        printf("*** %s\n", gera_nome_particao(i));
        imprime_arquivo(gera_nome_particao(i));
    }
}