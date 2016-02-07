%option header-file="lexer.h"
%option outfile="lexer.c"
%option noyywrap

%{
#include <stdlib.h>
#include <string.h>
#include <tokens.h>

extern int yycolumn, yyline;
%}

DIGIT         [0-9]
ID            [\_a-zA-Z][\_a-zA-Z0-9]*
INTEGER       []
DELIM         [ \t]
WS            {delim}+
NEWLINE       [\n]


%%
\"(\\.|[^\\"])*\"  {
  int i;
  for (i = 0; i < strlen (yytext) - 1; i++)
    {
      yytext[i] = yytext[i + 1];
    }
  yytext[strlen (yytext) - 2] = '\0';
  return SCONSTnum;
}
\'(\\.|[^\\'])*\'  {
  int i;
  for (i = 0; i < strlen (yytext) - 1; i++)
    {
      yytext[i] = yytext[i + 1];
    }
  yytext[strlen (yytext) - 2] = '\0';
  return SCONSTnum;
}
"/*".*"*/"         { }
{NEWLINE}          { yyline++; yycolumn=0; }
\&\&               { return ANDnum; }
\:\=               { return ASSGNnum; }
declarations       { return DECLARATIONnum; }
\.                 { return DOTnum; }
enddeclarations    { return ENDDECLARATIONnum; }
\=                 { return EQUALnum; }
\>                 { return GTnum; }
int                { return INTnum; }
\[                 { return LBRACnum; }
\(                 { return LPARENnum; }
method             { return METHODnum; }
\!\=               { return NEnum; }
\\\\               { return ORnum; }
program            { return PROGRAMnum; }
\]                 { return RBRACnum; }
\)                 { return RPARENnum; }
\;                 { return SEMInum; }
val                { return VALnum; }
while              { return WHILEnum; }
class              { return CLASSnum; }
\,                 { return COMMAnum; }
\/                 { return DIVIDEnum; }
else               { return ELSEnum; }
\=\=               { return EQnum; }
\>\=               { return GEnum; }
\{                 { return LBRACEnum; }
\<\=               { return LEnum; }
\<                 { return LTnum; }
\-                 { return MINUSnum; }
\!                 { return NOTnum; }
\+                 { return PLUSnum; }
\}                 { return RBRACEnum; }
return             { return RETURNnum; }
\*                 { return TIMESnum; }
void               { return VOIDnum; }

{DIGIT}+           { return ICONSTnum; }
{ID}               { return IDnum; }
<<EOF>>            { return EOFnum; }
%%

int yyline = 1;
int yycolumn = 0;
