all-am: lexer.h

lexer.h: lexer.l
	flex lexer.l

bin_PROGRAMS = jjc
jjc_SOURCES = jjc.c lexer.l
