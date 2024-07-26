#ifndef B_ESTRELA
#define B_ESTRELA

#include "../include/registro.h"

#define MM 4
#define MM2 2

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
            TipoRegistro re[MM2];
        } U1;
    } UU;
} TipoPagina;

// Protótipos das funções
void Pesquisa(TipoRegistro *x, Apontador *ap);

#endif