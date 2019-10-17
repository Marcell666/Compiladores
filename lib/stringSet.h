#include "../arvore.h"
typedef struct stringSet StringSet;
	

StringSet* STS_cria();

//retorna 1 se ok
//retorna 0 se erro - se ja tem a palavra no set
int STS_insereIni(StringSet *ss,char *s, TipoPrimitivo t);
int STS_insereFim(StringSet *ss,char *s, TipoPrimitivo t);
void STS_remove(StringSet *ss);

//retorna a distancia da palavra procurada para o escopo no qual ela se encontra
//retorna 0 se nao encontrar
//retorna 1 se estiver no escopo atual
//retorna 2 ou mais se estiver um escopo acima ou mais


//retorna 1 se a string está na lista
//retorna 0 se nao esta
int STS_procura(StringSet *ss, char *s, TipoPrimitivo *t);

//retorna um vetor tipoPrimitivo do tamanho exato que corresponde ao tipos da funcao e dos parametros nesta ordem

//isso vai ser util para identificar funcoes e verificar se os tipos dos argumentos estao corretos
TipoPrimitivo* STS_recuperaFuncao(StringSet *ss, char *s, int *tamanho);

void STS_liberaParametro(TipoPrimitivo *p);

//retorna -1 se nao for encontrada palavra com esse tipo
//retorna tipoPrimitivo (int>0) correspondente ao tipo da variavel 
int STS_vazia(StringSet *ss);
void STS_limpa(StringSet *ss);
void STS_libera(StringSet *ss);
