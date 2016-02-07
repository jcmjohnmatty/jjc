%option header-file="lexer.h"
%option outfile="lexer.c"
%option noyywrap

%{

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <tokens.h>

#include <strtbl.h>

extern int yycolumn, yyline, yylval;

%}

DIGIT         [0-9]
ID            [\_a-zA-Z][\_a-zA-Z0-9]*
INTEGER       []
DELIM         [ \t]
WS            {delim}+
NEWLINE       [\n]

%%

\'(\\.|[^\\'])*\'  {
  yycolumn += yyleng;

  int i;
  for (i = 0; i < strlen (yytext) - 1; i++)
    {
      yytext[i] = yytext[i + 1];
    }
  yytext[strlen (yytext) - 2] = '\0';

  if (!strtbl_contains_value (string_table, yytext))
    {
      yylval = strtbl_put (string_table, yytext);
    }

  return SCONSTnum;
}
"/*".*"*/"         { }
{DELIM}            { yycolumn += yyleng; }
{NEWLINE}          { yyline++; yycolumn=0; }
\&\&               { yycolumn += yyleng; return ANDnum; }
\:\=               { yycolumn += yyleng; return ASSGNnum; }
declarations       { yycolumn += yyleng; return DECLARATIONnum; }
\.                 { yycolumn += yyleng; return DOTnum; }
enddeclarations    { yycolumn += yyleng; return ENDDECLARATIONnum; }
\=                 { yycolumn += yyleng; return EQUALnum; }
\>                 { yycolumn += yyleng; return GTnum; }
int                { yycolumn += yyleng; return INTnum; }
\[                 { yycolumn += yyleng; return LBRACnum; }
\(                 { yycolumn += yyleng; return LPARENnum; }
method             { yycolumn += yyleng; return METHODnum; }
\!\=               { yycolumn += yyleng; return NEnum; }
\|\|               { yycolumn += yyleng; return ORnum; }
program            { yycolumn += yyleng; return PROGRAMnum; }
\]                 { yycolumn += yyleng; return RBRACnum; }
\)                 { yycolumn += yyleng; return RPARENnum; }
\;                 { yycolumn += yyleng; return SEMInum; }
val                { yycolumn += yyleng; return VALnum; }
while              { yycolumn += yyleng; return WHILEnum; }
class              { yycolumn += yyleng; return CLASSnum; }
\,                 { yycolumn += yyleng; return COMMAnum; }
\/                 { yycolumn += yyleng; return DIVIDEnum; }
else               { yycolumn += yyleng; return ELSEnum; }
\=\=               { yycolumn += yyleng; return EQnum; }
\>\=               { yycolumn += yyleng; return GEnum; }
\{                 { yycolumn += yyleng; return LBRACEnum; }
\<\=               { yycolumn += yyleng; return LEnum; }
\<                 { yycolumn += yyleng; return LTnum; }
\-                 { yycolumn += yyleng; return MINUSnum; }
\!                 { yycolumn += yyleng; return NOTnum; }
\+                 { yycolumn += yyleng; return PLUSnum; }
\}                 { yycolumn += yyleng; return RBRACEnum; }
return             { yycolumn += yyleng; return RETURNnum; }
\*                 { yycolumn += yyleng; return TIMESnum; }
void               { yycolumn += yyleng; return VOIDnum; }

{DIGIT}+           {
  yycolumn += yyleng;
  yylval = atoi (yytext);
  return ICONSTnum;
}
{ID}               {
  yycolumn += yyleng;
  int i;
  for (i = 0; i < strlen (yytext); ++i)
    {
      yylval = tolower(yytext[i]);
    }

  if (!strtbl_contains_value (string_table, yytext))
    {
      yylval = strtbl_put (string_table, yytext);
    }

  return IDnum;
}
<<EOF>>            { return EOFnum; }
.                  { yycolumn += strlen (yytext); }
%%

int yyline = 1;
int yycolumn = 0;
int yylval = -1;
