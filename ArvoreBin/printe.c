#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

void printaRegistros(const char *filename) {
    FILE *arq = fopen(filename, "rb");
    if (arq == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    Registro reg;
    while (fread(&reg, sizeof(Registro), 1, arq) == 1) {
        printf("Chave: %ld, PosEsquerda: %ld, PosDireita: %ld\n",
               reg.chave,0,0);
    }

    fclose(arq);
}

int main() {
    const char *filename = "aleatorio.bin"; 
    //const char *filename = "arvoreb";
    printaRegistros(filename);
    return 0;
}
