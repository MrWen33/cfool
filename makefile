.PHONY:clean
cfool:y.tab.o AST.o
	g++ -g y.tab.o AST.o -o cfool

y.tab.o:y.tab.c lex.yy.c
	g++ -g -c y.tab.c -o y.tab.o

y.tab.c:cfool.y
	yacc -d cfool.y

lex.yy.c:cfool.l
	lex cfool.l

clean:
	rm -f *.o cfool lex.yy.c y.tab.c y.tab.h