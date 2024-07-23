#ifndef B_ESTRELA
#define B_ESTRELA

#define MM 4
#define MM2 2

typedef int chaves;
typedef struct registro {
    chaves chave;
    long int dado1;
    char dado2[1000];
    char dado3[5000];
} registro;

typedef enum {interna, externa} TipoIntExt;

typedef struct pagina* apontador;

typedef struct pagina {
    TipoIntExt pt;
    union {
        struct {
            int ni;
            chaves ri[MM];
            apontador pi[MM + 1];
        } pagInt;
        struct {
            int ne;
            registro re[MM2];
        } pagFolha;
    } tipo;
} pagina;

// Protótipos das funções
bool Pesquisa(registro *x, apontador *ap);

#endif