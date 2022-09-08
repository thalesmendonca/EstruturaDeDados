#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define LIBERADO 0
#define OCUPADO 1

#define TAM_NOME 15

typedef struct Cliente {
    int cod;
    char nome[TAM_NOME];
    int ocupado;
} TCliente;

void imprime_cliente(TCliente *cliente)
{
	printf("%5d, %15s, %s\n", cliente->cod, cliente->nome,
           (cliente->ocupado? "OCUPADO" : "LIBERADO"));
}

TCliente *cliente(int cod, char *nome, int ocupado)
{
	TCliente *cliente = (TCliente *) malloc(sizeof(TCliente));
	if (cliente) memset(cliente, 0, sizeof(TCliente));
	cliente->cod = cod;
	strcpy(cliente->nome, nome);
	cliente->ocupado = ocupado;
	return cliente;
}

void salva_cliente(TCliente *cliente, FILE *out)
{
	fwrite(&cliente->cod, sizeof(int), 1, out);
	fwrite(cliente->nome, sizeof(char), sizeof(cliente->nome), out);
	fwrite(&cliente->ocupado, sizeof(int), 1, out);
}

TCliente *le_cliente(FILE *in)
{
	TCliente *cliente = (TCliente *) malloc(sizeof(TCliente));
	if (0 >= fread(&cliente->cod, sizeof(int), 1, in)) {
		free(cliente);
		return NULL;
	}
	fread(cliente->nome, sizeof(char), sizeof(cliente->nome), in);
	fread(&cliente->ocupado, sizeof(int), 1, in);
	return cliente;
}

int tamanho_cliente()
{
	return sizeof(int) + // cod
		sizeof(char) * TAM_NOME + // nome
		sizeof(int); //flag
}