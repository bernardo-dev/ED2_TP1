#include "registro.h"

const int ITENSPAGINA = 512;

typedef struct {
  unsigned int posicao;
  int chave;
} TipoIndice;

typedef Registro TipoItem;
