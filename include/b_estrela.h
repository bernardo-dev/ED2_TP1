#ifndef B_ESTRELA
#define B_ESTRELA

#include "../include/registro.h"

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
void inicializa(TipoApontadorB arvore);

// Insere na pagina interna
void insereNaPagInt(TipoApontadorB Ap, TipoRegistro Reg, TipoApontadorB ApDir);

// Insere na pagina externa
void insereNaPagExt(TipoApontadorB Ap, TipoRegistro Reg);

void ins(Registro Reg, TipoApontadorB Ap, short *Cresceu,
         TipoRegistro *RegRetorno, TipoApontadorB *ApRetorno);
void insere(TipoRegistro Reg, TipoApontadorB *Ap);
bool Pesquisa(TipoRegistro *x, TipoApontadorB *ap);

#endif