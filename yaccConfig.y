%{

#include<math.h>
#include<stdio.h>
#include<stdlib.h>
//#include "arvore.h"
#include "yaccConfig.tab.h"
#include "lexConfig.h"
#include "costura.h"
int lineNumber=1;
int yyerror(Arvore **a, char *s );
int yylex(void);
//#define MAX_ID 81 TODO

%}


%code requires {
  #include "arvore.h"
}


%parse-param {Arvore **a}
%union{
	char sval[81];
	int ival;
	double dval;
	char cval;


	Arvore* arvore;
	Numeral* numeral;
	Exp* exp;
	//Programa* programa;
	Definicao* definicao;
	Bloco* bloco;
	Comando* comando;
	ChamadaArg* chamadaArg;
	Chamada* chamada;
	Exp* expressao;
	TipoPrimitivo tipo;
	Variavel* variavel;

}

//----------------------------------

%type <arvore> programa;
%type <definicao> definicao-lista;
%type <definicao> definicao-cauda;
%type <definicao> definicao;
%type <definicao> def-variavel;
%type <tipo> tipo;
%type <definicao> def-funcao;
%type <tipo> tipo-opcional;
%type <definicao> parametro-lista;
%type <definicao> parametro-cauda;
%type <definicao> parametro;
%type <bloco> bloco;
%type <definicao> def-variavel-lista;
%type <comando> comando-lista;
%type <comando> comando;
%type <bloco> else-opcional;
%type <variavel> var;
%type <numeral> numeral;

%type <expressao> expP;
%type <expressao> expSinal;
%type <expressao> expMult;
%type <expressao> expAdd;
%type <expressao> expComp;
%type <expressao> expCompELogico
%type <expressao> exp;

%type <expressao> exp-opcional;
%type <chamada> chamada;
%type <chamadaArg> lista-exp;
%type <chamadaArg> exp-lista-cauda;

//--------------------

%token	TK_DOIS_IGUAIS TK_MENOR TK_MENOR_IGUAL TK_MAIOR TK_MAIOR_IGUAL TK_DIFERENTE_COMP 
%token	TK_OU_LOGICO 
%token	TK_E_LOGICO
%token	TK_MAIS
%token	TK_MENOS
%token	TK_MULTIPLICAR
%token	TK_DIVIDIR
%token	TK_DIFERENTE
%token	TK_DOIS_PONTOS TK_PONTO_VIRGULA TK_VIRGULA TK_PONTO

%token	TK_IGUAL
%token	TK_ARROBA

%token	TK_NAO_RECONHECIDO

%token	TK_COMENTARIO
%token	<ival>	TK_INTEIRO
%token	<dval>	TK_DOUBLE
%token	<ival>	TK_INTEIRO_HEX
%token	<dval>	TK_DOUBLE_HEX
%token	<sval>	TK_STRING
%token	TK_AS
%token	<ival>	TK_BOOL
%token	<cval>	TK_CHAR
%token	TK_ELSE
%token	<ival>	TK_TRUE
%token	<ival>	TK_FALSE
%token	TK_FLOAT
%token	TK_IF
%token	TK_INT
%token	TK_NEW
%token	TK_RETURN
%token	TK_WHILE

%token	<sval>	TK_ID
%token	TK_MENOS_UNARIO

%token	TK_PARENTESES_ESQ TK_PARENTESES_DIR TK_COLCHETES_ESQ TK_COLCHETES_DIR TK_CHAVES_ESQ TK_CHAVES_DIR

%start programa

%%

/*
S -> AaA
A -> b
  -> €




S -> A
  -> aA 
A -> b
  -> €
*/

//expAdd + expAdd == expAdd + expAdd

programa:	/*empty*/{
			fprintf(stderr, "programa vazio\n");
			*a = NULL;
			//$$ = NULL;
		}
		|definicao-lista{
			fprintf(stderr, "programa completo\n");
			$$ = ARV_cria($1);
			*a = ARV_cria($1);
		}
;
definicao-lista:definicao-cauda definicao{
			fprintf(stderr, "definicao-lista\n");
			$$ = ARV_insereDefinicao($2, $1);
		}
;
definicao-cauda:/*empty*/{
			$$ = NULL;
		}
		|definicao-cauda definicao{
			$$ = ARV_insereDefinicao($2, $1);
		}
;

definicao:	def-variavel
		{$$ = $1;}
		| def-funcao
		{$$ = $1;}
;

