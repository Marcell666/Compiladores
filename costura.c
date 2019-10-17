#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"
#include "arvoreInterna.h"
#include "lib/pilhaDeLista.h"
#include "costura.h"

void costuraDefinicao( PilhaDeLista *pv, PilhaDeLista *pf, Definicao *d);
void costuraBloco( PilhaDeLista *pv, PilhaDeLista *pf, Bloco *b);
void costuraComando(PilhaDeLista *pv, PilhaDeLista *pf, Comando *c);
TipoPrimitivo costuraVariavel(PilhaDeLista *pv, PilhaDeLista *pf, Variavel *v);
void costuraChamadaArgs(PilhaDeLista *pv, PilhaDeLista *pf, ChamadaArg *ac, TipoPrimitivo *v, int n);
TipoPrimitivo costuraChamada(PilhaDeLista *pv, PilhaDeLista *pf, Chamada *c);

TipoPrimitivo costuraAs(PilhaDeLista *pv, PilhaDeLista *pf, Exp *e);
int mesmoNumeral(TipoPrimitivo tp1,	TipoPrimitivo tp2,	TipoPrimitivo *ret);
int numeralDiferente(Exp *e, TipoPrimitivo tp1,	TipoPrimitivo tp2);

TipoPrimitivo costuraExp(PilhaDeLista *pv, PilhaDeLista *pf, Exp *e);


void CTR_verificaVariaveis(Arvore *a){
	fprintf(stderr, "Costurando Arvore \n");	
	/*
		percorre arvore
		verifica no atual
		se declara variavel coloca o nome no gerenciador
		se chama variavel verifica o nome no gerenciador
		se for um type cast 'as' cria um no com o tipo certo (?)
		
		tipagem(?)
	*/
	PilhaDeLista *pv = PDL_cria();
	PilhaDeLista *pf = PDL_cria();
	PDL_insereEscopo(pv);
	costuraDefinicao(pv, pf, a->topo);
	PDL_removeEscopo(pv);

}

void costuraDefinicao( PilhaDeLista *pv, PilhaDeLista *pf, Definicao *d){
	if(d == NULL) return;
	/*	TODO
		O yacc esta lendo as coisas na ordem inversa
		por causa da recursao a esquerda para ler as definicoes
		ou eu inverto a lista de novo
		ou minhas definicoes vao ficar todas ao contrario
		Isto é, numa funcao definida acima de outra
		foo2{}
		foo{}
		foo2 NAO fica visivel para foo
		mas se estiver assim
		foo{}
		foo2{}
		Ai da certo
		Para isso nao acontecer preciso inverter a ordem de leitura

		vou desinverter para definicoes de funcoes, mas nao para os parametro de uma definicao de funcao
		assim quando chamar um funcao nao preciso inverter os argumentos
	*/

	fprintf(stderr, "Costurando Definicao \n");
	if(d->tag == EDefinicaoVariavel){
		costuraDefinicao(pv, pf, d->prox);
		//desinvertendo
		
		if(!PDL_inserePalavra(pv, d->u.DefinicaoVariavel.id, d->u.DefinicaoVariavel.tipo)){
			fprintf(stderr, "linha %d, ERRO ao incluir nome da variavel %s na tabela\n", d->lineNumber, d->u.DefinicaoVariavel.id);
			exit(1);
		}		
	}else if(d->tag == EDefinicaoParametro){
		fprintf(stderr, "linha %d, incluindo parametro %s na tabela de funcoes\n", d->lineNumber, d->u.DefinicaoVariavel.id);
		//nessa estrutura nao vamos retirar com a saida do escopo
		if(!PDL_insereParametro(pf, d->u.DefinicaoVariavel.id, d->u.DefinicaoVariavel.tipo)){
			fprintf(stderr, "linha %d, ERRO ao incluir nome do parametro %s na tabela de funcoes\n", d->lineNumber, d->u.DefinicaoVariavel.id);
			exit(1);
		}

		//nessa estrutura vamos retirar com a saida do escopo
		if(!PDL_inserePalavra(pv, d->u.DefinicaoVariavel.id, d->u.DefinicaoVariavel.tipo)){
			fprintf(stderr, "linha %d, ERRO ao incluir nome do parametro %s na tabela de variaveis\n", d->lineNumber, d->u.DefinicaoVariavel.id);
			exit(1);
		}
		costuraDefinicao(pv, pf, d->prox);
		//vai continuar invertido
		
	}else if(d->tag == EDefinicaoFuncao){
		costuraDefinicao(pv, pf, d->prox);
		//desinvertendo

		//guardando funcao e parametros dela
		//para tipagem
		fprintf(stderr, "linha %d, incluindo nome da funcao %s na tabela de funcoes\n", d->lineNumber, d->u.DefinicaoFuncao.id);

		if(!PDL_insereFuncao(pf, d->u.DefinicaoFuncao.id,  d->u.DefinicaoFuncao.tipoRetorno)){
			fprintf(stderr, "linha %d, ERRO ao incluir nome da funcao %s na tabela de funcoes\n", d->lineNumber, d->u.DefinicaoFuncao.id);
			exit(1);
		}
		//guardando parametros como variaveis
		//para costura
		if(!PDL_inserePalavra(pv, d->u.DefinicaoFuncao.id,  d->u.DefinicaoFuncao.tipoRetorno)){
			fprintf(stderr, "linha %d, ERRO ao incluir nome da funcao %s na tabela de variaveis\n", d->lineNumber, d->u.DefinicaoFuncao.id);
			exit(1);
		}
		PDL_insereEscopo(pv);
		costuraDefinicao(pv, pf, d->u.DefinicaoFuncao.parametro);
		costuraBloco(pv, pf, d->u.DefinicaoFuncao.bloco);
		PDL_removeEscopo(pv);
	}
}

