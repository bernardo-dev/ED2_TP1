#include "../include/registro.h"
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

long int gerarLongIntAleatorio() {
  long int parteAlta = ((long int)rand() << 32) | rand();
  long int parteBaixa = ((long int)rand() << 32) | rand();
  // faz com que gere numeros negativos
  return (parteAlta << 1) ^ (parteBaixa >> 31);
}

void gerarDadoAleatorio(char *dado, int tamanho) {
  // do 32 ao 126 sao caracteres imprimiveis
  // 126 - 32 = 94
  // 94 + 1 para incluir o 126
  for (int i = 0; i < (tamanho - 1); i++) {
    int codigoAscii = (rand() % 95) + 32;
    dado[i] = codigoAscii;
  }
  dado[tamanho - 1] = '\0';
}

int main() {
  // Inicializa o gerador de numeros aleatorios
  srand(42);

  Registro reg;
  memset(&reg, 0, sizeof(Registro));
  int quantidade = 1000000;

  FILE *pArquivo = NULL;
  pArquivo = fopen("crescente.bin", "wb");

  for (int i = 1; i <= quantidade; i++) {
    reg.chave = i;
    reg.dado1 = gerarLongIntAleatorio();
    gerarDadoAleatorio(reg.dado2, 1001);
    gerarDadoAleatorio(reg.dado3, 5001);
    fwrite(&reg, sizeof(Registro), 1, pArquivo);
    // printf("Registro %d: chave(%-5d) dado1(%-21ld) dado2(%-5.5s) dado3(%-5.5s)\n",
    //        i, reg.chave, reg.dado1, reg.dado2, reg.dado3);
  }

  fclose(pArquivo);
  return 0;
}
