#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int		comparacoes = 0;

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
	No			*novo;
	Registro	registro;

	if (arvore->raiz == NULL)
	{
		novo = (No *)malloc(sizeof(No));
		if (novo == NULL)
		{
			fprintf(stderr, "Erro ao alocar memória\n");
			return ;
		}
		novo->direita = NULL;
		novo->esquerda = NULL;
		if (fread(&novo->registro, sizeof(Registro), 1, arq) != 1)
		{
			fprintf(stderr, "Erro ao ler do arquivo\n");
			free(novo);
			return ;
		}
		arvore->raiz = novo;
	}
	while (fread(&registro, sizeof(Registro), 1, arq) == 1)
	{
		inserir(&(arvore->raiz), registro);
	}
}

void	montaArquivo(FILE *arq, No *no, int pos)
{
	NoExterno	novo;

	if (no == NULL)
		return ;
	fseek(arq, 0, SEEK_SET); // vai pro inicio da arvore
	if (fread(&novo, sizeof(NoExterno), 1, arq) == 0)
	{ // arvore vazia
		novo.registro = no->registro;
		novo.posDireita = -1;
		novo.posEsquerda = -1;
		fseek(arq, 0, SEEK_SET); // vai pro inicio da arvore
		fwrite(&novo, sizeof(NoExterno), 1, arq);
	}
	else if (novo.registro.chave > no->registro.chave)
	{ // esquerda
		if (novo.posEsquerda == -1)
		{
			novo.posEsquerda = pos++;
			fseek(arq, -sizeof(NoExterno), SEEK_CUR);
			fwrite(&novo, sizeof(NoExterno), 1, arq);
			// reescrevendo o arquvo anterior para guardar a nova posição
			fseek(arq, novo.posEsquerda * sizeof(Registro), SEEK_SET);
			novo.registro = no->registro;
			novo.posEsquerda = -1;
			novo.posDireita = -1;
			fwrite(&novo, sizeof(No), 1, arq);
		}
		else
		{
			montaArquivo(arq, no, novo.posEsquerda);
		}
	}
	else if (novo.registro.chave < no->registro.chave)
	{
		if (novo.posDireita == -1)
		{
			novo.posDireita = pos++;
			fseek(arq, -sizeof(No), SEEK_CUR);
			fwrite(&novo, sizeof(No), 1, arq);
			// reescrevendo o arquvo anterior para guardar a nova posição
			fseek(arq, novo.posDireita * sizeof(NoExterno), SEEK_SET);
			novo.registro = no->registro;
			novo.posEsquerda = -1;
			novo.posDireita = -1;
			fwrite(&novo, sizeof(NoExterno), 1, arq);
		}
		else
		{
			montaArquivo(arq, no, novo.posDireita);
		}
	}
	montaArquivo(arq, no->esquerda, pos );
	montaArquivo(arq, no->direita, pos);
}

/*Registro	*buscaArquivo(FILE *arq, int chave, int pos)
{
	Registro novo;
	fseek(arq,pos*sizeof(Registro),SEEK_SET);
	if (fread(&novo, sizeof(Registro), 1, arq) == 0) return (NULL);
		// árvore vazia
	if(novo.chave == chave) return (&novo);
	if(novo.chave > chave){ //esquerda
		if(novo.posEsquerda == -1) return (NULL);
		else buscaArquivo(arq,chave,novo.posEsquerda);
	}
	else{ //Direita
		if(novo.posDireita == -1) return (NULL);
		else buscaArquivo(arq,chave,novo.posDireita);
	}
}*/
int	main(void)
{
	FILE *arq;

	// Tente abrir o arquivo "arvoreBin.bin"
	arq = fopen("arvoreBin.bin", "rb");
	if (arq == NULL)
	{
		printf("Arvore não encontrada!!\n");

		// Tente abrir o arquivo "aleatorio.bin"
		arq = fopen("crescente.bin", "rb");
		if (arq == NULL)
		{
			printf("Erro ao identificar o arquivo ''aleatorio.bin''- Não é possivel criar a árvore!!\n");
		}
		else
		{
			ArvoreBin *arvore = NULL;
			montaArvore(&arvore, arq);
			fclose(arq);

			if (arvore != NULL)
			{
				// Crie o arquivo "arvoreBin.bin"
				arq = fopen("arvoreBin.bin", "wb");
				if (arq != NULL)
				{
					int pos = 0;
					montaArquivo(arq, arvore->raiz, &pos);
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

	if (arq != NULL)
	{
		/*Registro *registro = buscaArquivo(arq, 100, 0);
		if (registro == NULL) {
			printf("Item não encontrado!!\n");
		} else {
			printf("Item encontrado!!\nChave: %ld\nDado1: %d\n",
				registro->chave, registro->dado1);
		}
		fclose(arq);
	} else {
		printf("Erro ao abrir o arquivo para busca.\n");*/
	}

	return (0);
}