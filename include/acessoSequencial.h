#ifndef ACESSOSEQUENCIAL_H
#define ACESSOSEQUENCIAL_H

#include "registro.h"
#include <stdbool.h>
#include <stdio.h>

const int ITENSPAGINA = 1024;
const int MAXTABELA = 1024;

// Estrutura de índice da tabela de paginas
typedef struct {
  unsigned int posicao;
  int chave;
} TipoIndice;

typedef Registro TipoItem;

// Funcao que realiza a pesquisa sequencial
bool pesquisaAcessoSequencial(TipoIndice tabelaIndices[], int tam,
                              TipoItem *pItem, FILE *pArquivo);

#endif