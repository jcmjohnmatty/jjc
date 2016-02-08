%option header-file="lexer.h"
%option outfile="lexer.c"
%option noyywrap

%{

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <tokens.h>

#include <errors.h>
#include <strtbl.h>

extern int yycolumn, yyline, yylval;

%}

DIGIT         [0-9]
ID            [\_a-zA-Z][\_a-zA-Z0-9]*
INTEGER       []
DELIM         [ \t]
NEWLINE       [\n]

%%

"/*".*"*/"         { }
\'(\\.|[^\\'])*\'  {
  int length = yyleng;
  int i;
  int j;
  int diff = 2;
  for (i = 0; i < length - diff + 1; ++i)
    {
      /* Handle escape sequences... */
      if (yytext[i] == '\\')
        {
          switch (yytext[i + 1])
            {
            case 'n':
              yytext[i] = '\n';
              break;

            case 't':
              yytext[i] = '\t';
              break;

            case '\'':
              yytext[i] = '\'';
              break;

            case '\\':
              yytext[i] = '\\';
              break;

            default:
              /* Print error message and continue. */
              char message[34];
              sprintf (message, "unrecognized escape sequence `\\%c'", yytext[i + 1]);
              warning_line_column (yyline, i - (diff - 2), message);
              --i;
              --diff;
              break;
            }
          for (j = i + 2; j < length - diff + 1; ++j)
            {
              yytext[j - 1] = yytext[j + 1];
            }
          ++diff;
        }
      else
        {
          yytext[i] = yytext[i + 1];
        }
    }
  yytext[length - diff] = '\0';

  if (!strtbl_contains_value (string_table, yytext))
    {
      yylval = strtbl_put (string_table, yytext);
    }

  yycolumn += length;
  return SCONSTnum;
}
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
.                  { yycolumn += yyleng; }
%%

int yyline = 1;
int yycolumn = 0;
int yylval = -1;
