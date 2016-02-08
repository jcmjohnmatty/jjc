all-am: lexer.h

lexer.h: lexer.l
	flex lexer.l

bin_PROGRAMS = jjc
jjc_SOURCES = errors.c jjc.c lexer.l strtbl.c
