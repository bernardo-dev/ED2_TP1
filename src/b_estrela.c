#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../include/b_estrela.h"
#include "../include/utils.h"
#include "../include/registro.h"

void inicializa(TipoApontador arvore){
    arvore = NULL;
}

void insereNaPag(TipoApontador Ap, Registro Reg, TipoApontador ApDir){
    //variaveis para metricas
    Metrica metrica;
    metrica.comparacoes = 0;

    short naoAchouPosicao;
    int k;

    if((*Ap)->Pt == Interna){ //Inserção em página interna
        k = Ap->UU.U0.ni;
        naoAchouPosicao = (k > 0);
        while(naoAchouPosicao){
            metrica.comparacoes++;
            if(Reg.chave >= Ap->UU.U0.ri[k-1]){
                naoAchouPosicao = FALSE;
                break;
            }
            Ap->UU.U0.ri[k] = Ap->UU.U0.ri[k-1];
            Ap->UU.U0.pi[k+1] = Ap->UU.U0.pi[k];
            k--;
            if(k < 1)
                naoAchouPosicao = FALSE;
        }
        Ap->UU.U0.ri[k] = Reg.chave;
        Ap->UU.U0.pi[k+1] = ApDir;
        Ap->UU.U0.ni++;
        return;
    } else if ((*Ap)->Pt == Externa){ //Inserção em pagina folha/externa
        k = Ap->UU.U1.ne;
        naoAchouPosicao = (k > 0);
        while(naoAchouPosicao){
            metrica.comparacoes++;
            if(Reg.chave >= Ap->UU.U1.re[k-1].chave){
                naoAchouPosicao = FALSE;
                break;
            }
            Ap->UU.U1.re[k] = Ap->UU.U1.re[k-1];
            k--;
            if(k < 1)
                naoAchouPosicao = FALSE;
        }
        Ap->UU.U1.re[k] = Reg;
        Ap->UU.U1.ne++;
        return;
    }
}

void ins(TipoRegistro Reg, TipoApontadorB Ap, short *Cresceu, TipoRegistro *RegRetorno, TipoApontadorB *ApRetorno) {
    //variaveis para metricas
    Metrica metrica;
    metrica.comparacoes = 0;

    long i = 1, j;
    TipoApontadorB ApTemp;

    if (Ap->Pt == Externa) {
        if (Ap->UU.U1.ne == MM2) {
            // Não tem espaço na página externa
            ApTemp = (TipoApontadorB) malloc(sizeof(TipoPaginaB));
            ApTemp->Pt = Externa;
            ApTemp->UU.U1.ne = 0;

            if (Reg.chave < Ap->UU.U1.re[M].chave) {
                insereNaPag(ApTemp, Ap->UU.U1.re[MM2-1]);
                Ap->UU.U1.ne--;
                insereNaPag(Ap, Reg);
            } else {
                insereNaPag(ApTemp, Reg);
            }

            for (j = M + 2; j <= MM2; j++) {
                insereNaPag(ApTemp, Ap->UU.U1.re[j-1]);
            }

            Ap->UU.U1.ne = M + 1;
            *RegRetorno = Ap->UU.U1.re[M];
            *ApRetorno = ApTemp;
            *Cresceu = true;
            return;
        } else {
            // Tem espaço na página externa
            insereNaPag(Ap, Reg);
            *Cresceu = false;
            return;
        }
    } else {
        // Busca binária para encontrar a posição de inserção
        long esq = 0, dir = Ap->UU.U0.ni - 1, meio;
        while (esq <= dir) {
            meio = (esq + dir) / 2;
            metrica.comparacoes++;
            if (Reg.chave == Ap->UU.U0.ri[meio]) {
                printf("Erro: Registro já está presente");
                *Cresceu = false;
                return;
            } 
            metrica.comparacoes++;
            else if (Reg.chave < Ap->UU.U0.ri[meio]) {
                dir = meio - 1;
            } else {
                esq = meio + 1;
            }
        }
        i = esq;

        ins(Reg, Ap->UU.U0.pi[i], Cresceu, RegRetorno, ApRetorno);

        if (!*Cresceu) return;

        if (Ap->UU.U0.ni < MM) {
            insereNaPag(Ap, RegRetorno->chave, *ApRetorno);
            *Cresceu = false;
            return;
        }

        ApTemp = (TipoApontadorB) malloc(sizeof(TipoPaginaB));
        ApTemp->Pt = Interna;
        ApTemp->UU.U0.ni = 0;
        ApTemp->UU.U0.pi[0] = NULL;

        if (i < M + 1) {
            insereNaPag(ApTemp, Ap->UU.U0.ri[MM-1], Ap->UU.U0.pi[MM]);
            Ap->UU.U0.ni--;
            insereNaPag(Ap, RegRetorno->chave, *ApRetorno);
        } else {
            insereNaPag(ApTemp, RegRetorno->chave, *ApRetorno);
        }

        for (j = M + 2; j <= MM; j++) {
            insereNaPag(ApTemp, Ap->UU.U0.ri[j-1], Ap->UU.U0.pi[j]);
        }

        Ap->UU.U0.ni = M;
        ApTemp->UU.U0.pi[0] = Ap->UU.U0.pi[M + 1];
        RegRetorno->chave = Ap->UU.U0.ri[M];
        *ApRetorno = ApTemp;
    }
}

