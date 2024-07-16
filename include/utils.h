#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

// Imprime os argumentos passados para o programa
void imprimirArgumentos(int, char *[]);

// Abre o arquivo de registros de acordo com a situação
FILE *abrirArquivoRegistros(int);

// Fecha o arquivo de registros
void fecharArquivoRegistros(FILE *);

#endif