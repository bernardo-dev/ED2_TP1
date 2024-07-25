#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int			comparacoes = 0;

void	inserir(No **no, Registro registro)
{
	No	*novo;

	if (*no == NULL)
	{
		novo = (No *)malloc(sizeof(No));
		if (novo == NULL)
		{
			fprintf(stderr, "Erro ao alocar memória\n");
			return ;
		}
		novo->direita = NULL;
		novo->esquerda = NULL;
		novo->registro = registro;
		*no = novo;
	}
	else
	{
		if (registro.chave < (*no)->registro.chave)
		{
			inserir(&((*no)->esquerda), registro);
		}
		else
		{
			inserir(&((*no)->direita), registro);
		}
	}
}

void	montaArvore(ArvoreBin *arvore, FILE *arq)
{
	Registro	registro;

	while (fread(&registro, sizeof(Registro), 1, arq) == 1)
	{
		inserir(&(arvore->raiz), registro);
	}
}

void	montaArquivo(FILE *arq, No *no, int *pos)
{
	NoExterno	novoExterno;
	int			currentPos;
	int			leftPos;
	int			rightPos;

	if (no == NULL)
		return ;
	novoExterno.registro = no->registro;
	novoExterno.posDireita = -1;
	novoExterno.posEsquerda = -1;
	fseek(arq, (*pos) * sizeof(NoExterno), SEEK_SET);
	fwrite(&novoExterno, sizeof(NoExterno), 1, arq);
	currentPos = *pos;
	(*pos)++;
	if (no->esquerda != NULL)
	{
		leftPos = *pos;
		montaArquivo(arq, no->esquerda, pos);
		fseek(arq, currentPos * sizeof(NoExterno), SEEK_SET);
		fread(&novoExterno, sizeof(NoExterno), 1, arq);
		novoExterno.posEsquerda = leftPos;
		fseek(arq, currentPos * sizeof(NoExterno), SEEK_SET);
		fwrite(&novoExterno, sizeof(NoExterno), 1, arq);
	}
	if (no->direita != NULL)
	{
		rightPos = *pos;
		montaArquivo(arq, no->direita, pos);
		fseek(arq, currentPos * sizeof(NoExterno), SEEK_SET);
		fread(&novoExterno, sizeof(NoExterno), 1, arq);
		novoExterno.posDireita = rightPos;
		fseek(arq, currentPos * sizeof(NoExterno), SEEK_SET);
		fwrite(&novoExterno, sizeof(NoExterno), 1, arq);
	}
}

Registro	*buscaChave(FILE *arq, int chave, int posAtual)
{
	NoExterno	noAtual;
	Registro	*registroEncontrado;

	if (posAtual == -1)
		return (NULL);
	fseek(arq, posAtual * sizeof(NoExterno), SEEK_SET);
	fread(&noAtual, sizeof(NoExterno), 1, arq);
	if (noAtual.registro.chave == chave)
	{
		registroEncontrado = (Registro *)malloc(sizeof(Registro));
		if (registroEncontrado == NULL)
		{
			perror("Erro ao alocar memória");
			exit(EXIT_FAILURE);
		}
		*registroEncontrado = noAtual.registro;
		// Preencha outros campos de registroEncontrado conforme necessário
		return (registroEncontrado);
	}
	if (chave < noAtual.registro.chave)
		return (buscaChave(arq, chave, noAtual.posEsquerda));
	else
		return (buscaChave(arq, chave, noAtual.posDireita));
}

int	main(void)
{
	FILE		*arq;
	int			pos;
	Registro	*registro;
	int			achou = 0, naoachou;

	// Tente abrir o arquivo "arvoreBin.bin"
	arq = fopen("arvoreBin.bin", "rb");
	if (arq == NULL)
	{
		printf("Arvore não encontrada!!\n");
		// Tente abrir o arquivo "aleatorio.bin"
		arq = fopen("aleatorio.bin", "rb");
		if (arq == NULL)
		{
			printf("Erro ao identificar o arquivo ''aleatorio.bin''- Não é possivel criar a árvore!!\n");
		}
		else
		{
			ArvoreBin arvore = {NULL}; // Inicialize a árvore com raiz nula
			montaArvore(&arvore, arq);
			fclose(arq);
			if (arvore.raiz != NULL)
			{
				// Crie o arquivo "arvoreBin.bin"
				arq = fopen("arvoreBin.bin", "wb");
				if (arq != NULL)
				{
					pos = 0;
					montaArquivo(arq, arvore.raiz, &pos);
					fclose(arq);
					// Reabra o arquivo "arvoreBin.bin" para leitura
					arq = fopen("arvoreBin.bin", "rb");
					if (arq == NULL)
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
	}
	achou = 0, naoachou = 0;
	for (int chave = 1; chave < 10001; chave++)
	{
		registro = buscaChave(arq, chave, 0);
		if (registro != NULL)
		{
			achou++;
			free(registro);
		}
		else
		{
			naoachou++;
		}
	}
	printf("Achou: %d\nNão achou: %d\n",achou,naoachou);
	return (0);
}
