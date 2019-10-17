#include "../arvore.h"
typedef struct PilhaDeLista PilhaDeLista;
	

PilhaDeLista* PDL_cria();

//retorna 1 se ok
//retorna 0 se ja tem palavra em algum set da pilha
int PDL_inserePalavra(PilhaDeLista *p,char *s, TipoPrimitivo t);
//retorna 1 se achou a palavra
//retorna 0 se nao a palavra
int PDL_procuraPalavra(PilhaDeLista *p, char *s, TipoPrimitivo *t);




PilhaDeLista* PDL_insereEscopo(PilhaDeLista *l);
//PilhaDeLista* PDL_insereFim(PilhaDeLista *l);
void PDL_removeEscopo(PilhaDeLista *l);
	//remove do inicio

PilhaDeLista* PDL_insereFuncao(PilhaDeLista *p, char *id, TipoPrimitivo t);
PilhaDeLista* PDL_insereParametro(PilhaDeLista *p, char *id, TipoPrimitivo t);
TipoPrimitivo* PDL_recuperaFuncao(PilhaDeLista *p, char *s, int *tamanho);
void PDL_liberaParametro(TipoPrimitivo *p);

int PDL_vazia(PilhaDeLista *l);
void PDL_limpa(PilhaDeLista *l);
void PDL_libera(PilhaDeLista *l);