void costuraBloco( PilhaDeLista *pv, PilhaDeLista *pf, Bloco *b){
	if(b == NULL) return;
	fprintf(stderr, "Costurando Bloco \n");
	costuraDefinicao(pv, pf, b->definicaoVariavel);
	costuraComando(pv, pf, b->comando);
}

//TODO mover isso para modulo utilitario
//copia em costura.c
//copia em arvore.c
/*
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
*/
void costuraComando(PilhaDeLista *pv, PilhaDeLista *pf, Comando *c){
	TipoPrimitivo tp1;
	TipoPrimitivo tp2;
	TipoPrimitivo* v;
	if(c == NULL) return;	
	fprintf(stderr, "Costurando Comando \n");
	switch(c->tag){
		case CIf:
			fprintf(stderr, "Costurando Comando If\n");
			tp1 = costuraExp(pv, pf, c->u.no_If.exp);	
			if(tp1 != BOOLEAN && tp1 != INT){
				fprintf(stderr, "linha %d, Erro! Comando if avalia somente int ou boolean\n", c->lineNumber);
			}
			PDL_insereEscopo(pv);
				costuraBloco(pv, pf, c->u.no_If.bloco_if);
			PDL_removeEscopo(pv);
			if(c->u.no_If.bloco_else!=NULL){
				PDL_insereEscopo(pv);
					costuraBloco(pv, pf, c->u.no_If.bloco_else);
				PDL_removeEscopo(pv);
			}
		break;
		case CWhile:
			fprintf(stderr, "Costurando Comando While\n");
			tp1 = costuraExp(pv, pf, c->u.no_While.exp);
			if(tp1 != BOOLEAN && tp1 != INT){
				fprintf(stderr, "linha %d, Erro! Comando while avalia somente int ou boolean\n", c->lineNumber);
			}
			PDL_insereEscopo(pv);
			costuraBloco(pv, pf, c->u.no_While.bloco);
			PDL_removeEscopo(pv);
		break;
		case CAtribuicao:
			fprintf(stderr, "Costurando Comando Atribuicao\n");
			tp1 = costuraVariavel(pv, pf, c->u.Atribuicao.var);
			tp2 = costuraExp(pv, pf, c->u.Atribuicao.exp);
			if(tp1 != tp2){
				fprintf(stderr, "linha %d, Erro! Variavel %s do tipo %s, mas recebeu %s\n", c->lineNumber, c->u.Atribuicao.var->u.id, tipoPrimitivoToString(tp1), tipoPrimitivoToString(tp2));
			}

		break;
		case CReturn:
			fprintf(stderr, "Costurando Comando Return\n");
			v = PDL_recuperaFuncao(pf, NULL, NULL);
			if(v == NULL){
				fprintf(stderr, "linha %d, Erro! Nao e possivel recuperar ultima funcao da tabela de funcoes\n", c->lineNumber);
			}
				
			tp1 = v[0];
			tp2 = costuraExp(pv, pf, c->u.no_Return.exp);
			PDL_liberaParametro(v);
			if(tp1 == VOID){
				fprintf(stderr, "linha %d, Erro! Funcao void, nao deveria retornar\n", c->lineNumber);
				exit(1);
			}
			if(tp1 != tp2){
				fprintf(stderr, "linha %d, Erro! Funcao retorna tipo %s, mas esta retornando %s\n", c->lineNumber, tipoPrimitivoToString(tp1), tipoPrimitivoToString(tp2));
				exit(1);
			}
		break;
		case CChamada:
			fprintf(stderr, "Costurando Comando Chamada\n");
			costuraChamada(pv, pf, c->u.chamada);
		break;
		case CArroba:
			fprintf(stderr, "Costurando Comando Print\n");
			tp1 = costuraExp(pv, pf, c->u.arrobaExp);
			if(tp1 != CHAR &&
				tp1 != INT &&
				tp1 != STRING &&
				tp1 != BOOLEAN &&
				tp1 != DOUBLE){
					fprintf(stderr, "linha %d, Erro! Tipo %s, invalido para imprimir\n", c->lineNumber, tipoPrimitivoToString(tp1));
			}
		break;
		case CBloco:
			fprintf(stderr, "Costurando Comando Bloco\n");
			PDL_insereEscopo(pv);
			costuraBloco(pv, pf, c->u.bloco);
			PDL_removeEscopo(pv);
		break;
	}
	costuraComando(pv, pf, c->prox);
/*	TODO
		Por alguma razao nao consegui colocar na gramatica recursao a esquerda para os comandos
		Por isso eles estao na ordem correta
	*/
}
TipoPrimitivo costuraVariavel(PilhaDeLista *pv, PilhaDeLista *pf, Variavel *v){
	TipoPrimitivo tp1;
	TipoPrimitivo tp2;
	fprintf(stderr, "Costurando Variavel \n");
	if(v->tag == PRIMITIVO){
		fprintf(stderr, "Procurando Variavel %s\n", v->u.id);
		if(!PDL_procuraPalavra(pv, v->u.id, &tp1)){
			fprintf(stderr, "linha %d, ERRO! Variavel %s nao declarada\n", v->lineNumber, v->u.id);
			exit(1);
		}else{
			fprintf(stderr, "Variavel %s encontrada\n", v->u.id);
		}
		return tp1;
	}else if(v->tag == ARRAY){
		fprintf(stderr, "Procurando Array\n");
/*	
		TODO
		Nao precisa fazer isso		
		if(!PDL_procuraPalavra(pv, v->u.array.id, &t)){
			fprintf(stderr, "ERRO! Variavel %s nao declarada\n", v->u.id);
			exit(1);
		}
*/
		tp1 =	costuraExp(pv, pf, v->u.array.exp1);
		//Sera que eu deveria verificar a exp sozinha?
		//So para verificar se uma variavel que possa ter sido chamada ali ja foi declarada antes
		tp2 = costuraExp(pv, pf,v->u.array.exp2);
		if(tp2 != INT){
			fprintf(stderr, "linha %d, ERRO! So é possivel indexar por inteiro. Expressao de tipo %s\n", v->lineNumber, tipoPrimitivoToString(tp2));
			exit(1);
		}
		return ARV_criaTipoArray(tp1);
	}
	fprintf(stderr, "linha %d, ERRO! Variavel sem tipo\n", v->lineNumber);
	return VOID;
}

