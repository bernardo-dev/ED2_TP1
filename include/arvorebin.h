#ifndef ARVOREBIN_H
#define ARVOREBIN_H

#include "registro.h"
#include <stdio.h>
#include "utils.h"

typedef struct no {
  Registro registro;
  struct no *esquerda, *direita;
} No;

typedef struct {
  Registro registro;
  int posEsquerda, posDireita;
} NoExterno;

typedef struct arvore {
  No *raiz;
} ArvoreBin;

// Insere um registro na arvore binaria
void inserir(No **no, Registro registro);

// Monta a arvore binaria a partir de um arquivo de registros
void montaArvoreBinaria(ArvoreBin *arvore, FILE *arq, int);

// Monta um arquivo binario com a arvore binaria
void montaArquivo(FILE *arq, No *no, int *pos, Metrica *metrica);

// Busca um registro na arvore binaria
Registro *buscaChave(FILE *arq, int chave, int posAtual, Metrica *metrica);

void montaArvoreBinariaExterna(ArvoreBin *arvore, FILE *arq, int quantidade, Metrica *metrica);

void inserirExterno(FILE *arq, No **no, Registro registro, int *pos, Metrica *metrica);

#endif