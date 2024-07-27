#include "../include/arvorebin.h"
#include <stdlib.h>

void inserir(No **no, Registro registro) {
  No *novo;

  if (*no == NULL) {
    novo = (No *)malloc(sizeof(No));
    if (novo == NULL) {
      fprintf(stderr, "Erro ao alocar memória\n");
      return;
    }
    novo->direita = NULL;
    novo->esquerda = NULL;
    novo->registro = registro;
    *no = novo;
  } else {
    if (registro.chave < (*no)->registro.chave) {
      inserir(&((*no)->esquerda), registro);
    } else {
      inserir(&((*no)->direita), registro);
    }
  }
}

void montaArvoreBinaria(ArvoreBin *arvore, FILE *arq, int quantidade) {
  Registro registro;
  for (int i = 0; i < quantidade; i++) {
    if (fread(&registro, sizeof(Registro), 1, arq) != 1) {
      return;
    }
    inserir(&(arvore->raiz), registro);
  }
}

void montaArquivo(FILE *arq, No *no, int *pos) {
  if (no == NULL)
    return;

  NoExterno novoExterno;
  novoExterno.registro = no->registro;
  novoExterno.posDireita = -1;
  novoExterno.posEsquerda = -1;

  fseek(arq, (*pos) * sizeof(NoExterno), SEEK_SET);
  fwrite(&novoExterno, sizeof(NoExterno), 1, arq);
  int currentPos = *pos;
  (*pos)++;

  if (no->esquerda != NULL) {
    int leftPos = *pos;
    montaArquivo(arq, no->esquerda, pos);
    fseek(arq, currentPos * sizeof(NoExterno), SEEK_SET);
    fread(&novoExterno, sizeof(NoExterno), 1, arq);
    novoExterno.posEsquerda = leftPos;
    fseek(arq, currentPos * sizeof(NoExterno), SEEK_SET);
    fwrite(&novoExterno, sizeof(NoExterno), 1, arq);
  }

  if (no->direita != NULL) {
    int rightPos = *pos;
    montaArquivo(arq, no->direita, pos);
    fseek(arq, currentPos * sizeof(NoExterno), SEEK_SET);
    fread(&novoExterno, sizeof(NoExterno), 1, arq);
    novoExterno.posDireita = rightPos;
    fseek(arq, currentPos * sizeof(NoExterno), SEEK_SET);
    fwrite(&novoExterno, sizeof(NoExterno), 1, arq);
  }
}

Registro *buscaChave(FILE *arq, int chave, int posAtual) {
  if (posAtual == -1)
    return NULL;

  NoExterno noAtual;
  fseek(arq, posAtual * sizeof(NoExterno), SEEK_SET);
  fread(&noAtual, sizeof(NoExterno), 1, arq);

  if (noAtual.registro.chave == chave) {
    Registro *registroEncontrado = (Registro *)malloc(sizeof(Registro));
    if (registroEncontrado == NULL) {
      perror("Erro ao alocar memória");
      exit(EXIT_FAILURE);
    }
    *registroEncontrado = noAtual.registro;
    // Preencha outros campos de registroEncontrado conforme necessário
    return registroEncontrado;
  }

  if (chave < noAtual.registro.chave)
    return buscaChave(arq, chave, noAtual.posEsquerda);
  else
    return buscaChave(arq, chave, noAtual.posDireita);
}
