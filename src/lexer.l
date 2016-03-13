%option header-file="lexer.h"
%option outfile="lexer.c"
%option noyywrap

%{

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <parser.h>

#include <ast.h>
#include <errors.h>
#include <strtbl.h>

extern int yycolumn, yyline;
extern YYSTYPE yylval;
%}

DIGIT         [0-9]
ID            [a-zA-Z_][a-zA-Z_0-9]*
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
                              return EOF; }
<COMMENT>\n                 { ++yyline; yycolumn = 0; }
<COMMENT>.                  { yycolumn += yyleng; }
<INITIAL>\'([^\\'])*[<<EOF>>]  {
  warning_line_column(yyline, yycolumn + yyleng,
                      "missing terminating ' character");
  return EOF;
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
      yylval.constant = strtbl_put (string_table, yytext);
    }
  else
    {
      yylval.constant = strtbl_get_index (string_table, yytext);
    }

  yycolumn += length;
  return SCONST;
}
<INITIAL>{DELIM}            { yycolumn += yyleng; }
<INITIAL>{NEWLINE}          { yyline++; yycolumn=0; }
<INITIAL>\&\&               { yycolumn += yyleng; return AND; }
<INITIAL>\:\=               { yycolumn += yyleng; return ASSGN; }
<INITIAL>declarations       { yycolumn += yyleng; return DECLARATION; }
<INITIAL>\.                 { yycolumn += yyleng; return DOT; }
<INITIAL>enddeclarations    { yycolumn += yyleng; return ENDDECLARATION; }
<INITIAL>\=                 { yycolumn += yyleng; return EQUAL; }
<INITIAL>\>                 { yycolumn += yyleng; return GT; }
<INITIAL>int                { yycolumn += yyleng; return INT; }
<INITIAL>\[                 { yycolumn += yyleng; return LBRAC; }
<INITIAL>\(                 { yycolumn += yyleng; return LPAREN; }
<INITIAL>method             { yycolumn += yyleng; return METHOD; }
<INITIAL>\!\=               { yycolumn += yyleng; return NE; }
<INITIAL>\|\|               { yycolumn += yyleng; return OR; }
<INITIAL>program            { yycolumn += yyleng; return PROGRAM; }
<INITIAL>\]                 { yycolumn += yyleng; return RBRAC; }
<INITIAL>\)                 { yycolumn += yyleng; return RPAREN; }
<INITIAL>\;                 { yycolumn += yyleng; ;return SEMI; }
<INITIAL>val                { yycolumn += yyleng; return VAL; }
<INITIAL>while              { yycolumn += yyleng; return WHILE; }
<INITIAL>class              { yycolumn += yyleng; return CLASS; }
<INITIAL>\,                 { yycolumn += yyleng; return COMMA; }
<INITIAL>\/                 { yycolumn += yyleng; return DIVIDE; }
<INITIAL>else               { yycolumn += yyleng; return ELSE; }
<INITIAL>\=\=               { yycolumn += yyleng; return EQ; }
<INITIAL>\>\=               { yycolumn += yyleng; return GE; }
<INITIAL>\{                 { yycolumn += yyleng; return LBRACE; }
<INITIAL>\<\=               { yycolumn += yyleng; return LE; }
<INITIAL>\<                 { yycolumn += yyleng; return LT; }
<INITIAL>\-                 { yycolumn += yyleng; return MINUS; }
<INITIAL>\!                 { yycolumn += yyleng; return NOT; }
<INITIAL>\+                 { yycolumn += yyleng; return PLUS; }
<INITIAL>\}                 { yycolumn += yyleng; return RBRACE; }
<INITIAL>return             { yycolumn += yyleng; return RETURN; }
<INITIAL>\*                 { yycolumn += yyleng; return TIMES; }
<INITIAL>void               { yycolumn += yyleng; return VOID; }

<INITIAL>{DIGIT}+           {
  yycolumn += yyleng;
  yylval.constant = atoi (yytext);
  return ICONST;
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
      yytext[i] = tolower (yytext[i]);
    }

  if (!strtbl_contains_value (string_table, yytext))
    {
      yylval.constant = strtbl_put (string_table, yytext);
    }
  else
    {
      yylval.constant = strtbl_get_index (string_table, yytext);
    }

  return ID;
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
<INITIAL><<EOF>>            { return EOF; }

%%

int yyline = 1;
int yycolumn = 0;
YYSTYPE yylval;
