#include "../include/acessoSequencial.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  // Verifica se a quantidade de argumentos é válida
  if (argc < 5) {
    printf("\033[1;31mNúmero de argumentos inválido!\033[0m\n");
    return 0;
  }

  int metodo = atoi(argv[1]);
  int quantidade = atoi(argv[2]);
  int situacao = atoi(argv[3]);
  int chave = atoi(argv[4]);

  FILE *pArquivoRegistros = NULL;
  pArquivoRegistros = abrirArquivoRegistros(situacao);
  if (pArquivoRegistros == NULL) {
    printf("\033[1;31mErro ao abrir o arquivo\033[0m\n");
  }

  // Sempre declare as variaveis fora do switch

  // Variaveis para o metodo 1
  TipoIndice tabelaIndices[MAXTABELA];
  TipoItem item;
  int pos, cont;

  switch (metodo) {
  case 1:
    imprimirArgumentos(argc, argv);

    cont = 0; // contador de itens
    pos = 0;  // posicao da pagina

    // gera a tabela de indice das paginas
    for (int i = 0; i < quantidade; i++) {
      if (fread(&item, sizeof(TipoItem), 1, pArquivoRegistros) == 1) {
        cont++;
        if (cont % ITENSPAGINA == 1) {
          tabelaIndices[pos].chave = item.chave;
          tabelaIndices[pos].posicao = pos + 1;
          pos++;
        }
      }
    }

    fflush(stdout);     // limpa o buffer de saida
    item.chave = chave; // chave a ser pesquisada

    if (pesquisaAcessoSequencial(tabelaIndices, pos, &item,
                                 pArquivoRegistros)) {
      printf("\033[1;32mItem encontrado!\033[0m\n");
    } else {
      printf("\033[1;31mItem não encontrado!\033[0m\n");
    }
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

  fecharArquivoRegistros(pArquivoRegistros);
  return 0;
}
