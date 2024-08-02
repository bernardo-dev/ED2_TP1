#include <stdlib.h>
#include "../include/arvoreB.h"
#include "../include/utils.h"
#include "../include/registro.h"

void inicializaArvoreB(TipoApontador *Arvore)
{
    *Arvore = NULL;
}

int pesquisaArvoreB(Registro *x, TipoApontador Ap, Metrica *metrica)
{
    long i = 1;

    if (Ap == NULL)
    {
        printf("Registro nao encontrado na arvore\n");
        return 0;
    }

    metrica->comparacoes++;
    while (i < Ap->n && x->chave > Ap->r[i-1].chave)
    { 
        metrica->comparacoes++;
        i++;
    }

    if (x->chave == Ap->r[i-1].chave)
    { 
        metrica->comparacoes++;
        *x = Ap->r[i-1];
        // printf("Chave encontrada!\n");
        return 1;
    }
    if (x->chave < Ap->r[i-1].chave)
    {
        metrica->comparacoes++;
        return pesquisaArvoreB(x, Ap->p[i - 1], metrica);
    }
    else
    {
        metrica->comparacoes++;
        return pesquisaArvoreB(x, Ap->p[i], metrica);
    }
    return 0; // Este retorno nunca será alcançado, mas é necessário para evitar avisos do compilador
}

void insereNaPaginaArvoreB(TipoApontador Ap, Registro Reg, TipoApontador ApDir, Metrica *metrica)
{
    int k;
    int NaoAchouPosicao;

    k = Ap->n;
    NaoAchouPosicao = (k > 0);
    while (NaoAchouPosicao)
    {
        if (Reg.chave >= Ap->r[k - 1].chave)
        {
            NaoAchouPosicao = 0;
            break;
        }
        Ap->r[k] = Ap->r[k - 1];
        Ap->p[k + 1] = Ap->p[k];
        k--;
        if (k < 1)
            NaoAchouPosicao = 0;
    }
    Ap->r[k] = Reg;
    Ap->p[k + 1] = ApDir;
    Ap->n++;
    metrica->escritas++;
}

void insArvoreB(Registro Reg, TipoApontador Ap, short *Cresceu, Registro *RegRetorno, TipoApontador *ApRetorno, Metrica *metrica)
{
    TipoApontador ApTemp;
    long i = 1, j;

    if (Ap == NULL)
    {
        *Cresceu = 1;
        (*RegRetorno) = Reg;
        (*ApRetorno) = NULL;
        metrica->escritas++;

        return;
    }

    while (i < Ap->n && Reg.chave > Ap->r[i - 1].chave)
    {
        i++;
        metrica->comparacoes++;
    }

    if (i <= Ap->n && Reg.chave == Ap->r[i - 1].chave)
    {   
        *Cresceu = 0;
        return;
    }

    if (i <= Ap->n && Reg.chave < Ap->r[i - 1].chave)
    {
        i--;
    }
    insArvoreB(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno, metrica);
    if (!*Cresceu)
        return;
    if (Ap->n < MM1)
    {
        //Pagina tem espaco 
        insereNaPaginaArvoreB(Ap, *RegRetorno, *ApRetorno, metrica);
        *Cresceu = 0;
        return;
    }
    // Overflow: Pagina tem que ser dividida 
    ApTemp = (TipoApontador) malloc(sizeof(TipoPagina));
    ApTemp->n = 0;
    ApTemp->p[0] = NULL;
    if (i <= M1 + 1)
    {
        insereNaPaginaArvoreB(ApTemp, Ap->r[MM1 - 1], Ap->p[MM1], metrica);
        Ap->n--;
        insereNaPaginaArvoreB(Ap, *RegRetorno, *ApRetorno, metrica);
    }
    else
        insereNaPaginaArvoreB(ApTemp, *RegRetorno, *ApRetorno, metrica);
    for (j = M1 + 2; j <= MM1; j++)
        insereNaPaginaArvoreB(ApTemp, Ap->r[j - 1], Ap->p[j], metrica);
    Ap->n = M1;
    ApTemp->p[0] = Ap->p[M1 + 1];
    *RegRetorno = Ap->r[M1];
    *ApRetorno = ApTemp;
    metrica->escritas++;
}

void insereArvoreB(Registro Reg, TipoApontador *Ap, Metrica *metrica)
{
    short Cresceu;
    Registro RegRetorno;
    TipoPagina *ApRetorno, *ApTemp;

    insArvoreB(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno, metrica);

    if (Cresceu)   // Arvore cresce na altura pela raiz 
    {
        ApTemp = (TipoPagina*)malloc(sizeof(TipoPagina));
        ApTemp->n = 1;
        ApTemp->r[0] = RegRetorno;
        ApTemp->p[1] = ApRetorno;
        ApTemp->p[0] = (*Ap);
        *Ap = ApTemp;
        metrica->escritas++;
    }
}