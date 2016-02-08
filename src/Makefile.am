all-am: lexer.h

lexer.h: lexer.l
	flex lexer.l

bin_PROGRAMS = $(top_builddir)/bin/jjc
__top_builddir__bin_jjc_SOURCES = errors.c jjc.c lexer.l strtbl.c
