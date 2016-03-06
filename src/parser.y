%file-prefix "parser"
%defines "parser.h"

%{
  #include <stdio.h>
  #include <stdlib.h>

  #include <ast.h>
%}

%token <constant> AND
%token <constant> ASSGN
%token <constant> DECLARATION
%token <constant> DOT
%token <constant> ENDDECLARATION
%token <constant> EQUAL
%token <constant> GT
%token <constant> ID
%token <constant> INT
%token <constant> LBRAC
%token <constant> LPAREN
%token <constant> METHOD
%token <constant> NE
%token <constant> OR
%token <constant> PROGRAM
%token <constant> RBRAC
%token <constant> RPAREN
%token <constant> SEMI
%token <constant> VAL
%token <constant> WHILE
%token <constant> CLASS
%token <constant> COMMA
%token <constant> DIVIDE
%token <constant> ELSE
%token <constant> EQ
%token <constant> GE
%token <constant> ICONST
%token <constant> IF
%token <constant> LBRACE
%token <constant> LE
%token <constant> LT
%token <constant> MINUS
%token <constant> NOT
%token <constant> PLUS
%token <constant> RBRACE
%token <constant> RETURN
%token <constant> SCONST
%token <constant> TIMES
%token <constant> VOID
%token <constant> EOF

%type <tree> PROGRAM
%type <tree> CLASS_DECLARATION
%type <tree> VARIABLE

%%

PROGRAM : PROGRAM ID SEMI CLASS_DECLARATION
          {
            $$ = ast_new (PROGRAMOP,
                          $4,
                          ast_make_leaf (IDNODE, $2));
            printtree ($$, 0);
          }
  ;

EXPRESSION : SIMPLE_EXPRESSION
             {
               $$ = $1;
             }
           | SIMPLE_EXPRESSION COMPARISON_OPERATOR SIMPLE_EXPRESSION
             {
               ast_set_left_subtree ($1, $2);
               $$ = ast_set_right_subtree ($3, $2);
             }

%%