// void ins(Registro Reg, TipoApontador Ap, short *Cresceu, Registro *RegRetorno, TipoApontador *ApRetorno){
//     //variaveis para metricas
//     Metrica metrica;
//     metrica.comparacoes = 0;
    
//     long i = 1;
//     long j;
//     TipoApontador ApTemp;

//     if(Ap == NULL){
//         *Cresceu = TRUE;
//         (*RegRetorno) = Reg;
//         (*ApRetorno) = NULL;
//         return;
//     }

//     while(i < Ap->n && Reg.chave > Ap->r[i-1].chave){
//         metrica.comparacoes++;
//         i++;
//     }

//     metrica.comparacoes++;
//     if(Reg.chave == Ap->UU.U0.ri[i-1].chave){ // Registro ja esta presente
//         *Cresceu = FALSE;
//         return;
//     }

//     metrica.comparacoes++;
//     if(Reg.chave < Ap->r[i-1].chave)
//         i--;

//     ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno);

//     if(!*Cresceu)
//         return;

//     if(Ap->n < MM){ // Pagina tem espaço
//         insereNaPag(Ap, *RegRetorno, *ApRetorno);
//         *Cresceu = FALSE;
//         return;
//     }

//     // overflow: Pagina tem que ser dividida
//     ApTemp = (TipoApontador)malloc(sizeof(TipoPagina));
//     ApTemp->n = 0;
//     ApTemp->p[0] = NULL;

//     if(i < M+1){
//         insereNaPag(ApTemp, Ap->r[MM-1], Ap->p[MM]);
//         Ap->n--;
//         insereNaPag(Ap, *RegRetorno, *ApRetorno);
//     }
//     else
//         insereNaPag(ApTemp, *RegRetorno, *ApRetorno);

//     for(j=M+2; j<=MM; j++)
//         insereNaPag(ApTemp, Ap->r[j-1], Ap->p[j]);

//     Ap->n = M;

//     // Tentativa de mesclar com o irmão se houver  
//     if (Ap->p[M + 1]) { // checar se tem um irmão do lado direito  
//         // Tem que fazer a lógica para combinar se o irmão tem mais de n Registros  
//         if (ApTemp->n + Ap->p[M + 1]->n <= MM) {  
//             // Combinar os nós  
//             for (j = 0; j < ApTemp->n; j++) {  
//                 Ap->p[M + 1]->r[Ap->p[M + 1]->n++] = ApTemp->r[j];  
//             }  
//             free(ApTemp); // liberar a memória do nó temporário  
//             return;  
//         }  
//     }  
    
//     Ap->p[M + 1] = ApTemp; // não conseguiu combinar, somente adiciona  
//     *RegRetorno = Ap->r[M];  
//     *ApRetorno = ApTemp;  
// }

void insere(Registro Reg, TipoApontador *Ap){
    short Cresceu;
    Registro RegRetorno;
    TipoPagina *ApRetorno, *ApTemp;

    ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);

    if(Cresceu){  // arvore cresce na altura pela raiz
        ApTemp = (TipoPagina*)malloc(sizeof(TipoPagina));
        ApTemp->n = 1;
        ApTemp->r[0] = RegRetorno;
        ApTemp->p[1] = ApRetorno;
        ApTemp->p[0] = *Ap;
        *Ap = ApTemp;
    }
}

bool Pesquisa(Registro *x, TipoApontador *Ap, FILE *pArquivo){
    //variaveis para metricas
    Metrica metrica;
    metrica.comparacoes = 0;
    metrica.leituras = 0;
    metrica.inicio = clock();

    int i;
    TipoApontador Pag;
    Pag = *Ap;

    if((*Ap)->Pt == Interna){
        i = 1;
        while(i < Pag->UU.U0.ni && x->chave > Pag->UU.U0.ri[i-1]){
            metrica.comparacoes++;
            i++;
        }   

        metrica.comparacoes++;
        if(x->chave < Pag->UU.U0.ri[i-1])
            Pesquisa(x, &Pag->UU.U0.pi[i-1]);
        else
            Pesquisa(x, &Pag->UU.U0.pi[i]);
        return;
    }

    i = 1;
    while(i < Pag->UU.U1.ne && x->chave > Pag->UU.U1.re[i-1].chave){
        metrica.comparacoes++;
        i++;
    }

    metrica.comparacoes++;
    if(x->chave == Pag->UU.U1.re[i-1].chave){
        *x = Pag->UU.U1.re[i-1];
        metrica.fim = clock();
        printf("\033[1;34mMetricas Pesquisa Arvore b*\033[0m\n");
        imprimirMetricas(metrica);
        return true;
    } else{
        metrica.fim = clock();
        printf("\033[1;34mMetricas Pesquisa Arvore b*\033[0m\n");
        imprimirMetricas(metrica);
        return false;
    }
}