def-variavel:  TK_ID TK_DOIS_PONTOS tipo TK_PONTO_VIRGULA{
		fprintf(stderr, "definicao de variavel\n");
		fprintf(stderr, "definicao de variavel %s %s\n", $1, tipoPrimitivoToString($3));
		$$ = ARV_criaDefinicaoVariavel(lineNumber, NULL, $1, $3);
}
;

def-funcao:	TK_ID TK_PARENTESES_ESQ parametro-lista	 TK_PARENTESES_DIR tipo-opcional bloco{
			fprintf(stderr, "definicao de funcao \n");
			$$ = ARV_criaDefinicaoFuncao(lineNumber, NULL, $1, $3, $5, $6);
		}
;
tipo-opcional: /*empty*/{
			$$=VOID;
		}
		|TK_DOIS_PONTOS tipo{
			$$=$2;
		}
;
parametro-lista : /*empty*/{
			$$=NULL;
			fprintf(stderr, "sem parametros\n");
		}
			| parametro-cauda parametro{
			$$ = ARV_insereDefinicao($2, $1);
			fprintf(stderr, "parametros\n");
		}
;
parametro-cauda :  /*empty*/{
			$$=NULL;
		}
		| parametro-cauda parametro TK_VIRGULA{
			$$ =ARV_insereDefinicao($2, $1);
		}
;




// A versão abaixo me parece melhor, mas por alguma razao sobrenatural, a de cima e que funciona


//parametro-lista:  /*empty*/{
//			$$=NULL;
//		}
//		|parametro-cauda parametro{
//			$$ = ARV_insereDefinicao($2, $1);
//		}
//;
//parametro-cauda:/*empty*/{
//			$$=NULL;
//		}
//		|parametro-lista parametro TK_VIRGULA{
//			$$ =ARV_insereDefinicao($2, $1);
//		}
//;

parametro : TK_ID TK_DOIS_PONTOS tipo{
		fprintf(stderr, "parametro\n");
		$$ = ARV_criaDefinicaoParametro(lineNumber, NULL, $1, $3);
	}
;

bloco : TK_CHAVES_ESQ def-variavel-lista comando-lista TK_CHAVES_DIR{
	fprintf(stderr, "bloco\n");
	$$ = ARV_criaBloco(lineNumber, $2, $3);
}
;

def-variavel-lista: /*empty*/{
			$$ = NULL;
		}
		|def-variavel-lista def-variavel {
			$$ = ARV_insereDefinicao($2, $1);
		/*sera que estou invertendo a ordem que as variaveis sao lidos?
		Isso pode ser um problema na chamada de uma funcao?*/

		}
;
comando-lista:	/*empty*/{
			$$ = NULL;
		}
		//Se eu trocar a ordem desses dois aparecem dois conflitos de shift reduce
		|comando comando-lista{
			$$ = ARV_insereComando($1, $2);
		}
;

comando: TK_IF exp bloco else-opcional{
		fprintf(stderr, "comando if\n");
		$$ = ARV_criaComandoIf(lineNumber,  NULL, $2, $3, $4);
	}
        | TK_WHILE exp bloco{
		fprintf(stderr, "comando while\n");
		$$ = ARV_criaComandoWhile(lineNumber,  NULL, $2, $3);
	}
        | var TK_IGUAL exp TK_PONTO_VIRGULA{
		fprintf(stderr, "comando atribuicao\n");
		$$ = ARV_criaComandoAtribuicao(lineNumber,  NULL, $1, $3);
	}
        | TK_RETURN exp-opcional TK_PONTO_VIRGULA{
		fprintf(stderr, "comando return\n");
		$$ = ARV_criaComandoReturn(lineNumber,  NULL, $2);
	}
        | chamada TK_PONTO_VIRGULA{
		fprintf(stderr, "comando chamada\n");
		$$ = ARV_criaComandoChamada(lineNumber,  NULL, $1);
	}
	| TK_ARROBA exp TK_PONTO_VIRGULA{
		fprintf(stderr, "comando print\n");
		$$ = ARV_criaComandoArroba(lineNumber,  NULL, $2);
	}
	| bloco{
		fprintf(stderr, "comando bloco\n");
		$$ = ARV_criaComandoBloco(lineNumber,  NULL, $1);
	}
;
else-opcional: /*empty*/{
			 $$ = ARV_criaBloco(lineNumber, 	NULL, NULL);
		}|TK_ELSE bloco{
			$$ = $2;
		}
