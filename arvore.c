#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"
#include "arvoreInterna.h"

//1:10 recording 409

Arvore* ARV_cria(Definicao * d){
	Arvore *a;
	a = (Arvore*) malloc(sizeof(Arvore));
	if(a==NULL){
		fprintf(stderr, "Erro ao criar Arvore\n");
		exit(1);
	}
	a->topo = d;
	a->atual = d;
	a->tag = TDefinicao;
	
	return a;
}

Numeral* criaNumeral(int lineNumber, TipoPrimitivo tag){
	Numeral* n;
	n = (Numeral*) malloc(sizeof(Numeral));
	if(n==NULL){
		fprintf(stderr, "Erro ao alocar Numeral\n");
		exit(1);
	}
	n->tag = tag;
	n->lineNumber = lineNumber;
	return n;
}	

Numeral* ARV_criaNumeralInteiro(int lineNumber, int i){
	Numeral* n = criaNumeral(lineNumber, INT);
	n->u.ival = i;
	return n;
}	

Numeral* ARV_criaNumeralDouble(int lineNumber, double d){
	Numeral* n = criaNumeral(lineNumber, DOUBLE);
	n->u.dval = d;
	return n;
}	

Exp* criaExp(int lineNumber, ETag tag){
	Exp* e = (Exp*) malloc(sizeof(Exp));
	if(e==NULL){
		fprintf(stderr, "Erro ao alocar Expressao\n");
		exit(1);
	}
	e->tag = tag;
	e->lineNumber = lineNumber;
	return e;
}

Exp* ARV_criaExpBinaria(int lineNumber, EBinaryTag operador, Exp* e1, Exp* e2){
	Exp *e = criaExp(lineNumber, EBinaria);
	e->u.bin.e1 = e1;
	e->u.bin.e2 = e2;

	e->u.bin.tag = operador;

	return e;
}

Exp* ARV_criaExpString(int lineNumber, char *s){
	Exp *e = criaExp(lineNumber, EString);

	strcpy(e->u.sval, s);

	return e;
}

Exp* ARV_criaExpBoolean(int lineNumber, int b){
	Exp *e = criaExp(lineNumber, EBoolean);

	e->u.ival = b;

	return e;
}

Exp* ARV_criaExpInteiro(int lineNumber, int i){
	Exp *e = criaExp(lineNumber, EInteiro);

	e->u.ival = i;

	return e;
}

Exp* ARV_criaExpChar(int lineNumber, char c){
	Exp *e = criaExp(lineNumber, EChar);

	e->u.cval = c;

	return e;
}

Exp* ARV_criaExpNumeral(int lineNumber, Numeral* n){
	Exp* e = criaExp(lineNumber, ENumeral);
	e->u.numeral = n;//TODO corrigir
	return e;
}

Exp* ARV_criaExpVariavel(int lineNumber, Variavel* var){
	Exp* e = criaExp(lineNumber, EVariavel);
	e->u.var = var;
	return e;
}

Exp* ARV_criaExpParenteses(int lineNumber, Exp* exp){
	Exp *e = criaExp(lineNumber, EParenteses);
	e->u.e = exp;
	return e;
}

Exp* ARV_criaExpMenosUnario(int lineNumber, Exp* exp){
	Exp *e = criaExp(lineNumber, EMenosUnario);
	e->u.e = exp;
	return e;
}
/*
Exp* ARV_criaExpChamada(char *id, ChamadaArg *ac){
	Exp *e = criaExp(EChamada);
	e->u.chamadaFuncao = ARV_criaChamada(id, ac);
//	strcpy(e->u.chamadaFuncao.id, id);
//	e->u.chamadaFuncao.ac = ac;
	return e;
}
*/
Exp* ARV_criaExpChamada(int lineNumber, Chamada *c){
	Exp *e = criaExp(lineNumber, EChamada);
	e->u.chamadaFuncao = c;
	return e;
}

Exp* ARV_criaExpAs(int lineNumber, Exp *exp, TipoPrimitivo paraTipo){
	Exp *e = criaExp(lineNumber, EAs);
	e->u.as.exp = exp;
	e->u.as.deTipo = VOID;
/*TODO
	Posso nao especificar o tipo do qual a expressao vem?
*/
	//e->u.as.deTipo = deTipo;
	e->u.as.paraTipo = paraTipo;

	return e;
}

Exp* ARV_criaExpNewArray(int lineNumber, TipoPrimitivo tipo, Exp* exp){
	Exp *e = criaExp(lineNumber, ENewArray);
	//strcpy(e->u.newArray.id, id);
	e->u.newArray.tipo = tipo;
	e->u.newArray.exp = exp;
	return e;
}

