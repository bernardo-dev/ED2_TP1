#include "../include/registro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  printf("ola\n");

  Registro reg;
  reg.chave = 10;
  reg.dado1 = 10000000;
  strcpy(reg.dado2, "testetesteteste");
  strcpy(reg.dado3, "testeeeee");

  printf("%d\n", reg.chave);
  return 0;
}
