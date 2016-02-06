%option header-file="lex.yy.h"
%option outfile="lex.yy.c"
%option noyywrap

%{
#include <stdlib.h>
#include <tokens.h>

extern int yycolumn, yylength, yyline, yylval;
%}

DIGIT         [0-9]
ID            [\_a-zA-Z][\_a-zA-Z0-9]*
INTEGER       []
DELIM         [ \t]
WS            {delim}+
NEWLINE       [\n]

%%
{NEWLINE}           {yyline++; yycolumn=0;}
\&\&                { return ANDnum; }
\:\=                { return ASSGNnum; }
declarations        { return DECLARATIONnum; }
"."                 { return DOTnum; }
enddeclarations     { return ENDDECLARATIONnum; }
"="                 { return EQUALnum; }
">"                 { return GTnum; }
int                 { return INTnum; }
"["                 { return LBRACnum; }
"("                 { return LPARENnum; }
method              { return METHODnum; }
"!="                { return NEnum; }
"\\"                { return ORnum; }
program             { return PROGRAMnum; }
"]"                 { return RBRACnum; }
")"                 { return RPARENnum; }
";"                 { return SEMInum; }
val                 { return VALnum; }
while               { return WHILEnum; }
class               { return CLASSnum; }
","                 { return COMMAnum; }
"/"                 { return DIVIDEnum; }
else                { return ELSEnum; }
"=="                { return EQnum; }
">="                { return GEnum; }
"{"                 { return LBRACEnum; }
"<="                { return LEnum; }
"<"                 { return LTnum; }
"-"                 { return MINUSnum; }
"!"                 { return NOTnum; }
"+"                 { return PLUSnum; }
"}"                 { return RBRACEnum; }
return              { return RETURNnum; }
"*"                 { return TIMESnum; }
void                { return VOIDnum; }

{DIGIT}+            { return ICONSTnum; }
"\""[^\n]*"\""          { return SCONSTnum; }
{ID}                { return IDnum; }

%%

int yyline = 0;
int yycolumn = 0;
