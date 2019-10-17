#include <stdio.h>
#include <stdlib.h>
#include "pilhaDeLista.h"
#include "stringSet.h"

typedef struct no{
	StringSet* ss;
	struct no* prox;
}No;

struct PilhaDeLista {
	No* prim;
	No* ult;
};

PilhaDeLista* PDL_cria(){
	PilhaDeLista *p = (PilhaDeLista*) malloc(sizeof(PilhaDeLista)); 
	if(p==NULL){
		fprintf(stderr, "Erro ao alocar PilhaDeLista\n");
		exit(1);
	}
	
	p->prim = NULL;
	p->ult = NULL;
	return p;
}

int PDL_inserePalavra(PilhaDeLista *p,char *s, TipoPrimitivo t){
	if(!STS_insereIni(p->prim->ss, s, t)){
		return 0;
	}
	return 1;
}

int PDL_procuraPalavra(PilhaDeLista *p, char *s, TipoPrimitivo *t){
	No *n;
	for(n = p->prim; n!=NULL; n = n->prox){
		if(STS_procura(n->ss, s, t)){
			return 1;
		}
	}
	return 0;
}

PilhaDeLista* PDL_insereEscopo(PilhaDeLista *p){
	No* n = (No*) malloc(sizeof(No));
	if(n==NULL){
		fprintf(stderr, "Erro ao alocar No da PilhaDeLista\n");
		exit(1);
	}
	n->ss = STS_cria();
	n->prox = p->prim;
	p->prim = n;
	if(p->ult == NULL){
		p->ult = p->prim;
	}
	return p;	
}

PilhaDeLista* PDL_insereFuncao(PilhaDeLista *p, char *id, TipoPrimitivo t){
	PDL_insereEscopo(p);
	//PDL_inserePalavra(p, id, t);

	STS_insereFim(p->prim->ss, id, t);


	return p;
}

PilhaDeLista* PDL_insereParametro(PilhaDeLista *p, char *id, TipoPrimitivo t){
	//PDL_inserePalavra(p, id, t);

	STS_insereFim(p->prim->ss, id, t);

	return p;
}


TipoPrimitivo* PDL_recuperaFuncao(PilhaDeLista *p, char *s, int *tamanho){
	No *n;
	TipoPrimitivo *tp;
	for(n = p->prim; n!=NULL; n = n->prox){
		tp = STS_recuperaFuncao(n->ss, s, tamanho);
		if(tp != NULL) return tp;
	}
	return NULL;
}

void PDL_liberaParametro(TipoPrimitivo *p){
	STS_liberaParametro(p);
}

PilhaDeLista* PDL_insereFim(PilhaDeLista *p){
	No* n = (No*) malloc(sizeof(No));
	if(n==NULL){
		fprintf(stderr, "Erro ao alocar No da PilhaDeLista\n");
		exit(1);
	}
	n->ss = STS_cria();
	n->prox = NULL;
	if(p->ult != NULL){
		p->ult->prox = n;
	}
	p->ult = n;
	if(p->prim == NULL){
		p->prim = p->ult;
	}
	return p;	
}

void PDL_removeEscopo(PilhaDeLista *p){
	No* prim;
	if(p->prim==NULL){
		fprintf(stderr, "Erro, PilhaDeLista vazia\n");
	}
	prim = p->prim;
	p->prim = p->prim->prox;
	free(prim);
}

int PDL_vazia(PilhaDeLista *p){
	return p->prim == NULL;
}

void PDL_limpa(PilhaDeLista *p){
	No *c,*a;
	c = p->prim;
	while(c!=NULL){
		a = c;
		c = c->prox;
		free(a);
	}
	p->prim = NULL;
	p->ult = NULL;
}

void PDL_libera(PilhaDeLista *p){
	PDL_limpa(p);
	free(p);
}
