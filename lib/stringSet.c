#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stringSet.h"
//precisamos do tipo primitivo que esta na arvore .h

//TODO remover isso daqui e colocar num lugar comum
//tem outra na arvore (?)
#define MAX_ID 81

typedef struct no{
	char s[MAX_ID];
	TipoPrimitivo t;
	struct no* prox;
}No;

struct stringSet {
	No* prim;
	No* ult;
};


StringSet* deFato_insereIni(StringSet *ss,char *s, TipoPrimitivo t);
StringSet* deFato_insereFim(StringSet *ss,char *s, TipoPrimitivo t);

StringSet* STS_cria(){
	StringSet *ss = (StringSet*) malloc(sizeof(StringSet)); 
	if(ss==NULL){
		fprintf(stderr, "Erro ao alocar StringSet\n");
		exit(1);
	}
	
	ss->prim = NULL;
	ss->ult = NULL;
	return ss;
}

int STS_insereIni(StringSet *ss, char *s, TipoPrimitivo t){
	if(STS_procura(ss,s, NULL)) return 0;
	deFato_insereIni(ss, s, t);
	return 1;
}
StringSet* deFato_insereIni(StringSet *ss,char *s, TipoPrimitivo t){
	No* n = (No*) malloc(sizeof(No));
	if(n==NULL){
		fprintf(stderr, "Erro ao alocar No da StringSet\n");
		exit(1);
	}
	strcpy(n->s, s);
	n->prox = ss->prim;
	n->t = t;
	ss->prim = n;
	if(ss->ult == NULL){
		ss->ult = ss->prim;
	}
	return ss;	
}
int STS_insereFim(StringSet *ss,char *s, TipoPrimitivo t){
	if(STS_procura(ss,s, NULL)) return 0;
	deFato_insereFim(ss, s, t);
	return 1;
}
StringSet* deFato_insereFim(StringSet *ss,char *s, TipoPrimitivo t){
	No* n = (No*) malloc(sizeof(No));
	if(n==NULL){
		fprintf(stderr, "Erro ao alocar No da StringSet\n");
		exit(1);
	}
	strcpy(n->s, s);
	n->t = t;
	n->prox = NULL;
	if(ss->ult != NULL){
		ss->ult->prox = n;
	}
	ss->ult = n;
	if(ss->prim == NULL){
		ss->prim = ss->ult;
	}
	return ss;	
}



int STS_procura(StringSet *ss, char *s, TipoPrimitivo *t){
	No *n;
	for(n=ss->prim;n!=NULL; n = n->prox){
		if(strcmp(n->s, s)==0){
			if(t!=NULL)*t = n->t;
			return 1;
		}
	}
	return 0;
}

// TODO melhorar a eficiencia disso
TipoPrimitivo* STS_recuperaFuncao(StringSet *ss, char *s, int *tamanho){
	No *n;
	TipoPrimitivo *p;
	int i;
	if(tamanho!=NULL){
		*tamanho = 0;
	}
	//Se a string recebida for NULL ele retorna esta lista como funcao
	//isso vai servir para o caso do return no qual preciso dos dados da ultima funcao incluida na tabela
	//
	//Se estou procurando uma funcao especifica e nao e esta a funcao que estou procurando, entao retorna NULL par indicar que nao e esta
	//Se estou procurando a ultima funcao incluida entao vou passar NULL para esta funcao, e ela vai me retornar a ultima funcao incluida

	fprintf(stderr, "SS: comparando funcao %s procurando %s\n", ss->prim->s, s);
	fprintf(stderr, "SS: comparando funcao %s procurando %s\n", ss->prim->prox->s, s);
	fprintf(stderr, "SS: comparando funcao %s procurando %s\n", ss->prim->prox->prox->s, s);
	if(s!=NULL && strcmp(ss->prim->s, s)!=0){
		printf("Funcao nao %s encontrada\n", s);
		return NULL;
	}
	for(n=ss->prim;n!=NULL; n = n->prox){
		i++;
	}
	if(tamanho!=NULL) *tamanho = i;
	
	p = (TipoPrimitivo*) malloc(i*sizeof(TipoPrimitivo));
	if(p==NULL){
		fprintf(stderr, "Erro ao alocar vetor de TipoPrimitivo\n");
		exit(1);
	}
	i = 0;
	for(n=ss->prim;n!=NULL; n = n->prox){
		p[i]= n->t;
		i++;
	}
	return p;
}

void STS_liberaParametro(TipoPrimitivo *p){
	free(p);
}
/*
void STS_recupera(StringSet *ss){
	char* data;
	No* prim;
	if(l->prim==NULL){
		fprintf(stderr, "Erro, StringSet vazia\n");
		return NULL;
	}
	data = l->prim->data; 
	prim = l->prim;
	l->prim = l->prim->prox;
	free(prim);
	return data;
}

*/

void STS_remove(StringSet *ss){
	No* prim;
	if(ss->prim==NULL){
		fprintf(stderr, "Erro, StringSet vazia\n");
	}
	prim = ss->prim;
	ss->prim = ss->prim->prox;
	free(prim);
}

int STS_vazia(StringSet *ss){
	return ss->prim == NULL;
}

void STS_limpa(StringSet *ss){
	No *c,*a;
	c = ss->prim;
	while(c!=NULL){
		a = c;
		c = c->prox;
		free(a);
	}
	ss->prim = NULL;
	ss->ult = NULL;
}

void STS_libera(StringSet *ss){
	STS_limpa(ss);
	free(ss);
}
