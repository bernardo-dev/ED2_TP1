#ifndef ARVOREBIN_H
#define ARVOREBIN_H

#include "registro.h"

typedef struct no{
    Registro registro;
    struct no *esquerda,*direita;
}No;

typedef struct 
{
  Registro registro;
  int posEsquerda,posDireita;
  
}NoExterno;

typedef struct arvore{
    No *raiz;
}ArvoreBin;

void inserir(No **no, Registro registro);
void montaArvore(ArvoreBin *arvore, FILE *arq);
void montaArquivo(FILE *arq, No *no, int *pos);
Registro* buscaChave(FILE *arq, int chave, int posAtual);
#endif 