;
exp-opcional: /*empty*/{
			$$ = NULL;
		}
		|exp{
			$$ = $1;
		}
;

tipo:	TK_INT {
		fprintf(stderr, "tipo int \n");
		$$ = INT;
	}
	| TK_CHAR {
		fprintf(stderr, "tipo char \n");
		$$ = CHAR;
	}
	| TK_FLOAT {
		fprintf(stderr, "tipo float \n");
		$$ = DOUBLE;
	}
	| TK_BOOL {
		fprintf(stderr, "tipo bool \n");
		$$ = BOOLEAN;
	}
	| TK_COLCHETES_ESQ tipo TK_COLCHETES_DIR{
		$$ = ARV_criaTipoArray($2);
	}
;

var: 	TK_ID {
		fprintf(stderr, "var id\n");
		$$ = ARV_criaVariavelId(lineNumber, $1);
	}
//TODO consertar isso!!
	| TK_ID TK_COLCHETES_ESQ exp TK_COLCHETES_DIR{
//	| expP TK_COLCHETES_ESQ exp TK_COLCHETES_DIR{
		Variavel *v;
		Exp *e;
		fprintf(stderr, "var array\n");
		v = ARV_criaVariavelId(lineNumber, $1);
		e = ARV_criaExpVariavel(lineNumber, v);		
			
		$$ = ARV_criaVariavelArray(lineNumber, e, $3);
	}

/*
	TODO Isto está correto?
	Se eu colocar exp ao invés de expP ocorrem ~80 conflitos
	Acho que não faz mesmo sentido permitir um expressão complexa aqui
	Entretanto talvez devesse incluir mais opcoes

	Realmente nao entendi como generalizar a variavel para expressao. Como posso trabalhar com isso?

*/
;

numeral: TK_INTEIRO{
		fprintf(stderr, "numeral INTEIRO:%d\n", $1);
		$$=ARV_criaNumeralInteiro(lineNumber, $1);
	}
	|TK_DOUBLE{
		fprintf(stderr, "numeral DOUBLE:%lf\n", $1);
		$$=ARV_criaNumeralDouble(lineNumber, $1);
	}
	|TK_INTEIRO_HEX{
		fprintf(stderr, "numeral INTEIRO HEX:%d\n", $1);
		$$=ARV_criaNumeralInteiro(lineNumber, $1);
	}
	|TK_DOUBLE_HEX{
		fprintf(stderr, "numeral DOUBLE HEX:%lf\n", $1);
		$$=ARV_criaNumeralDouble(lineNumber, $1);
	}
;
expP:numeral {
		$$ = ARV_criaExpNumeral(lineNumber, $1);
	}
	| TK_STRING{
		fprintf(stderr, "expressao string\n");
		$$ = ARV_criaExpString(lineNumber, $1);
	}
	| TK_TRUE{
		fprintf(stderr, "expressao boolean true\n");
		$$ = ARV_criaExpBoolean(lineNumber, $1);
	}
	| TK_FALSE{
		fprintf(stderr, "expressao boolean false\n");
		$$ = ARV_criaExpBoolean(lineNumber, $1);
	}
	| var{
		fprintf(stderr, "expressao var\n");
		$$ = ARV_criaExpVariavel(lineNumber, $1);
	}
	| TK_PARENTESES_ESQ exp TK_PARENTESES_DIR{
		fprintf(stderr, "expressao parenteses\n");
		$$ = ARV_criaExpParenteses(lineNumber, $2);
	}

	
;

expSinal: expP{
		fprintf(stderr, "expressao sinal\n");
		$$ = $1;
	}
	| TK_MENOS expP{
		fprintf(stderr, "expressao sinal menos unario\n");
		$$ = ARV_criaExpMenosUnario(lineNumber, $2);
	}
	| chamada{
		fprintf(stderr, "expressao chamada\n");
		$$ = ARV_criaExpChamada(lineNumber, $1);
	}
	| expP TK_AS tipo{
		fprintf(stderr, "expressao as\n");
		$$ = ARV_criaExpAs(lineNumber, $1, $3);
	}
	| TK_NEW tipo TK_COLCHETES_ESQ exp TK_COLCHETES_DIR{
		fprintf(stderr, "expressao new array\n");
		$$ = ARV_criaExpNewArray(lineNumber, $2, $4);
	}

//	TODO				Precisa disso??
//	| TK_MAIS expSinal
//		{
//		fprintf(stderr, "expressao sinal mais\n");
//		$$ = $1;
//	}
;

