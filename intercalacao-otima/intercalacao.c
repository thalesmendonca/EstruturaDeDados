#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "nomes.c"
#include "cliente.c"
#include <string.h>
#include <limits.h>

typedef struct vetor {
    TCliente *cli;
    FILE *f;
} TVet;

typedef struct vetorNomes {
    char *nome;
} TVetNomes;

typedef struct No {
    TCliente *vencedor;
    struct No *endVencedor;
    FILE *f;
    struct No *pai;
    struct No *dir;
    struct No *esq;
} TNo;

typedef struct ListaNo {
    TNo *No;
    struct ListaNo *prox;
} TLNo;

void intercalacao_basico(char *nome_arquivo_saida, int num_p, TNomes *nome_particoes) {

    int fim = 0; //variavel que controla fim do procedimento
    FILE *out; //declara ponteiro para arquivo

    //abre arquivo de saida para escrita
    if ((out = fopen(nome_arquivo_saida, "wb")) == NULL) {
        printf("Erro ao abrir arquivo de sa?da\n");
    } else {
        //cria vetor de parti??es
        TVet v[num_p];

        //abre arquivos das particoes, colocando variavel de arquivo no campo f do vetor
        //e primeiro cliente do arquivo no campo cli do vetor
        for (int i=0; i < num_p; i++) {
            v[i].f = fopen(nome_particoes->nome, "rb");
            if (v[i].f != NULL) {
                TCliente *c = le_cliente(v[i].f);
                if (c == NULL) {
                    //arquivo estava vazio
                    //coloca HIGH VALUE nessa posi??o do vetor
                    v[i].cli = cliente(INT_MAX, "");
                }
                else {
                    //conseguiu ler cliente, coloca na posi??o atual do vetor
                    v[i].cli = c;
                }
            }
            else {
                fim = 1;
            }
            nome_particoes = nome_particoes->prox;
        }

        while (!(fim)) { //conseguiu abrir todos os arquivos
            int menor = INT_MAX;
            int pos_menor;
            //encontra o cliente com menor chave no vetor
            for(int i = 0; i < num_p; i++){
                if(v[i].cli->cod_cliente < menor){
                    menor = v[i].cli->cod_cliente;
                    pos_menor = i;
                }
            }
            if (menor == INT_MAX) {
                fim = 1; //terminou processamento
            }
            else {
                //salva cliente no arquivo de sa?da
                salva_cliente(v[pos_menor].cli, out);
                //atualiza posi??o pos_menor do vetor com pr?ximo cliente do arquivo
                TCliente *c = le_cliente(v[pos_menor].f);
                if (c == NULL) {
                    //arquivo estava vazio
                    //coloca HIGH VALUE nessa posi??o do vetor
                    v[pos_menor].cli = cliente(INT_MAX, "");
                }
                else {
                    v[pos_menor].cli = c;
                }

            }
        }

        //fecha arquivos das parti??es de entrada
        for(int i = 0; i < num_p; i++){
            fclose(v[i].f);
        }
        //fecha arquivo de sa?da
        if(nome_particoes == NULL){
            nome_particoes = insere_inicio(nome_particoes, nome_arquivo_saida);
        } else {
            insere_fim(nome_particoes, nome_arquivo_saida);
        }
        fclose(out);
    }
}

TNomes *gera_nomes_particoes(int num){
    TVetNomes nomes[num];
    TNomes* lista_nomes_particoes = NULL;
    for (int i = num-1; i >= 0; i--) {
        nomes[i].nome = (char *) malloc(sizeof(char[10]));
        char num_particao[10];
        strcpy(nomes[i].nome, "p");
        sprintf(num_particao, "%d", i+1);
        strcat(nomes[i].nome, num_particao);
        strcat(nomes[i].nome, ".txt");
        lista_nomes_particoes = insere_inicio(lista_nomes_particoes, nomes[i].nome);
    }
    return lista_nomes_particoes;
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

void imprime_arquivo(char *name) {
    FILE *arq; //declara ponteiro para arquivo
    // abre arquivo para leitura
    arq = fopen(name, "r");
    if (arq != NULL) {// checa se não deu erro na abertura do arquivo
        char linha[150];
        fgets (linha, 150, arq);
        while (!feof(arq)) {//testa se chegou ao final do arquivo
            printf("%s", linha);
            fgets (linha, 150, arq);
        }
        fclose(arq); //fecha arquivo
    } else printf("Erro ao abrir arquivo\n");
}

int intercalacao_otima(char *nome_arquivo_saida, int num_p, TNomes *nome_particoes, int f) {
    int n_particoes = 0;
    while(nome_particoes->prox != NULL){
        intercalacao_basico(gera_nome_particao(++num_p), f-1, nome_particoes);
        nome_particoes++;
    }
    rename(nome_particoes->nome, nome_arquivo_saida);
    return n_particoes;
}

int main() {
    int num_particoes;
    int f;
    int num_particoes_intermediarias;
    TNomes *nomes_particoes = NULL;
    //le numero de particoes a serem intercaladas
    scanf("%d", &num_particoes);
    nomes_particoes = gera_nomes_particoes(num_particoes); //lse
    //le f (número total de arquivos a serem manipulados ao mesmo tempo)
    //lembrar que o método usa f-1 arquivos para leitura, e 1 arquivo para saída
    scanf("%d", &f);
    num_particoes_intermediarias = intercalacao_otima("saida.txt", num_particoes, nomes_particoes, f);
    printf("%d\n", num_particoes_intermediarias);
    printf("*** saida.txt\n");
    if (num_particoes_intermediarias > 0)
        imprime_arquivo("saida.txt");
}