void costuraChamadaArgs(PilhaDeLista *pv, PilhaDeLista *pf, ChamadaArg *ac, TipoPrimitivo *v, int n){
	TipoPrimitivo tp1;
	TipoPrimitivo tp2;
	if(ac == NULL && n==0){
			return;
	}
	if(ac == NULL || n==0){
				fprintf(stderr, "linha %d, ERRO! numero de argumentos para a funcao incorreto\n", ac->lineNumber);
	}
	/*	TODO
		O yacc esta lendo as coisas na ordem inversa
		por causa da recursao a esquerda para ler as definicoes
		ou eu inverto a lista de novo
		ou minhas definicoes vao ficar todas ao contrario
		Isto é uma funcao definida acima de outra
		foo2{}
		foo{}
		NAO fica visivel para ela
		mas se estiver assim
		foo{}
		foo2{}
		Ai da certo
		Para isso nao acontecer preciso inverter a ordem de leitura
	*/
	fprintf(stderr, "Costurando ChamadaArgs \n");
	tp1 = v[0];
	tp2 = costuraExp(pv, pf, ac->exp);

	if(tp1 != tp2){
		fprintf(stderr, "linha %d, ERRO! Tipo do parametro deve ser %s e o argumento e %s\n", ac->lineNumber, tipoPrimitivoToString(tp1), tipoPrimitivoToString(tp2));
	}
	costuraChamadaArgs(pv, pf, ac->prox, v+1, n-1);
}

