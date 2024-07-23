#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../include/b_estrela.h"
#include "../include/utils.h"

// FAZER INSERÇÃO PARA CRIAR A ARVORE B*

void inicializa(apontador arvore){
    arvore = NULL;
}

void insereNaPag(apontador ap, registro reg, apontador apDir){
    short naoAchouPosicao;
    int k;
    k = ap->n;
    naoAchouPosicao = (k > 0);
    while(naoAchouPosicao){
        if(reg.chave >= ap->r[k-1].chave){
            naoAchouPosicao = FALSE;
            break;
        }
        ap->r[k] = ap->r[k-1];
        ap->p[k+1] = ap->p[k];
        k--;
        if(k < 1)
            naoAchouPosicao = FALSE;
    }
    ap->r[k] = reg;
    ap->p[k+1] = apDir;
    ap->n++;
}

void ins(registro reg, apontador ap, short *cresceu, registro *regRetorno, apontador *apRetorno){
    long i = 1;
    long j;
    apontador apTemp;

    if(ap == NULL){
        *cresceu = TRUE;
        (*regRetorno) = reg;
        (*apRetorno) = NULL;
        return;
    }

    while(i < ap->n && reg.chave > ap->r[i-1].chave)
        i++;

    if(reg.chave == ap->r[i-1].chave){ // registro ja esta presente
        *cresceu = FALSE;
        return;
    }

    if(reg.chave < ap->r[i-1].chave)
        i--;

    ins(reg, ap->p[i], cresceu, regRetorno, apRetorno);

    if(!*cresceu)
        return;

    if(ap->n < MM){ // Pagina tem espaço
        insereNaPag(ap, *regRetorno, *apRetorno);
        *cresceu = FALSE;
        return;
    }

    // overflow: pagina tem que ser dividida
    apTemp = (apontador)malloc(sizeof(pagina));
    apTemp->n = 0;
    apTemp->p[0] = NULL;

    if(i < M+1){
        insereNaPag(apTemp, ap->r[MM-1], ap->p[MM]);
        ap->n--;
        insereNaPag(ap, *regRetorno, *apRetorno);
    }
    else
        insereNaPag(apTemp, *regRetorno, *apRetorno);

    for(j=M+2; j<=MM; j++)
        insereNaPag(apTemp, ap->r[j-1], ap->p[j]);

    ap->n = M;

    // Tentativa de mesclar com o irmão se houver  
    if (ap->p[M + 1]) { // checar se tem um irmão do lado direito  
        // Tem que fazer a lógica para combinar se o irmão tem mais de n registros  
        if (apTemp->n + ap->p[M + 1]->n <= MM) {  
            // Combinar os nós  
            for (j = 0; j < apTemp->n; j++) {  
                ap->p[M + 1]->r[ap->p[M + 1]->n++] = apTemp->r[j];  
            }  
            free(apTemp); // liberar a memória do nó temporário  
            return;  
        }  
    }  
    
    ap->p[M + 1] = apTemp; // não conseguiu combinar, somente adiciona  
    *regRetorno = ap->r[M];  
    *apRetorno = apTemp;  
}

void insere(registro reg, apontador *ap){
    short cresceu;
    registro regRetorno;
    pagina *apRetorno, *apTemp;

    ins(reg, *ap, &cresceu, &regRetorno, &apRetorno);

    if(cresceu){  // arvore cresce na altura pela raiz
        apTemp = (pagina*)malloc(sizeof(pagina));
        apTemp->n = 1;
        apTemp->r[0] = regRetorno;
        apTemp->p[1] = apRetorno;
        apTemp->p[0] = *ap;
        *ap = apTemp;
    }
}

bool Pesquisa(registro *x, apontador *ap, FILE *pArquivo){
    //variaveis para metricas
    Metrica metrica;
    metrica.comparacoes = 0;
    metrica.leituras = 0;
    metrica.escritas = 0;
    metrica.inicio = clock();

    int i;
    apontador pag;
    pag = *ap;

    if((*ap)->pt == interna){
        i = 1;
        while(i < pag->tipo.pagInt.ni && x->chave > pag->tipo.pagInt.ri[i-1]) 
            i++;
        if(x->chave < pag->tipo.pagInt.ri[i-1])
            Pesquisa(x, &pag->tipo.pagInt.pi[i-1]);
        else
            Pesquisa(x, &pag->tipo.pagInt.pi[i]);
        return;
    }

    i = 1;
    while(i < pag->tipo.pagFolha.ne && x->chave > pag->tipo.pagFolha.re[i-1].chave)
        i++;

    if(x->chave == pag->tipo.pagFolha.re[i-1].chave)
        *x = pag->tipo.pagFolha.re[i-1];
        return true;
    else
        return false;
}