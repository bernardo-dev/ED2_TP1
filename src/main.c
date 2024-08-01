#include "../include/acessoSequencial.h"
#include "../include/arvorebin.h"
#include "../include/utils.h"
#include "../include/b_estrela.h"
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
    return 0;
  }

  // Sempre declare as variaveis fora do switch

  // Variaveis para o metodo 1
  TipoIndice tabelaIndices[MAXTABELA];
  TipoItem item;
  int pos, cont;
  FILE *pArquivoIndices = NULL;
  char nomeArquivoIndices[100];
  int quantidadeIndices = 0;
  Metrica metrica;

  // Variáveis para o método 2
  FILE *pArquivoArvoreBinaria = NULL;
  Registro *registro;
  char nomeArquivoArvoreBinaria[100];

  switch (metodo) {
  case 1:
    // imprimirArgumentos(argc, argv);
    if (situacao > 1) {
      printf("\033[1;31mSituação inválida para o método 1 (Acesso Sequencial "
             "Indexado)\033[0m\n");
      break;
    }

    // Forma o nome do arquivo de indices
    sprintf(nomeArquivoIndices, "indices_%d.bin", quantidade);

    // Tenta abrir o arquivo de indices
    pArquivoIndices = fopen(nomeArquivoIndices, "rb");

    // Se não conseguir, cria o arquivo de indices
    if (pArquivoIndices == NULL) {
      metrica.leituras = 0;
      metrica.escritas = 0;
      metrica.comparacoes = 0;
      metrica.inicio = clock();
      pArquivoIndices = fopen(nomeArquivoIndices, "wb");

      cont = 0; // contador de itens
      pos = 0;  // posicao da pagina

      // gera a tabela de indice das paginas
      for (int i = 0; i < quantidade; i++) {
        metrica.leituras++;
        metrica.comparacoes++;
        if (fread(&item, sizeof(TipoItem), 1, pArquivoRegistros) == 1) {
          cont++;
          metrica.comparacoes++;
          if (cont % ITENSPAGINA == 1) {
            tabelaIndices[pos].chave = item.chave;
            tabelaIndices[pos].posicao = pos + 1;
            pos++;
          }
        }
      }
      // grava a tabela de indices no arquivo
      fwrite(tabelaIndices, sizeof(TipoIndice), pos, pArquivoIndices);
      metrica.escritas++;

      fclose(pArquivoIndices);

      metrica.fim = clock();

      // Imprime as metricas
      printf("\033[1;32mMetricas criacao arquivo de indices\033[0m\n");
      imprimirMetricas(metrica);
    } else {
      // Se conseguir abrir o arquivo de indices
      // Calcula a quantidade de indices
      fseek(pArquivoIndices, 0, SEEK_END);
      quantidadeIndices = ftell(pArquivoIndices) / sizeof(TipoIndice);
      pos = quantidadeIndices;
      fseek(pArquivoIndices, 0, SEEK_SET);

      // Le a tabela de indices
      fread(tabelaIndices, sizeof(TipoIndice), quantidadeIndices,
            pArquivoIndices);
      fclose(pArquivoIndices);
    }

    fflush(stdout);     // limpa o buffer de saida
    item.chave = chave; // chave a ser pesquisada

    // Pesquisa a chave na tabela de indices
    if (pesquisaAcessoSequencial(tabelaIndices, pos, &item,
                                 pArquivoRegistros)) {
      printf("\033[1;32mItem encontrado!\033[0m\n");
      imprimirRegistro(&item);
    } else {
      printf("\033[1;31mItem não encontrado!\033[0m\n");
    }
    break;
  case 2: {
    // imprimirArgumentos(argc, argv);
    // Forma o nome do arquivo da arvore binaria
    sprintf(nomeArquivoArvoreBinaria, "arvoreBinaria_%s_%d.bin",
            situacao == 1   ? "crescente"
            : situacao == 2 ? "decrescente"
                            : "aleatorio",
            quantidade);

    Metrica metrica = {0};
    metrica.inicio = clock();

    // Tenta abrir o arquivo da arvore binaria
    pArquivoArvoreBinaria = fopen(nomeArquivoArvoreBinaria, "rb");
    if (pArquivoArvoreBinaria == NULL) { // Não existe árvore feita
      // fclose(pArquivoArvoreBinaria);

      // Monta a árvore em memoria interna
      ArvoreBin arvore;
      // Inicializa a raiz da arvore
      arvore.raiz = NULL;
      clock_t inicioMontagem = clock();
      montaArvoreBinaria(&arvore, pArquivoRegistros, quantidade);
      clock_t fimMontagem = clock();
      double tempoMontagem = (double)(fimMontagem - inicioMontagem) / CLOCKS_PER_SEC;

      printf("\033[1;34mTempo para montar a árvore binária em memória interna: %f segundos\033[0m\n", tempoMontagem);

      // Se a arvore foi montada em memoria interna
      if (arvore.raiz != NULL) {
        // Cria o arquivo para a arvore binaria
        pArquivoArvoreBinaria = fopen(nomeArquivoArvoreBinaria, "wb");
        if (pArquivoArvoreBinaria != NULL) { // Se o arquivo foi criado
          pos = 0;

          montaArquivo(pArquivoArvoreBinaria, arvore.raiz, &pos, &metrica);

          fclose(pArquivoArvoreBinaria);

          // Reabre o arquivo para leitura
          pArquivoArvoreBinaria = fopen(nomeArquivoArvoreBinaria, "rb");
          if (pArquivoArvoreBinaria == NULL) {
            printf(
                "Erro ao reabrir o arquivo ''arvoreBin.bin'' após criação.\n");
                return 0;
          }
        } else {
          printf("\033[1;31mErro ao criar o arquivo %s\033[0m\n",
                 nomeArquivoArvoreBinaria);
                 return 0;
        }
      } else {
        printf("\033[1;31mErro ao montar a arvore binaria\033[0m\n");
        
        return 0;
      }
    }
    registro = buscaChave(pArquivoArvoreBinaria, chave, 0, &metrica);
    if(registro == NULL){
      printf("Registro não encontrado!!\n");
      return 0;
    }
    imprimirRegistro(registro);

    metrica.fim = clock();
    metrica.tempo = (double)(metrica.fim - metrica.inicio) / CLOCKS_PER_SEC;

    imprimirMetricas(metrica);
    break;
}
  case 3:
    imprimirArgumentos(argc, argv);
    // TODO: Implementar árvore B
    break;
  case 4:
    imprimirArgumentos(argc, argv);
    TipoApontadorB arvoreBEst;
    TipoRegistro registro;

    inicializaBEstrela(&arvoreBEst);

    for (int i = 0; i < quantidade; i++) {
      if (fread(&registro, sizeof(TipoRegistro), 1, pArquivoRegistros) != 1) {
        printf("Erro ao ler o registro\n");
        return 0;
      }
      insereBEstrela(registro, &arvoreBEst);
    }

    registro.chave = chave; // chave a ser pesquisada
    if (pesquisaBEstrela(&registro, &arvoreBEst)) {
      printf("Registro encontrado\n");
      imprimirRegistro(&registro);
    } else {
      printf("Registro não encontrado\n");
    }
    break;
  default:
    printf("Método não encontrado\n");
    break;
  }

  fecharArquivoRegistros(pArquivoRegistros);
  return 0;
}
