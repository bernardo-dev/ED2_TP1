#ifndef B_ESTRELA
#define B_ESTRELA

#include "../include/registro.h"

#define M 2
#define MM 4
#define MM2 4

typedef enum {Interna, Externa} TipoIntExt;

typedef struct TipoPagina* Apontador;

typedef struct TipoPagina {
    TipoIntExt Pt;
    union {
        struct {
            int ni;
            Chave ri[MM];
            Apontador pi[MM + 1];
        } U0;
        struct {
            int ne;
            Registro re[MM2];
        } U1;
    } UU;
} TipoPagina;

// Protótipos das funções
void inicializa(TipoApontador arvore);
void insereNaPag(TipoApontador Ap, Registro Reg, TipoApontador ApDir);
void ins(Registro Reg, TipoApontador Ap, short *Cresceu, Registro *RegRetorno, TipoApontador *ApRetorno);
void insere(Registro Reg, TipoApontador *Ap);
bool Pesquisa(Registro *x, Apontador *ap);

#endif