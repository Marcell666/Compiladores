#ifndef ARVORE_H
#define ARVORE_H
typedef struct arvore Arvore;
typedef struct exp Exp;
//typedef struct programa Programa;
typedef struct definicao Definicao;
typedef struct bloco Bloco;
typedef struct comando Comando;
typedef struct chamadaArg ChamadaArg;
typedef struct chamada Chamada;
typedef struct numeral Numeral;
typedef struct var Variavel;

typedef enum tipoPrimitivo{
	CHAR,
	INT,
	STRING,
	BOOLEAN,
	DOUBLE,
	VOID,
	//nao existe em monga, mas sera util para declarar funcoes sem retorno
	ARRAY_CHAR,
	ARRAY_INT,
	ARRAY_STRING,	
	ARRAY_BOOLEAN,
	ARRAY_DOUBLE
	/*
		TODO
		Corrigir!!
		O tipo precisaria ser uma estrutura, que guardaria outra estrutura tipo e
		um valor deste enum, alem de uma tag que decidiria se ele é o ultimo ou nao,
		ou talvez pudesse indicar se o ultimo pelo valor no ponteiro de "subtipo"
	*/
	

}TipoPrimitivo;


typedef enum expression_binary_tag {
//operadores
	ESoma,
	EMult,
	EDiv,
	ESub,
//relacionais
	EDois_Iguais,
	EDiferente_Comp,
	EMenorIgual,
	EMaiorIgual,
	EMenor,
	EMaior,
//operadores logicos
	EELogico,
	EOuLogico
} EBinaryTag;


Arvore* ARV_cria(Definicao *d);

//-- Numeral ------------------
Numeral* ARV_criaNumeralInteiro(int lineNumber, int i);	
Numeral* ARV_criaNumeralDouble(int lineNumber, double d);

//-- Expressao ------------------

Exp* ARV_criaExpBinaria(int lineNumber, EBinaryTag operador, Exp* e1, Exp* e2);

Exp* ARV_criaExpString(int lineNumber, char *s);
Exp* ARV_criaExpBoolean(int lineNumber, int b);
Exp* ARV_criaExpInteiro(int lineNumber, int i);
Exp* ARV_criaExpChar(int lineNumber, char c);

Exp* ARV_criaExpParenteses(int lineNumber, Exp* e);
Exp* ARV_criaExpMenosUnario(int lineNumber, Exp* e);

Exp* ARV_criaExpNumeral(int lineNumber, Numeral* n);

Exp* ARV_criaExpVariavel(int lineNumber, Variavel *var);

Exp* ARV_criaExpChamada(int lineNumber, Chamada *c);
//Exp* ARV_criaExpChamada(char *id, ChamadaArg *ac);
Exp* ARV_criaExpAs(int lineNumber, Exp *exp, TipoPrimitivo paraTipo);
//Exp* ARV_criaExpNewArray(char *id, TipoPrimitivo tipo );
Exp* ARV_criaExpNewArray(int lineNumber, TipoPrimitivo tipo, Exp* e);
//-- Tipo ----------------------

TipoPrimitivo ARV_criaTipoArray(TipoPrimitivo t);

//-- Variavel ------------------

Variavel* ARV_criaVariavelId(int lineNumber, char *id);
Variavel* ARV_criaVariavelArray(int lineNumber, Exp* e1, Exp* e2);

//-- Chamada ------------------

ChamadaArg* ARV_criaChamadaArg(int lineNumber, Exp *exp, ChamadaArg *ac);
Chamada* ARV_criaChamada(int lineNumber, char *id, ChamadaArg *ac);

//-- Programa ------------------

//Programa* ARV_criaPrograma(Definicao *def);

//-- Definicao ------------------
Definicao* ARV_insereDefinicao(Definicao* d1, Definicao* d2);//faz o prox de d1 apontar para d2
Definicao* ARV_criaDefinicaoVariavel(int lineNumber,  Definicao *prox, char *id, TipoPrimitivo tipo);
Definicao* ARV_criaDefinicaoParametro(int lineNumber,  Definicao *prox, char *id, TipoPrimitivo tipo);
Definicao* ARV_criaDefinicaoFuncao(int lineNumber,  Definicao *prox, char *id, Definicao* parametro, TipoPrimitivo tipo, Bloco* bloco);

//-- Bloco ------------------

Bloco* ARV_criaBloco(int lineNumber, Definicao* d, Comando* c);

//-- Comando ------------------
Comando* ARV_insereComando(Comando* c1, Comando* c2);//faz o prox de c1 apontar para c2
Comando* ARV_criaComandoIf(int lineNumber,  Comando *prox, Exp* e, Bloco *blocoIf, Bloco* blocoElse);
Comando* ARV_criaComandoWhile(int lineNumber,  Comando *prox, Exp* e, Bloco *blocoIO);
Comando* ARV_criaComandoAtribuicao(int lineNumber,  Comando *prox, Variavel *var, Exp* e);
Comando* ARV_criaComandoReturn(int lineNumber,  Comando *prox, Exp* e);
Comando* ARV_criaComandoChamada(int lineNumber,  Comando *prox, Chamada* c);
Comando* ARV_criaComandoArroba(int lineNumber,  Comando *prox, Exp* e);
Comando* ARV_criaComandoBloco(int lineNumber,  Comando *prox, Bloco* b);

//-- Exibe ---------------------

void ARV_exibe(Arvore* a);
void ARV_exibeExp(Exp *e, int identacao);
void ARV_exibeNumeral(Numeral *n, int identacao);
void ARV_exibeVar(Variavel *v, int identacao);
void ARV_exibeDefinicao(Definicao* d, int identacao);
void ARV_exibeBloco(Bloco *b, int identacao);
void ARV_exibeComando(Comando* c, int identificao);
void ARV_exibeChamadaArg(ChamadaArg* c, int identificao);
void ARV_exibeChamada(Chamada* c, int identificao);


//-- Utils ---------------------

char* tipoPrimitivoToString(TipoPrimitivo tp);
#endif
