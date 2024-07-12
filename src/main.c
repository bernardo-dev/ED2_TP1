// #include "../include/registro.h"
#include <stdio.h>
#include <stdlib.h>
// #include <string.h>

// pesquisa <método> <quantidade> <situação> <chave> [-P]
// onde:
//  <método> representa o método de pesquisa externa a ser executado,
// podendo ser um número inteiro de 1 a 4, de acordo com a ordem dos métodos
// mencionados;  <quantidade> representa a quantidade de registros do arquivo
// considerado;  <situação> representa a situação de ordem do arquivo,
// podendo ser 1 (arquivo ordenado ascendentemente), 2 (arquivo ordenado
// descendentemente) ou 3 (arquivo desordenado aleatoriamente);  <chave>
// representa a chave a ser pesquisada no arquivo considerado;  [-P]
// representa um argumento opcional que deve ser colocado quando se deseja que
// as chaves de pesquisa dos registros do arquivo considerado sejam apresentadas
// na tela.

// 1 acesso sequencial indexado
// 2 árvore binária de pesquisa adequada a memória externa
// 3 árvore B
// 4 árvore B*

void imprimirArgumentos(int argc, char *argv[]) {
  char metodos[4][100] = {
      "Acesso sequencial indexado",
      "Árvore binária adequada a memória externa",
      "Árvore B",
      "Árvore B*"};

  char situacao[3][100] = {"ascendentemente", "descendentemente",
                           "aleatoriamente"};

  if (argc < 5) {
    printf("\033[1;31mNúmero de argumentos inválido\033[0m\n");
    return;
  }

  printf("\033[1;34mMétodo: %s\033[0m\n", metodos[atoi(argv[1]) - 1]);
  printf("\033[1;34mQuantidade: %d\033[0m\n", atoi(argv[2]));
  printf("\033[1;34mSituação: %s\033[0m\n", situacao[atoi(argv[3]) - 1]);
  printf("\033[1;34mChave: %d\033[0m\n", atoi(argv[4]));
  if (argc == 6) {
    printf("\033[1;34mArgumento opcional: %s\033[0m\n", argv[5]);
  }
}

int main(int argc, char *argv[]) {
  int metodo = atoi(argv[1]);
  switch (metodo) {
  case 1:
    imprimirArgumentos(argc, argv);
    // Teste esse caso com o comando:
    // ./build/pesquisa 1 1000000 1 500000 -P
    // TODO: Implementar acesso sequencial indexado
    break;
  case 2:
    imprimirArgumentos(argc, argv);
    // TODO: Implementar árvore binária de pesquisa adequada a memória externa
    break;
  case 3:
    imprimirArgumentos(argc, argv);
    // TODO: Implementar árvore B
    break;
  case 4:
    imprimirArgumentos(argc, argv);
    // TODO: Implementar árvore B*
    break;
  default:
    printf("Método não encontrado\n");
    break;
  }
  return 0;
}
