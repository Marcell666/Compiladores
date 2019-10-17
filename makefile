prog:         	lex.yy.c lexConfig.l lexConfig.h yaccConfig.tab.c yaccConfig.tab.h arvore.c arvore.h yaccConfig.y lib/stringSet.c lib/stringSet.h lib/pilhaDeLista.c lib/pilhaDeLista.h costura.c costura.h
		gcc -Wall -o prog lex.yy.c yaccConfig.tab.c arvore.c lib/stringSet.c lib/pilhaDeLista.c costura.c

lex.yy.c:       lexConfig.l
		flex lexConfig.l

yaccConfig.tab.c:	yaccConfig.y
			bison -d -v yaccConfig.y
yaccConfig.tab.h:	yaccConfig.y
			bison -d -v yaccConfig.y

clean:;         rm -f prog yaccConfig.tab.c yaccConfig.tab.h lex.yy.c
