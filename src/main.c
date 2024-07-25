#include "../include/acessoSequencial.h"
#include "../include/arvorebin.h"
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
FILE *arquivoArvore= NULL;
Registro	*registro;

  switch (metodo) {
  case 1:
    // imprimirArgumentos(argc, argv);
    if (situacao > 1) {
      printf("\033[1;31mSituação inválida para o método 1\033[0m\n");
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
  case 2:
    //imprimirArgumentos(argc, argv);
    if(arquivoArvore = fopen("arvorebin.bin","rb") == NULL) //Não existe árvore feita
    {
      // Abre o arquivo necessario para a criação da árvore
      if(situacao == 1) {arquivoArvore = fopen("crescente.bin","rb"); if(arquivoArvore == NULL){printf("Arquivo de dados nao encontrado!!!!\n"); return 0;}}
      if(situacao == 2) {arquivoArvore = fopen("decrescente.bin","rb"); if(arquivoArvore == NULL){printf("Arquivo de dados nao encontrado!!!!\n"); return 0;}}
      if(situacao == 3) {arquivoArvore = fopen("aleatorio.bin","rb"); if(arquivoArvore == NULL){printf("Arquivo de dados nao encontrado!!!!\n"); return 0;}}

      // Monta a árvore em memória interna
      ArvoreBin arvore = {NULL};
      montaArvore(&arvore, arquivoArvore);
			fclose(arquivoArvore);

      // Monta "arvorebin.bin" onde a arvore em memoria externa esta localizada
      if (arvore.raiz != NULL)
			{
				// Cria o arquivo "arvoreBin.bin"
				arquivoArvore = fopen("arvoreBin.bin", "wb");
				if (arquivoArvore != NULL)
				{
					pos = 0;
          
					montaArquivo(arquivoArvore, arvore.raiz, &pos);
					fclose(arquivoArvore);
					// Reabre o arquivo "arvoreBin.bin" para leitura
					arquivoArvore = fopen("arvoreBin.bin", "rb");
					if (arquivoArvore == NULL)
					{
						printf("Erro ao reabrir o arquivo ''arvoreBin.bin'' após criação.\n");
					}
				}
				else
				{
					printf("Erro ao criar o arquivo ''arvoreBin.bin''.\n");
				}
			}
			else
			{
				printf("Erro ao montar a árvore.\n");
			}
		}
    registro = buscaChave(arquivoArvore, chave, 0);
    imprimirRegistro(registro);
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
