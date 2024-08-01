#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../include/b_estrela.h"
#include "../include/utils.h"

void inicializaBEstrela(TipoApontadorB *arvore) {
  (*arvore) = (TipoPaginaB*)malloc(sizeof(TipoPaginaB));
  (*arvore)->Pt = Externa;
  (*arvore)->UU.U1.ne = 0;
}

void insereNaPagInt(TipoApontadorB Ap, TipoChave Chave, TipoApontadorB ApDir) {
  // variaveis para metricas
  Metrica metrica;
  metrica.comparacoes = 0;

  bool naoAchouPosicao;
  int k;

  k = Ap->UU.U0.ni;
  naoAchouPosicao = (k > 0);
  while (naoAchouPosicao) {
    metrica.comparacoes++;
    if (Chave >= Ap->UU.U0.ri[k - 1]) {
      naoAchouPosicao = false;
      break;
    }
    Ap->UU.U0.ri[k] = Ap->UU.U0.ri[k - 1];
    Ap->UU.U0.pi[k + 1] = Ap->UU.U0.pi[k];
    k--;
    if (k < 1)
      naoAchouPosicao = false;
  }
  Ap->UU.U0.ri[k] = Chave;
  Ap->UU.U0.pi[k + 1] = ApDir;
  Ap->UU.U0.ni++;
  return;
}

void insereNaPagExt(TipoApontadorB Ap, TipoRegistro Reg) {
  int k = Ap->UU.U1.ne;
  bool naoAchouPosicao = (k > 0);
  while (naoAchouPosicao) {
    if (Reg.chave >= Ap->UU.U1.re[k - 1].chave) {
      naoAchouPosicao = false;
      break;
    }
    Ap->UU.U1.re[k] = Ap->UU.U1.re[k - 1];
    k--;
    if (k < 1)
      naoAchouPosicao = false;
  }
  Ap->UU.U1.re[k] = Reg;
  Ap->UU.U1.ne++;
  return;
}

void insBEstrela(TipoRegistro Reg, TipoApontadorB Ap, bool *Cresceu,
                 TipoRegistro *RegRetorno, TipoApontadorB *ApRetorno) {
  long i = 1, j;
  TipoApontadorB ApTemp;

  if (Ap->Pt == Externa) {
    if (Ap->UU.U1.ne == MM2) {
      // Não tem espaço na página externa
      ApTemp = (TipoApontadorB)malloc(sizeof(TipoPaginaB));
      ApTemp->Pt = Externa;
      ApTemp->UU.U1.ne = 0;

      if (Reg.chave < Ap->UU.U1.re[M].chave) {
        insereNaPagExt(ApTemp, Ap->UU.U1.re[MM2 - 1]);
        Ap->UU.U1.ne--;
        insereNaPagExt(Ap, Reg);
      } else {
        insereNaPagExt(ApTemp, Reg);
      }

      // Dividir a pagina externa
      for (j = M + 2; j <= MM2; j++) {
        insereNaPagExt(ApTemp, Ap->UU.U1.re[j - 1]);
      }

      Ap->UU.U1.ne = M + 1;
      *RegRetorno = Ap->UU.U1.re[M];
      *ApRetorno = ApTemp;
      *Cresceu = true;
      return;
    } else {
      // Tem espaço na página externa
      insereNaPagExt(Ap, Reg);
      *Cresceu = false;
      return;
    }
  } else {
    while (i < Ap->UU.U0.ni && Reg.chave > Ap->UU.U0.ri[i - 1]) {
      i++;
    }

    if (Reg.chave == Ap->UU.U0.ri[i - 1]) { // Registro já existe
      *Cresceu = false;
      return;
    }
    
    if (Reg.chave < Ap->UU.U0.ri[i - 1])
      i--;

    insBEstrela(Reg, Ap->UU.U0.pi[i], Cresceu, RegRetorno, ApRetorno);

    if (!*Cresceu)
      return;

    if (Ap->UU.U0.ni < MM) {
      insereNaPagInt(Ap, RegRetorno->chave, *ApRetorno);
      *Cresceu = false;
      return;
    }

    ApTemp = (TipoApontadorB)malloc(sizeof(TipoPaginaB));
    ApTemp->Pt = Interna;
    ApTemp->UU.U0.ni = 0;
    ApTemp->UU.U0.pi[0] = NULL;

    if (i < M + 1) {
      insereNaPagInt(ApTemp, Ap->UU.U0.ri[MM - 1], Ap->UU.U0.pi[MM]);
      Ap->UU.U0.ni--;
      insereNaPagInt(Ap, RegRetorno->chave, *ApRetorno);
    } else {
      insereNaPagInt(ApTemp, RegRetorno->chave, *ApRetorno);
    }

    for (j = M + 2; j <= MM; j++)
      insereNaPagInt(ApTemp, Ap->UU.U0.ri[j - 1], Ap->UU.U0.pi[j]);

    Ap->UU.U0.ni = M;
    ApTemp->UU.U0.pi[0] = Ap->UU.U0.pi[M + 1];
    RegRetorno->chave = Ap->UU.U0.ri[M];
    *ApRetorno = ApTemp;
  }
}

void insereBEstrela(TipoRegistro Reg, TipoApontadorB *Ap) {
  bool Cresceu;
  TipoRegistro RegRetorno;
  TipoPaginaB *ApRetorno, *ApTemp;

  insBEstrela(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);

  if (Cresceu) { // arvore cresce na altura pela raiz
    ApTemp = (TipoPaginaB *)malloc(sizeof(TipoPaginaB));
    ApTemp->UU.U0.ni = 1;
    ApTemp->UU.U0.ri[0] = RegRetorno.chave;
    ApTemp->UU.U0.pi[1] = ApRetorno;
    ApTemp->UU.U0.pi[0] = *Ap;
    *Ap = ApTemp;
  }
}

bool pesquisaBEstrela(TipoRegistro *x, TipoApontadorB *Ap) {
  int i;
  TipoApontadorB Pag;
  Pag = *Ap;

  if ((*Ap)->Pt == Interna) {
    i = 1;
    while (i < Pag->UU.U0.ni && x->chave > Pag->UU.U0.ri[i - 1]) {
      i++;
    }

    if (x->chave < Pag->UU.U0.ri[i - 1])
      return pesquisaBEstrela(x, &Pag->UU.U0.pi[i - 1]);
    else
      return pesquisaBEstrela(x, &Pag->UU.U0.pi[i]);
  }

  i = 1;
  while (i < Pag->UU.U1.ne && x->chave > Pag->UU.U1.re[i - 1].chave) {
    i++;
  }

  if (x->chave == Pag->UU.U1.re[i - 1].chave) {
    *x = Pag->UU.U1.re[i - 1];
    return true;
  } else {
    return false;
  }
}