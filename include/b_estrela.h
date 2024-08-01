#ifndef B_ESTRELA
#define B_ESTRELA

#include "../include/registro.h"
#include <stdbool.h>

#define M 2
#define MM 4
#define MM2 4

typedef int TipoChave;

typedef Registro TipoRegistro;

typedef enum { Interna, Externa } TipoIntExt;

typedef struct TipoPaginaB *TipoApontadorB;

typedef struct TipoPaginaB {
  TipoIntExt Pt;
  union {
    struct {
      int ni;
      TipoChave ri[MM]; // que a tipoChave está dentro da struct registro ou
                        // Chave = ri[MM];
      TipoApontadorB pi[MM + 1];
    } U0; // Interna
    struct {
      int ne;
      TipoRegistro re[MM2];
    } U1; // Externa
  } UU;
} TipoPaginaB;

// Inicializa a arvoreB*
void inicializaBEstrela(TipoApontadorB *arvore);

// Insere na pagina interna
void insereNaPagInt(TipoApontadorB Ap, TipoChave Chave, TipoApontadorB ApDir);

// Insere na pagina externa
void insereNaPagExt(TipoApontadorB Ap, TipoRegistro Reg);

// Insere recursivamente na arvore B*
void insereBEstrela(TipoRegistro Reg, TipoApontadorB *Ap);


// Pesquisa na arvore B*
bool pesquisaBEstrela(TipoRegistro *x, TipoApontadorB *Ap);

#endif