TipoPrimitivo ARV_criaTipoArray(TipoPrimitivo t){
	return t+ARRAY_CHAR;
}

Variavel* ARV_criaVariavelId(int lineNumber, char *id){
	Variavel* v = (Variavel*) malloc(sizeof(Variavel));
	if(v==NULL){
		fprintf(stderr, "Erro ao alocar Variavel\n");
		exit(1);
	}
	v->tag = PRIMITIVO;
	strcpy(v->u.id, id);
	v->lineNumber = lineNumber;
	return v;
}


Variavel* ARV_criaVariavelArray(int lineNumber, Exp* e1, Exp* e2){
	Variavel* v = (Variavel*) malloc(sizeof(Variavel));
	if(v==NULL){
		fprintf(stderr, "Erro ao alocar Variavel\n");
		exit(1);
	}
	v->tag = ARRAY;
	//TODO consertar isso
	
	v->u.array.exp1 = e1;
	v->u.array.exp2 = e2;
	v->lineNumber = lineNumber;
	return v;
}


ChamadaArg* ARV_criaChamadaArg(int lineNumber, Exp *exp, ChamadaArg *chamadaArg){
	ChamadaArg* ac;
	if(exp == NULL) return NULL;
	ac = (ChamadaArg*) malloc(sizeof(ChamadaArg));
	if(ac==NULL){
		fprintf(stderr, "Erro ao alocar Chamada Argumentos \n");
		exit(1);
	}
	ac->exp = exp;
	ac->prox = chamadaArg;
	ac->lineNumber = lineNumber;
	return ac;
}

Chamada* ARV_criaChamada(int lineNumber, char *id, ChamadaArg *ac){
	Chamada* c = (Chamada*) malloc(sizeof(Chamada));
	if(c==NULL){
		fprintf(stderr, "Erro ao alocar Chamada de funcao");
		exit(1);
	}
	strcpy(c->id, id);
	c->args = ac;
	c->lineNumber = lineNumber;
	return c;
}

/*
Programa* ARV_criaPrograma(Definicao *def){
	Programa* p = (Programa*) malloc(sizeof(Programa));
	if(p==NULL){
		fprintf(stderr, "Erro ao alocar Chamada de funcao");
		exit(1);
	}
	p->definicao = def;
	return p;
}
*/
Definicao* criaDefinicao(int lineNumber, DTag tag, Definicao *prox){
	Definicao* def = (Definicao*) malloc(sizeof(Definicao));
	if(def==NULL){
		fprintf(stderr, "Erro ao alocar Definicao");
		exit(1);
	}
	def->tag = tag;
	def->prox = prox;
	def->lineNumber = lineNumber;
	return def;
}

Definicao* ARV_criaDefinicaoVariavel(int lineNumber,  Definicao *prox, char *id, TipoPrimitivo tipo){
	Definicao* def = criaDefinicao(lineNumber, EDefinicaoVariavel, prox);
	strcpy(def->u.DefinicaoVariavel.id, id);
	def->u.DefinicaoVariavel.tipo = tipo;
	return def;
}

Definicao* ARV_criaDefinicaoParametro(int lineNumber,  Definicao *prox, char *id, TipoPrimitivo tipo){
	Definicao* def = criaDefinicao(lineNumber, EDefinicaoParametro, prox);
	strcpy(def->u.DefinicaoVariavel.id, id);
	def->u.DefinicaoVariavel.tipo = tipo;
	return def;
}

Definicao* ARV_criaDefinicaoFuncao(int lineNumber,  Definicao *prox, char *id, Definicao* parametro, TipoPrimitivo tipo, Bloco* bloco){
	Definicao* def = criaDefinicao(lineNumber, EDefinicaoFuncao, prox);
	strcpy(def->u.DefinicaoFuncao.id, id);
	def->u.DefinicaoFuncao.tipoRetorno = tipo;

	def->u.DefinicaoFuncao.parametro = parametro;
	def->u.DefinicaoFuncao.bloco = bloco;

	return def;
}

Definicao* ARV_insereDefinicao(Definicao* d1, Definicao* d2){
	if(d1==NULL){
		return d2;
	}
	d1->prox = d2;
	return d1;
}
Bloco* ARV_criaBloco(int lineNumber, Definicao* d, Comando* c){
	Bloco* b; 
	if(d == NULL && c == NULL) return NULL;
	b= (Bloco*) malloc(sizeof(Bloco));
	if(b==NULL){
		fprintf(stderr, "Erro ao alocar Bloco");
		exit(1);
	}
	b->definicaoVariavel = d;
	b->comando = c;
	b->lineNumber = lineNumber;
	return b;
}