TipoPrimitivo costuraChamada(PilhaDeLista *pv, PilhaDeLista *pf, Chamada *c){
	int tam;
	TipoPrimitivo tp1;
	TipoPrimitivo *v;
	fprintf(stderr, "Costurando Chamada \n");

	fprintf(stderr, "linha %d, procurando Funcao chamada %s\n", c->lineNumber, c->id);
	if(!PDL_procuraPalavra(pv, c->id, &tp1)){
		fprintf(stderr, "linha %d, ERRO! Funcao chamada %s nao declarada\n", c->lineNumber, c->id);
		exit(1);
	}

	v = PDL_recuperaFuncao(pf, c->id, &tam);
	if(v == NULL){
		fprintf(stderr, "linha %d, ERRO! Nao foi possivel recuperar %s\n", c->lineNumber, c->id);
		exit(1);
	}
	
	tp1 = v[0];
	
	costuraChamadaArgs(pv, pf, c->args,v+1,tam-1);
	PDL_liberaParametro(v);

	return tp1;
}

TipoPrimitivo costuraAs(PilhaDeLista *pv, PilhaDeLista *pf, Exp *e){
	TipoPrimitivo tp1;
	TipoPrimitivo tp2;
	static int mascaraConversao[10][10] = {
	{1,1,0,0,0},
	{1,1,0,1,1},
	{0,0,1,0,0},
	{0,1,0,1,0},
	{0,1,0,0,1},
};

/*
	isso e so uma tabela para converter,
	estou vindo de uma linha para uma coluna
	X			CHAR,	INT,	STRING,	BOOLEAN,	DOUBLE
	CHAR,
	INT,
	STRING,
	BOOLEAN,
	DOUBLE

	Nao e possivel converter array de nenhum tipo para nenhum tipo
	E String so converte para String
*/

	tp1 = costuraExp(pv, pf, e->u.as.exp);
	tp2 = e->u.as.paraTipo;

	if(tp1 >= ARRAY_CHAR ||
			tp2 >= ARRAY_CHAR ||
			mascaraConversao[tp1][tp2] == 0){
		fprintf(stderr, "linha %d, ERRO! Nao e possivel converter %s para %s\n", e->lineNumber, tipoPrimitivoToString(tp1), tipoPrimitivoToString(tp2));
	}
	return tp2;
}
int mesmoNumeral(TipoPrimitivo tp1,	TipoPrimitivo tp2,	TipoPrimitivo *ret){
	if(tp1 == tp2){
		if(tp1 == INT ||
			tp1 == DOUBLE){
			if(ret!=NULL) *ret = tp1;
			return 1;
		}
	}
	return 0;
}

int numeralDiferente(Exp *e, TipoPrimitivo tp1,	TipoPrimitivo tp2){
	if(tp1 == INT &&
			tp2 == DOUBLE ){
		e->u.bin.e1 = ARV_criaExpAs(e->lineNumber, e->u.bin.e1, DOUBLE);
		return 1;
	}

	if((tp1 == DOUBLE &&
			tp2 == INT)){
		e->u.bin.e2 = ARV_criaExpAs(e->lineNumber, e->u.bin.e2, DOUBLE);
		return 2;
	}
	return 0;
}

