#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int chave;
  long int dado1;
  char dado2[1001];
  char dado3[5001];
} Registro;

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



#endif