Comando* criaComando(int lineNumber, CTag tag, Comando *prox){
	Comando* c = (Comando*) malloc(sizeof(Comando));
	if(c==NULL){
		fprintf(stderr, "Erro ao alocar Comando");
		exit(1);
	}
	c->tag = tag;
	c->prox = prox;
	c->lineNumber = lineNumber;
	return c;
}

Comando* ARV_insereComando(Comando* c1, Comando* c2){
	if(c1==NULL){
		return c2;
	}
	c1->prox = c2;
	return c1;
}

Comando* ARV_criaComandoIf(int lineNumber,  Comando *prox, Exp* e, Bloco *blocoIf, Bloco* blocoElse){
	Comando* c = criaComando(lineNumber, CIf, prox);

	c->u.no_If.exp = e;
	c->u.no_If.bloco_if = blocoIf;
	c->u.no_If.bloco_else = blocoElse;
	return c;
}

Comando* ARV_criaComandoWhile(int lineNumber,  Comando *prox, Exp* e, Bloco *bloco){
	Comando* c = criaComando(lineNumber, CWhile, prox);

	c->u.no_While.exp = e;
	c->u.no_While.bloco = bloco;
	return c;
}

Comando* ARV_criaComandoAtribuicao(int lineNumber,  Comando *prox, Variavel *v, Exp* e){
	Comando* c = criaComando(lineNumber, CAtribuicao, prox);

	c->u.Atribuicao.var = v;
	c->u.Atribuicao.exp = e;
	return c;
}

Comando* ARV_criaComandoReturn(int lineNumber,  Comando *prox, Exp* e){
	Comando* c = criaComando(lineNumber, CReturn, prox);

	c->u.no_Return.exp = e;
	return c;
}

Comando* ARV_criaComandoChamada(int lineNumber,  Comando *prox, Chamada* chamada){
	Comando* c = criaComando(lineNumber, CChamada, prox);

	c->u.chamada = chamada;
	return c;
}

Comando* ARV_criaComandoArroba(int lineNumber,  Comando *prox, Exp* e){
	Comando* c = criaComando(lineNumber, CArroba, prox);

	c->u.arrobaExp = e;
	return c;
}


Comando* ARV_criaComandoBloco(int lineNumber,  Comando *prox, Bloco* b){
	Comando* c = criaComando(lineNumber, CBloco, prox);

	c->u.bloco = b;
	return c;
}


//---------- Exibe -----------------------------

void ARV_identa(int n){
	while(n--){
		printf("\t");
	}
}

//TODO mover isso para modulo utilitario
//copia em costura.c
//copia em arvore.c
char* tipoPrimitivoToString(TipoPrimitivo tp){
	switch(tp){
		case CHAR:	return "CHAR";
		case INT:	return "INT";
		case STRING:	return "STRING";
		case BOOLEAN:	return "BOOLEAN";
		case DOUBLE:	return "DOUBLE";
		case ARRAY_CHAR: return "ARRAY_CHAR";
		case ARRAY_INT: return "ARRAY_INT";
		case ARRAY_STRING: return "ARRAY_STRING";
		case ARRAY_BOOLEAN: return "ARRAY_BOOLEAN";
		case ARRAY_DOUBLE: return "ARRAY_DOUBLE";
		case VOID:	return "VOID";
	}
	return "Erro no tipoPrimitivo";
}

void ARV_exibe(Arvore* a){
	if(a==NULL) printf("FUDEU");
	if(a->tag == TDefinicao){
		printf("Arvore < \n");
		ARV_exibeDefinicao(a->topo, 1);
		printf("\n>");
	}
	else{
		printf("\nFUDEU\n");
	}
}

char* binToOp(EBinaryTag tag){
	if(tag == ESoma) return "+";
	if(tag == ESub) return "-";
	if(tag == EMult) return "*";
	if(tag == EDiv) return "/";

	if(tag == EDois_Iguais) return "==";
	if(tag == EDiferente_Comp) return "!=";
	if(tag == EMenorIgual) return "<=";
	if(tag == EMaiorIgual) return ">=";
	if(tag == EMenor) return "<";
	if(tag == EMaior) return ">";

	if(tag == EELogico) return "&&";
	if(tag == EOuLogico) return "||";
	return "ERRO com operador binario";
}



