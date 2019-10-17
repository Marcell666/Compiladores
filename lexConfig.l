%{

#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include "lexConfig.h"
#include "yaccConfig.tab.h"
int lineNumber;
%}

ASPAS	\\\"
DIGIT	[0-9]
LETTER	[a-zA-Z]
C	[#]
S	[ ]
L	[\n]
SS	[\t|\"|\\]
HEX	{DIGIT}|[a-fA-F]
ID	{LETTER}({LETTER}|{DIGIT}|_)*
double	{DIGIT}+(\.{DIGIT}+)?([Ee][+-]?{DIGIT}+)?
hexInt	0[xX]{HEX}+
hexDouble	(0[xX]){HEX}+(\.{HEX}+)?(p[+-]?{HEX}*)?
String 	\"({ASPAS}|[^"\n])*["\n]
Sym 	[~!@#$%^&*()_+|><"'{}.,?:]
comentario	({C}+({LETTER}|{DIGIT}|{Sym}|{S}|{SS})*{L})
PRINT		[@]

%%

{comentario} {
	fprintf(stderr, "Comentario:|%s|\n",yytext);
	//return TK_COMENTARIO;
}

{DIGIT}+ {
	fprintf(stderr, "Inteiro:%s(%d)\n",yytext,atoi(yytext));
	yylval.ival = atoi(yytext);
	return TK_INTEIRO;
}

{double} {
	fprintf(stderr, "Double:%s(%lg)\n",yytext,atof(yytext));
	yylval.dval = atof(yytext);
	return TK_DOUBLE;
}

{hexInt} {
	fprintf(stderr, "Inteiro Hexadecimal:%s\n",yytext);
	yylval.ival = atoi(yytext);
	return TK_INTEIRO_HEX;
}

{hexDouble} {
	fprintf(stderr, "Double Hexadecimal:%s\n",yytext);
	yylval.dval = atof(yytext);
	return TK_DOUBLE_HEX;
}


{String} {
	fprintf(stderr, "String:%s\n",yytext);
	strcpy(yylval.sval, yytext);
	return TK_STRING;
}


as|AS		{fprintf(stderr, "Palavra-Chave: %s\n", yytext); return TK_AS;}
bool|BOOL	{fprintf(stderr, "Palavra-Chave: %s\n", yytext); return TK_BOOL;}
char|CHAR	{fprintf(stderr, "Palavra-Chave: %s\n", yytext); return TK_CHAR;}
else|ELSE	{fprintf(stderr, "Palavra-Chave: %s\n", yytext); return TK_ELSE;}
true|TRUE	{fprintf(stderr, "Palavra-Chave: %s\n", yytext); return TK_TRUE;}
false|FALSE	{fprintf(stderr, "Palavra-Chave: %s\n", yytext); return TK_FALSE;}
float|FLOAT	{fprintf(stderr, "Palavra-Chave: %s\n", yytext); return TK_FLOAT;}
if|IF		{fprintf(stderr, "Palavra-Chave: %s\n", yytext); return TK_IF;}
int|INT		{fprintf(stderr, "Palavra-Chave: %s\n", yytext); return TK_INT;}
new|NEW		{fprintf(stderr, "Palavra-Chave: %s\n", yytext); return TK_NEW;}
return|RETURN	{fprintf(stderr, "Palavra-Chave: %s\n", yytext); return TK_RETURN;}
while|WHILE	{fprintf(stderr, "Palavra-Chave: %s\n", yytext); return TK_WHILE;}

{ID} {
	fprintf(stderr, "Identificador: %s\n", yytext);
	strcpy(yylval.sval, yytext);
	return TK_ID;
}

"@"	{fprintf(stderr, "Operador: %s\n", yytext); return TK_ARROBA;}
"+"	{fprintf(stderr, "Operador: %s\n", yytext); return TK_MAIS;}
"-"	{fprintf(stderr, "Operador: %s\n", yytext); return TK_MENOS;}
"*"	{fprintf(stderr, "Operador: %s\n", yytext); return TK_MULTIPLICAR;}
"/"	{fprintf(stderr, "Operador: %s\n", yytext); return TK_DIVIDIR;}
"="	{fprintf(stderr, "Operador: %s\n", yytext); return TK_IGUAL;}
"=="	{fprintf(stderr, "Operador: %s\n", yytext); return TK_DOIS_IGUAIS;}
"<"	{fprintf(stderr, "Operador: %s\n", yytext); return TK_MENOR;}
"<="	{fprintf(stderr, "Operador: %s\n", yytext); return TK_MENOR_IGUAL;}
">"	{fprintf(stderr, "Operador: %s\n", yytext); return TK_MAIOR;}
">="	{fprintf(stderr, "Operador: %s\n", yytext); return TK_MAIOR_IGUAL;}
"~="	{fprintf(stderr, "Operador: %s\n", yytext); return TK_DIFERENTE_COMP;}
"&&"	{fprintf(stderr, "Operador: %s\n", yytext); return TK_E_LOGICO;}
"||"	{fprintf(stderr, "Operador: %s\n", yytext); return TK_OU_LOGICO;}
"!"	{fprintf(stderr, "Operador: %s\n", yytext); return TK_DIFERENTE;}
":"	{fprintf(stderr, "Operador: %s\n", yytext); return TK_DOIS_PONTOS;}
";"	{fprintf(stderr, "Operador: %s\n", yytext); return TK_PONTO_VIRGULA;}
","	{fprintf(stderr, "Operador: %s\n", yytext); return TK_VIRGULA;}
"."	{fprintf(stderr, "Operador: %s\n", yytext); return TK_PONTO;}
"("	{fprintf(stderr, "Operador: %s\n", yytext); return TK_PARENTESES_ESQ;}
")"	{fprintf(stderr, "Operador: %s\n", yytext); return TK_PARENTESES_DIR;}
"["	{fprintf(stderr, "Operador: %s\n", yytext); return TK_COLCHETES_ESQ;}
"]"	{fprintf(stderr, "Operador: %s\n", yytext); return TK_COLCHETES_DIR;}
"{"	{fprintf(stderr, "Operador: %s\n", yytext); return TK_CHAVES_ESQ;}
"}"	{fprintf(stderr, "Operador: %s\n", yytext); return TK_CHAVES_DIR;}

"{"[^}\n]*"}"

[ \t]+

\n	{lineNumber++;}

<<EOF>> {fprintf(stderr, "Fim Do Arquivo(de fato) %s\n", yytext); return 0;} 

. {/*printf("Nao reconhecido: %s\n", yytext);*/ return TK_NAO_RECONHECIDO;}

%%
/*
main(argc, argv)

	int argc;

	char **argv;
	{
		a=0;
		++argv, --argc; 
		if(argc>0)
			yyin = fopen(argv, "r");
		else
			yyin = stdin;
		while(1)
			yylex();
	}
*/
int yywrap(){
	fprintf(stdout, "END OF FILE!!!\n");
	return 1;
}
