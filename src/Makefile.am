BUILT_SOURCES = lexer.h parser.h

lexer.h: lexer.l
	$(LEX) lexer.l

parser.h: parser.y
	$(YACC) parser.y

bin_PROGRAMS = $(top_builddir)/bin/jjc
__top_builddir__bin_jjc_SOURCES = ast.c errors.c jjc.c lexer.l strtbl.c parser.y