void ARV_exibeExp(Exp *e, int identacao){
	char *op = binToOp(e->u.bin.tag);
	switch(e->tag){
	case EBinaria:
		ARV_identa(identacao);
		fprintf(stdout, "Exp opBin <\n" );
		ARV_exibeExp(e->u.bin.e1, identacao+1);
		ARV_identa(identacao);
		fprintf(stdout, " |%s| \n", op);
		ARV_exibeExp(e->u.bin.e2, identacao+1);
		ARV_identa(identacao);
		fprintf(stdout, " >\n");
	break;
	case EString:
		ARV_identa(identacao);
		fprintf(stdout, "Exp String < %s >\n", e->u.sval);
		fprintf(stdout, " >");
	
	break;
	case EBoolean:
		ARV_identa(identacao);
		fprintf(stdout, "Exp String < %s >\n", e->u.boolean?"TRUE":"FALSE" );
	break;
	case EInteiro:
		ARV_identa(identacao);
		fprintf(stdout, "Exp Inteiro < %d >\n", e->u.ival );
	break;
	case EDouble:
		ARV_identa(identacao);
		fprintf(stdout, "Exp Double < %lf >\n", e->u.dval );
	break;
	case EChar:
		ARV_identa(identacao);
		fprintf(stdout, "Exp Char < %c >\n", e->u.cval );
	break;
	case ENumeral:
		ARV_identa(identacao);
		fprintf(stdout, "Exp Numeral <\n" );

		ARV_exibeNumeral(e->u.numeral, identacao+1);

		ARV_identa(identacao);
		fprintf(stdout, ">\n");
	break;


	case EVariavel:
		ARV_identa(identacao);
		fprintf(stdout, "Exp Var <\n" );
		ARV_exibeVar(e->u.var, identacao+1);

		ARV_identa(identacao);
		fprintf(stdout, ">\n");
	break;

	case EParenteses:
		ARV_identa(identacao);
		fprintf(stdout, "Exp Parenteses <\n" );

		ARV_exibeExp(e->u.e, identacao+1);

		ARV_identa(identacao);
		fprintf(stdout, ">\n");
	break;
	case EMenosUnario:
		ARV_identa(identacao);
		fprintf(stdout, "Exp MenosUnario <\n" );

		ARV_exibeExp(e->u.e, identacao+1);

		ARV_identa(identacao);
		fprintf(stdout, ">\n");
	break;


	case EChamada:
		ARV_identa(identacao);
		fprintf(stdout, "Exp Chamada <\n" );

		ARV_exibeChamada(e->u.chamadaFuncao, identacao+1);

		ARV_identa(identacao);
		fprintf(stdout, ">\n");
	break;
	case EAs:
		ARV_identa(identacao);
		fprintf(stdout, "Exp As %s %s <\n",tipoPrimitivoToString(e->u.as.deTipo),
					  tipoPrimitivoToString(e->u.as.paraTipo)
		);

		ARV_exibeExp(e->u.as.exp,identacao+1);

		ARV_identa(identacao);
		fprintf(stdout, ">\n");
	break;
	case ENewArray:
		ARV_identa(identacao);
		fprintf(stdout, "Exp New Array %s <\n",					  tipoPrimitivoToString(e->u.newArray.tipo)
		);

		ARV_exibeExp(e->u.newArray.exp,identacao+1);

		ARV_identa(identacao);
		fprintf(stdout, ">\n");
	break;
	default:
		fprintf(stderr, "Erro ao exibir expressao, tipo nao encontrado\n");
	}
}

void ARV_exibeNumeral(Numeral *n, int identacao){
	if(n->tag == DOUBLE){
		ARV_identa(identacao);
		fprintf(stdout, "Numeral %s %lf\n", tipoPrimitivoToString(n->tag),  n->u.dval);
		ARV_identa(identacao);
		fprintf(stdout, ">\n");
	}else if(n->tag == INT){
		ARV_identa(identacao);
		fprintf(stdout, "Numeral %s %d\n", tipoPrimitivoToString(n->tag),  n->u.ival);
		ARV_identa(identacao);
		fprintf(stdout, ">\n");
	}
}


void ARV_exibeVar(Variavel *v, int identacao){
	if(v->tag == PRIMITIVO){
		ARV_identa(identacao);
		fprintf(stdout, "Variavel PRIMITIVO %s\n", v->u.id);
	}else if(v->tag == ARRAY){
		//TODO consertar isso!!
		ARV_identa(identacao);
		fprintf(stdout, "Variavel ARRAY <\n" );
		ARV_exibeExp(v->u.array.exp1, identacao+1);
		ARV_exibeExp(v->u.array.exp2, identacao+1);

		ARV_identa(identacao);
		fprintf(stdout, ">\n");
	}
}