TipoPrimitivo costuraExpBinaria(PilhaDeLista *pv, PilhaDeLista *pf, Exp *e){
	TipoPrimitivo tp1;
	TipoPrimitivo tp2;
	TipoPrimitivo tpRet;
	int ret;

	tp1 = costuraExp(pv, pf, e->u.bin.e1);
	tp2 = costuraExp(pv, pf, e->u.bin.e2);

	switch(e->u.bin.tag){
	//operadores
	case ESoma:
	case EMult:
	case EDiv:
	case ESub:

		if(mesmoNumeral(tp1, tp2, &tpRet)){
				return tpRet;
		}
		ret = numeralDiferente(e, tp1, tp2);
		
		if(ret == 1){
			return DOUBLE;
		}else if(ret == 2){
			return DOUBLE;
		}else{
			fprintf(stderr, "linha %d, ERRO! nao e possivel operar %s com %s\n", e->lineNumber, tipoPrimitivoToString(tp1), tipoPrimitivoToString(tp2));
			exit(1);
		}
  
		
	break;
//relacionais

	case EDois_Iguais:
	case EDiferente_Comp:
		if(tp1 == tp2){
			return BOOLEAN;
		}
	//note que nao tem break
	case EMenorIgual:
	case EMaiorIgual:
	case EMenor:
	case EMaior:

		if(mesmoNumeral(tp1, tp2, &tpRet)){
				return BOOLEAN;
		}
		ret = numeralDiferente(e, tp1, tp2);
		
		if(ret == 1){
			return BOOLEAN;
		}else if(ret == 2){
			return BOOLEAN;
		}else{
			fprintf(stderr, "linha %d, ERRO! nao e possivel comparar %s com %s\n", e->lineNumber, tipoPrimitivoToString(tp1), tipoPrimitivoToString(tp2));
			exit(1);
		}	
	break;


//operadores logicos
	case EELogico:
	case EOuLogico:
	if(tp1==BOOLEAN &&
			tp2==BOOLEAN)
		return BOOLEAN;
	break;
	}
	fprintf(stderr, "linha %d, ERRO! Nenhum tipo valido para exp binaria\n", e->lineNumber);
	return VOID;
}

TipoPrimitivo costuraExp(PilhaDeLista *pv, PilhaDeLista *pf, Exp *e){
	TipoPrimitivo tp1;
	TipoPrimitivo tp2;
	if(e == NULL){ 
			fprintf(stderr, "linha %d, ERRO! Exp nulo, retornando void\n", e->lineNumber);
			return VOID;
	}
	fprintf(stderr, "Costurando Exp \n");
	switch(e->tag){
		case EBinaria:

			return costuraExpBinaria(pv, pf, e);
		break;

		case EString:
			return STRING;
		break;
		case EBoolean:
			return BOOLEAN;
		break;
		case EInteiro:
			return INT;
		break;
		case EDouble:
			return DOUBLE;
		break;
		case EChar:
			return INT;
			/*TODO
				Nao entendi muito bem porque promover char para int
				Mas acho que vai fazer sentido mais tarde
			*/
			//return CHAR;
		break;
		case ENumeral:
			return e->u.numeral->tag;
		break;

		case EVariavel:
			return costuraVariavel(pv, pf, e->u.var);
		break;
		case EParenteses:
			return costuraExp(pv, pf, e->u.e);
		break;
		case EMenosUnario:
			return costuraExp(pv, pf, e->u.e);
		break;

		case EChamada:
			return costuraChamada(pv, pf, e->u.chamadaFuncao);
		break;
		case EAs:
			return costuraAs(pv, pf, e);
			break;
		case ENewArray:
/*
	Por enquanto so estou permitindo array
	Para arrays de arrays isso precisaria ser diferente
*/
			tp1 = e->u.newArray.tipo+ARRAY_CHAR;
			tp2 = costuraExp(pv, pf, e->u.newArray.exp);
			if(tp2 != INT){
				fprintf(stderr, "linha %d, ERRO! Nao e possivel indexar atraves do tipo %s, somente com inteiro\n", e->lineNumber, tipoPrimitivoToString(tp1));
			}
			return tp1;
		break;
	}
	fprintf(stderr, "linha %d, ERRO! Nenhum tipo valido para exp\n", e->lineNumber);
	return VOID;
}
