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

%s COMMENT
%s STRING
%%


<INITIAL>"/*"               { BEGIN(COMMENT); }
<COMMENT>"*/"               { BEGIN(INITIAL); }
<COMMENT><<EOF>>            { error_line_column (yyline, yycolumn,
                                                 "unterminated comment");
                              return EOFnum; }
<COMMENT>\n                 { ++yyline; yycolumn = 0; }
<COMMENT>.                  { yycolumn += yyleng; }
<INITIAL>\'([^\\'])*[<<EOF>>]  {
  warning_line_column(yyline, yycolumn + yyleng,
                      "missing terminating ' character");
  return EOFnum;
}
<INITIAL>\'(\\.|[^\\'])*\'  {
  int length = yyleng;
  int i;
  int j;
  char message[34];

  for (i = 0; i < length - 1; ++i)
    {
      if (yytext[i] == '\n')
        {
          warning_line_column(yyline, yycolumn + i,
                              "missing terminating ' character");
        }
      yytext[i] = yytext[i + 1];
    }
  yytext[length - 2] = '\0';

  for (i = 0; i < strlen (yytext); ++i)
    {
      /* Handle escape sequences... */
      if (yytext[i] == '\\')
        {
          switch (yytext[i + 1])
            {
            case 'n':
              yytext[i] = '\n';
              for (j = i + 1; j < strlen (yytext); ++j)
                {
                  yytext[j] = yytext[j + 1];
                }
              yytext[j - 1] = '\0';
              break;

            case 't':
              yytext[i] = '\t';
              for (j = i + 1; j < strlen (yytext); ++j)
                {
                  yytext[j] = yytext[j + 1];
                }
              yytext[j - 1] = '\0';
              break;

            case '\'':
              yytext[i] = '\'';
              for (j = i + 1; j < strlen (yytext); ++j)
                {
                  yytext[j] = yytext[j + 1];
                }
              yytext[j - 1] = '\0';
              break;

            case '\\':
              yytext[i] = '\\';
              for (j = i + 1; j < strlen (yytext); ++j)
                {
                  yytext[j] = yytext[j + 1];
                }
              yytext[j - 1] = '\0';
              break;

            default:
              /* Print error message and continue. */
              sprintf (message, "unrecognized escape sequence `\\%c'", yytext[i + 1]);
              warning_line_column (yyline, yycolumn + i, message);
              break;
            }
        }
    }

  if (!strtbl_contains_value (string_table, yytext))
    {
      yylval = strtbl_put (string_table, yytext);
    }
  else
    {
      yylval = strtbl_get_index (string_table, yytext);
    }

  yycolumn += length;
  return SCONSTnum;
}
<INITIAL>{DELIM}            { yycolumn += yyleng; }
<INITIAL>{NEWLINE}          { yyline++; yycolumn=0; }
<INITIAL>\&\&               { yycolumn += yyleng; return ANDnum; }
<INITIAL>\:\=               { yycolumn += yyleng; return ASSGNnum; }
<INITIAL>declarations       { yycolumn += yyleng; return DECLARATIONnum; }
<INITIAL>\.                 { yycolumn += yyleng; return DOTnum; }
<INITIAL>enddeclarations    { yycolumn += yyleng; return ENDDECLARATIONnum; }
<INITIAL>\=                 { yycolumn += yyleng; return EQUALnum; }
<INITIAL>\>                 { yycolumn += yyleng; return GTnum; }
<INITIAL>int                { yycolumn += yyleng; return INTnum; }
<INITIAL>\[                 { yycolumn += yyleng; return LBRACnum; }
<INITIAL>\(                 { yycolumn += yyleng; return LPARENnum; }
<INITIAL>method             { yycolumn += yyleng; return METHODnum; }
<INITIAL>\!\=               { yycolumn += yyleng; return NEnum; }
<INITIAL>\|\|               { yycolumn += yyleng; return ORnum; }
<INITIAL>program            { yycolumn += yyleng; return PROGRAMnum; }
<INITIAL>\]                 { yycolumn += yyleng; return RBRACnum; }
<INITIAL>\)                 { yycolumn += yyleng; return RPARENnum; }
<INITIAL>\;                 { yycolumn += yyleng; return SEMInum; }
<INITIAL>val                { yycolumn += yyleng; return VALnum; }
<INITIAL>while              { yycolumn += yyleng; return WHILEnum; }
<INITIAL>class              { yycolumn += yyleng; return CLASSnum; }
<INITIAL>\,                 { yycolumn += yyleng; return COMMAnum; }
<INITIAL>\/                 { yycolumn += yyleng; return DIVIDEnum; }
<INITIAL>else               { yycolumn += yyleng; return ELSEnum; }
<INITIAL>\=\=               { yycolumn += yyleng; return EQnum; }
<INITIAL>\>\=               { yycolumn += yyleng; return GEnum; }
<INITIAL>\{                 { yycolumn += yyleng; return LBRACEnum; }
<INITIAL>\<\=               { yycolumn += yyleng; return LEnum; }
<INITIAL>\<                 { yycolumn += yyleng; return LTnum; }
<INITIAL>\-                 { yycolumn += yyleng; return MINUSnum; }
<INITIAL>\!                 { yycolumn += yyleng; return NOTnum; }
<INITIAL>\+                 { yycolumn += yyleng; return PLUSnum; }
<INITIAL>\}                 { yycolumn += yyleng; return RBRACEnum; }
<INITIAL>return             { yycolumn += yyleng; return RETURNnum; }
<INITIAL>\*                 { yycolumn += yyleng; return TIMESnum; }
<INITIAL>void               { yycolumn += yyleng; return VOIDnum; }

<INITIAL>{DIGIT}+           {
  yycolumn += yyleng;
  yylval = atoi (yytext);
  return ICONSTnum;
}
<INITIAL>{DIGIT}{ID}        {
  yycolumn += yyleng;
  char* message = malloc (18 + strlen (yytext) + 1);
  sprintf (message, "invalid identifier %s", yytext);
  error_line_column (yyline, yycolumn, message);
  free (message);
}
<INITIAL>{ID}               {
  yycolumn += yyleng;
  int i;
  for (i = 0; i < strlen (yytext); ++i)
    {
      yytext[i] = tolower(yytext[i]);
    }

  if (!strtbl_contains_value (string_table, yytext))
    {
      yylval = strtbl_put (string_table, yytext);
    }
  else
    {
      yylval = strtbl_get_index (string_table, yytext);
    }

  return IDnum;
}
<INITIAL>\:\=               {
  yycolumn += yyleng;
  error_line_column (yyline, yycolumn, "invalid operator");
}
<INITIAL>\|\&               {
  yycolumn += yyleng;
  error_line_column (yyline, yycolumn, "invalid operator");
}
<INITIAL>\&\|               {
  yycolumn += yyleng;
  error_line_column (yyline, yycolumn, "invalid operator");
}
<INITIAL>\=\<               {
  yycolumn += yyleng;
  error_line_column (yyline, yycolumn, "invalid operator");
}
<INITIAL>\=\>               {
  yycolumn += yyleng;
  error_line_column (yyline, yycolumn, "invalid operator");
}
<INITIAL><<EOF>>            { return EOFnum; }
<INITIAL>.                  { yycolumn += yyleng; }

%%

int yyline = 1;
int yycolumn = 0;
int yylval = -1;
