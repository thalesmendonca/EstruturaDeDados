#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define TAM_NOME 100

typedef struct Departamento {
    int cod_dept;
    int sala;
    char nome[TAM_NOME];
} TDepartamento;

typedef struct Funcionario {
    int cod_func;
    int cod_dept;
    char nome[TAM_NOME];
} TFuncionario;

TFuncionario *le_funcionario(FILE *in)
{
    TFuncionario *func = (TFuncionario *) malloc(sizeof(TFuncionario));
    char linha[150];
    if (fgets (linha, 150, in) == NULL ) {
        free(func);
        return NULL;
    }

    char delimitador[] = ";";
    char *ptr;
    int cod;

    //quebra a linha
    ptr = strtok(linha, delimitador);
    cod = atoi(ptr);
    func->cod_func = cod;
    ptr = strtok(NULL, delimitador);
    cod = atoi(ptr);
    func->cod_dept = cod;
    ptr = strtok(NULL, delimitador);
    strcpy(func->nome, ptr);

    return func;
}

TDepartamento *le_departamento(FILE *in)
{
    TDepartamento *dept = (TDepartamento *) malloc(sizeof(TDepartamento));
    char linha[150];
    if (fgets (linha, 150, in) == NULL ) {
        free(dept);
        return NULL;
    }

    char delimitador[] = ";";
    char *ptr;
    int cod, sala;

    //quebra a linha
    ptr = strtok(linha, delimitador);
    cod = atoi(ptr);
    dept->cod_dept = cod;
    ptr = strtok(NULL, delimitador);
    sala = atoi(ptr);
    dept->sala = sala;
    ptr = strtok(NULL, delimitador);
    strcpy(dept->nome, ptr);

    return dept;
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

void leftOuterJoin(char *nome_arq_dept, char *nome_arq_funcionarios, char *nome_arq_join) {
    FILE *arq_dept = fopen(nome_arq_dept, "r"), *arq_join = fopen(nome_arq_join, "w+");
    TDepartamento *dpt = le_departamento(arq_dept);
    while(dpt != NULL){
      int flag = 0;
      FILE *arq_func = fopen(nome_arq_funcionarios, "r");
      TFuncionario *fnc = le_funcionario(arq_func);
      while(fnc != NULL){
        if(fnc->cod_dept == dpt->cod_dept){
          fprintf(arq_join, "%d", dpt->cod_dept);
          fprintf(arq_join, "%s", ";");
          fprintf(arq_join, "%d", dpt->sala);
          fprintf(arq_join, "%s", ";");
          fprintf(arq_join, "%s", dpt->nome);
          fprintf(arq_join, "%s", ";");
          fprintf(arq_join, "%d", fnc->cod_func);
          fprintf(arq_join, "%s", ";");
          fprintf(arq_join, "%s", fnc->nome);
          fprintf(arq_join, "%s", ";");
          fprintf(arq_join, "%s", "\n");
          flag = 1;  
        }
        fnc = le_funcionario(arq_func);
      }
      if(flag == 0){
        fprintf(arq_join, "%d", dpt->cod_dept);
          fprintf(arq_join, "%s", ";");
          fprintf(arq_join, "%d", dpt->sala);
          fprintf(arq_join, "%s", ";");
          fprintf(arq_join, "%s", dpt->nome);
          fprintf(arq_join, "%s", ";");
          fprintf(arq_join, "%d", 0);
          fprintf(arq_join, "%s", ";");
          fprintf(arq_join, "%s", ";");
          fprintf(arq_join, "%s", "\n");
      }
      dpt = le_departamento(arq_dept);
      fclose(arq_func);
    }
  fclose(arq_dept);
  fclose(arq_join);
}

int main() {
    leftOuterJoin("departamentos.txt", "funcionarios.txt", "join.txt");
    imprime_arquivo("join.txt");
}