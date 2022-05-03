#include <stdio.h>

void merge(char *nomeArq1, char *nomeArq2, char *nomeArqMerge);

int main() {
    merge("/media/thales/BA843265843223F3/projetos-c/EstruturaDeDados/Arquivos/arquivo1", "/media/thales/BA843265843223F3/projetos-c/EstruturaDeDados/Arquivos/arquivo2", "/media/thales/BA843265843223F3/projetos-c/EstruturaDeDados/Arquivos/merge");
    return 0;
}

void merge(char *nomeArq1, char *nomeArq2, char *nomeArqMerge){
    FILE *a1, *a2, *merge;
    a1 = fopen(nomeArq1, "r");
    a2 = fopen(nomeArq2, "r");
    merge = fopen(nomeArqMerge, "w");
    if(a1 && a2 && merge){
        int n1, n2;
        int teste1 = fscanf(a1, "%d", &n1);
        int teste2 = fscanf(a2, "%d", &n2);
        while(!feof(a1) && !feof(a2)){
            if(n1 == n2){
                fprintf(merge, "%d\n", n1);
                fscanf(a1, "%d", &n1);
                fscanf(a2, "%d", &n2);
            }else if(n1 < n2){
                fprintf(merge, "%d\n", n1);
                fscanf(a1, "%d", &n1);
            } else {
                fprintf(merge, "%d\n", n2);
                fscanf(a2, "%d", &n2);
            }
        }
        if(feof(a1) && feof(a2)){
            fclose(a1);
            fclose(a2);
            return;
        }
        while(!feof(a1)){
            fprintf(merge, "%d\n", n1);
            fscanf(a1, "%d", &n1);
        }
        while(!feof(a2)){
            fprintf(merge, "%d\n", n2);
            fscanf(a2, "%d", &n2);
        }
        fclose(a1);
        fclose(a2);
    }else{
        printf("Problema em abrir os arquivos");
    }
}