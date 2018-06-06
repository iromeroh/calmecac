.PHONY: default
default: calmecac ;
CC=gcc
LEX=flex
YACC=bison
CFLAGS=
ODIR=./
INCDIR=./
TESTDIR=./test
PROG_NAME=calmecac
BINDIR=.

lex.yy.c:
	$(LEX) lexer.l
	
grammar.tab.c:
	$(YACC) -d grammar.y

calmecac: lex.yy.c grammar.tab.c
	$(CC) -o $(BINDIR)/$(PROG_NAME) lex.yy.c grammar.tab.c parser.c bst.c $(CFLAGS) $(LIBS)
	
clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ lex.yy.c grammar.tab.c grammar.tab.h $(PROG_NAME)