expMult: expSinal{
		$$ = $1;
	}
	| expMult TK_MULTIPLICAR expSinal{
		fprintf(stderr, "expressao multiplicacao\n");
		$$ = ARV_criaExpBinaria(lineNumber, EMult, $1, $3);
	}
	| expMult TK_DIVIDIR expSinal{
		fprintf(stderr, "expressao multiplicacao\n");
		$$ = ARV_criaExpBinaria(lineNumber, EDiv, $1, $3);
	}
;
expAdd: expMult{
		$$ = $1;
	}
	| expAdd TK_MAIS expMult{
		fprintf(stderr, "expressao soma\n");
		$$ = ARV_criaExpBinaria(lineNumber, ESoma, $1, $3);
	}
	| expAdd TK_MENOS expMult{
		fprintf(stderr, "expressao subtracao\n");
		$$ = ARV_criaExpBinaria(lineNumber, ESub, $1, $3);
	}
;
expComp:expAdd{
		$$ = $1;
	}
	| expAdd TK_DOIS_IGUAIS expAdd{
		fprintf(stderr, "expressao comparacao\n");
		$$ = ARV_criaExpBinaria(lineNumber, EDois_Iguais, $1, $3);
	}
	| expAdd TK_DIFERENTE_COMP expAdd{
		fprintf(stderr, "expressao comparacao\n");
		$$ = ARV_criaExpBinaria(lineNumber, EDiferente_Comp, $1, $3);
	}
	| expAdd TK_MENOR_IGUAL expAdd{
		fprintf(stderr, "expressao comparacao\n");
		$$ = ARV_criaExpBinaria(lineNumber, EMenorIgual, $1, $3);
	}
	| expAdd TK_MAIOR_IGUAL expAdd{
		fprintf(stderr, "expressao comparacao\n");
		$$ = ARV_criaExpBinaria(lineNumber, EMaiorIgual, $1, $3);
	}
	| expAdd TK_MENOR expAdd{
		fprintf(stderr, "expressao comparacao\n");
		$$ = ARV_criaExpBinaria(lineNumber, EMenor, $1, $3);
	}
	| expAdd TK_MAIOR expAdd{
		fprintf(stderr, "expressao comparacao\n");
		$$ = ARV_criaExpBinaria(lineNumber, EMaior, $1, $3);
	}
;

expCompELogico: expComp{
		$$ = $1;
		fprintf(stderr, "expressao identidade\n");
	}
	|expCompELogico TK_E_LOGICO expComp{
		fprintf(stderr, "expressao Logica OU\n");
		$$ = ARV_criaExpBinaria(lineNumber, EELogico, $1, $3);
	}
;

exp: expCompELogico{
	$$ = $1;
		fprintf(stderr, "expressao identidade\n");
	}
	|exp TK_OU_LOGICO expCompELogico{
		fprintf(stderr, "expressao Logica E\n");
		$$ = ARV_criaExpBinaria(lineNumber, EOuLogico, $1, $3);
	}
;

chamada: TK_ID TK_PARENTESES_ESQ lista-exp TK_PARENTESES_DIR{
		fprintf(stderr, "chamada de funcao\n");
		$$ = ARV_criaChamada(lineNumber, $1, $3);
}
;

lista-exp: 	 /*empty*/{
			$$ = ARV_criaChamadaArg(lineNumber, NULL, NULL);
		}
		|exp-lista-cauda exp{
			$$ = ARV_criaChamadaArg(lineNumber, $2, $1);		
		}
		

;
exp-lista-cauda:  /*empty*/{
			$$ = ARV_criaChamadaArg(lineNumber, NULL, NULL);		
		}
		|exp-lista-cauda exp TK_VIRGULA{
			$$ = ARV_criaChamadaArg(lineNumber, $2, $1);		
		}
;
%%
int yyerror(Arvore **a, char *s ){
	fprintf( stderr ,"--> ERRO! %s\n" , s );
	return 1;
}
int main(void){
	Arvore *a = NULL;
	yyparse(&a);
	if(a==NULL){
		fprintf(stderr,"Erro ao recuperar arvore de sintaxe\n");
		exit(1);
	}
	ARV_exibe(a);
	CTR_verificaVariaveis(a);
	fprintf(stderr, "deve ter dado tudo certo");
	//TPG_verificaTipos(a);
	


		//printf("\nlineNumber: %d\n", lineNumber);
		//printf("yychar: |%d|\n", (int) yychar);
	return 0;
}

