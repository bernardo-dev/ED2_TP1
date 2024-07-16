#include "../include/acessoSequencial.h"

bool pesquisaAcessoSequencial(TipoIndice tabelaIndices[], int tam,
                              TipoItem *pItem, FILE *pArquivo) {
  TipoItem pagina[ITENSPAGINA];
  int i, quantidadeItens;
  long deslocamento;

  // procura a página onde o item pode se encontrar
  i = 0;
  while (i < tam && tabelaIndices[i].chave <= pItem->chave) {
    i++;
  }

  // caso a chave desejada seja menor que a primeira chave.
  // o item nao existe no arquivo
  if (i == 0) {
    return false;
  } else {
    // a ultima pagina pode nao estar completa
    if (i < tam) {
      quantidadeItens = ITENSPAGINA;
    } else {
      fseek(pArquivo, 0, SEEK_END);
      quantidadeItens = (ftell(pArquivo) / sizeof(TipoItem)) % ITENSPAGINA;
    }

    // le a pagina desejada do arquivo
    deslocamento =
        (tabelaIndices[i - 1].posicao - 1) * ITENSPAGINA * sizeof(TipoItem);
    fseek(pArquivo, deslocamento, SEEK_END);
    fread(&pagina, sizeof(TipoItem), quantidadeItens, pArquivo);

    // pesquisa sequencial na pagina lida
    for (i = 0; i < quantidadeItens; i++) {
      if (pagina[i].chave == pItem->chave) {
        *pItem = pagina[i];
        return true;
      }
    }
    return false;
  }
}