void ARV_exibeDefinicao(Definicao* d, int identacao){
	while(d != NULL){
		if(d->tag == EDefinicaoVariavel || d->tag == EDefinicaoParametro){
			ARV_identa(identacao);
			fprintf(stdout, "Def Var < %s %s>\n", d->u.DefinicaoVariavel.id , tipoPrimitivoToString(d->u.DefinicaoVariavel.tipo));
		}else if(d->tag == EDefinicaoFuncao){
			ARV_identa(identacao);
			fprintf(stdout, "Def Funcao %s <\n", d->u.DefinicaoFuncao.id);
			ARV_exibeDefinicao(d->u.DefinicaoFuncao.parametro, identacao+1);
			ARV_identa(identacao);
			fprintf(stdout, " > retorno %s < \n", tipoPrimitivoToString(d->u.DefinicaoFuncao.tipoRetorno));
			ARV_exibeBloco(d->u.DefinicaoFuncao.bloco, identacao+1);
			
			ARV_identa(identacao);
			fprintf(stdout, ">\n");
		
		}
		d = d->prox;
	}
}

void ARV_exibeBloco(Bloco *b, int identacao){
	ARV_identa(identacao);
	fprintf(stdout, "Bloco <\n");
	if(b == NULL){
		ARV_identa(identacao);
		fprintf(stdout, "- Vazio -\n");
		return;
	}
	else{
		ARV_exibeDefinicao(b->definicaoVariavel, identacao+1);
		ARV_exibeComando(b->comando, identacao+1);
	}
	ARV_identa(identacao);
	fprintf(stdout, ">\n");
}

void ARV_exibeComando(Comando* c, int identacao){
	while(c != NULL){		
		switch(c->tag){
		case CIf:
			ARV_identa(identacao);
			fprintf(stdout, "Comando If <\n");
			ARV_exibeExp(c->u.no_If.exp, identacao+1);
			ARV_identa(identacao);
			fprintf(stdout, ", \n");
			ARV_exibeBloco(c->u.no_If.bloco_if, identacao+1);
			ARV_identa(identacao);
			fprintf(stdout, ", \n");
			ARV_exibeBloco(c->u.no_If.bloco_else, identacao+1);
			ARV_identa(identacao);
			fprintf(stdout, ">\n");
		break;
		case CWhile:
			ARV_identa(identacao);
			fprintf(stdout, "Comando While <\n");

			ARV_exibeExp(c->u.no_While.exp, identacao+1);

			ARV_identa(identacao);
			fprintf(stdout, ", \n");
			ARV_exibeBloco(c->u.no_While.bloco, identacao+1);

			ARV_identa(identacao);
			fprintf(stdout, ">\n");
		break;
		case CAtribuicao:
			ARV_identa(identacao);
			fprintf(stdout, "Comando Atribuicao %s <\n", 
c->u.Atribuicao.var->u.id);
			ARV_exibeExp(c->u.Atribuicao.exp, identacao+1);
			ARV_identa(identacao);
			fprintf(stdout, ">\n");
		break;
		case CReturn:
			ARV_identa(identacao);
			fprintf(stdout, "Comando Return <\n");
			ARV_exibeExp(c->u.no_Return.exp, identacao+1);
			ARV_identa(identacao);
			fprintf(stdout, ">\n");
		break;
		case CChamada:
			ARV_identa(identacao);
			fprintf(stdout, "Comando Chamada <\n");
			ARV_exibeChamada(c->u.chamada, identacao+1);
			ARV_identa(identacao);
			fprintf(stdout, ">\n");
		break;
		case CArroba:
			ARV_identa(identacao);
			fprintf(stdout, "Comando Arroba <\n");
			ARV_exibeExp(c->u.arrobaExp, identacao+1);
			ARV_identa(identacao);
			fprintf(stdout, ">\n");
		break;
		case CBloco:
			ARV_identa(identacao);
			fprintf(stdout, "Comando Bloco <\n");
			ARV_exibeBloco(c->u.bloco, identacao+1);
			ARV_identa(identacao);	
			fprintf(stdout, ">\n");
		break;
		}
		c = c->prox;
	}
}

void ARV_exibeChamadaArg(ChamadaArg* c, int identacao){
	while(c != NULL){
		ARV_identa(identacao);
		fprintf(stdout, "ChamadaArg <\n");
		ARV_exibeExp(c->exp, identacao+1);
		ARV_identa(identacao);
		fprintf(stdout, ">\n");
		c = c->prox;
	}
}

void ARV_exibeChamada(Chamada* c, int identacao){
	ARV_identa(identacao);
	fprintf(stdout, "Chamada %s <\n", c->id);
	ARV_exibeChamadaArg(c->args, identacao+1);
	ARV_identa(identacao);
	fprintf(stdout, ">\n");
}
