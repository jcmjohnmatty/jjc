%file-prefix "parser"
%defines "parser.h"

%code requires
{
  %include <ast.h>
}

%{
  #include <stdio.h>
  #include <stdlib.h>
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
%type <tree> CLASS_BODY
%type <tree> DECLARATIONS
%type <tree> VARIABLE_DECLARATION
%type <tree> VARIABLE_DECLARATION_ID
%type <tree> VARIABLE_INITIALIZER
%type <tree> ARRAY_INITALIZER
%type <tree> ARRAY_CREATION_EXPRESSION
%type <tree> METHOD_DECLARATION
%type <tree> FORMAL_PARAMETER_LIST
%type <tree> BLOCK
%type <tree> TYPE
%type <tree> STATEMENT_LIST
%type <tree> STATEMENT
%type <tree> ASSIGNMENT_STATEMENT
%type <tree> METHOD_CALL_STATEMENT
%type <tree> RETURN_STATEMENT
%type <tree> IF_STATEMENT
%type <tree> WHILE_STATEMENT
%type <tree> EXPRESSION
%type <tree> SIMPLE_EXPRESSION
%type <tree> TERM
%type <tree> FACTOR
%type <tree> UNSIGNED_CONSTANT
%type <tree> VARIABLE

%%

PROGRAM : PROGRAM ID SEMI CLASS_DECLARATION_LIST
          {
            $$ = ast_new (PROGRAMOP,
                          $4,
                          ast_make_leaf (IDNODE, $2));
            printtree ($$, 0);
          }
  ;
/**
 * @todo What is `CLASS'?
 */

CLASS_DECLARATION_LIST : CLASS_DECLARATION
                         {
                           $$ = $1;
                         }
                       | CLASS_DECLARATION_LIST CLASS_DECLARATION
                         {
                           ast* t = ast_make_leaf (CLASSOP, NULL, $2);
                           $$ = ast_set_left_subtree ($1,
                                                      t);
                         }
  ;

CLASS_DECLARATION : CLASS ID CLASS_BODY
                    {
                      $$  = ast_new (CLASSDEFOP,
                                     $3,
                                     ast_make_leaf (IDNODE, $2));
                    }
  ;

CLASS_BODY : RBRACE DECLARATION_LIST METHOD_DECLARATION_LIST LBRACE
             {
               if ($3 == NULL && $2 == NULL)
                 {
                   $$ = NULL;
                 }
               else
                 {
                   $$ = ast_new (BODYOP, $2, $3);
                 }
             }
  ;

DECLARATION_LIST : /* empty */
                   {
                     $$ = NULL;
                   }
                 | FIELD_DECLARATION_LIST
                   {
                     $$ = ast_make_leaf (BODYOP, NULL, $1);
                   }
                 | DECLARATION_LIST FIELD_DECLARATION_LIST
                   {
                     $$ = ast_make_leaf (BODYOP, NULL, NULL);
                     $$ = ast_set_right_subtree ($$, $2);
                     if ($1 != NULL)
                       {
                         $$ = ast_set_left_subtree ($1, $$);
                       }
                   }
  ;

FIELD_DECLARATION_LIST : VARIABLE_DECLARATION_OR_INITIALIZATION_STATEMENT
                         {
                           $$ = ast_make_leaf (DECLOP, NULL, $1);
                         }
                       | FIELD_DECLARATION_LIST VARIABLE_DECLARATION_OR_INITIALIZATION_STATEMENT
                         {
                           $$ = ast_make_leaf (DECLOP, NULL, NULL);
                           $$ = ast_set_right_subtree ($$, $2);
                           if ($1 != NULL)
                             {
                               $$ = ast_set_left_subtree ($1, $$);
                             }
                         }
  ;


VARIABLE_DECLARATION_OR_INITIALIZATION_STATEMENT : TYPE VARIABLE_DECLARATION_OR_INITIALIZATION_LIST SEMI :
  ;

VARIABLE_DECLARATION_OR_INITIALIZATION_LIST : VARIABLE_DECLARATION_OR_INITIALIZATION
                                            | VARIABLE_DECLARATION_OR_INITIALIZATION COMMA VARIABLE_DECLARATION_OR_INITIALIZATION_LIST
  ;

VARIABLE_DECLARATION_OR_INITIALIZATION : VARIABLE_DECLARATION_ID
                                       | VARIABLE_INITIALIZATION
  ;

VARIABLE_DECLARATION_ID : VARIABLE_DECLARATION_LIST
                       {
                       }
  ;

VARIABLE_INITIALIZATION
  ;

METHOD_DECLARATION_LIST : /* empty */
                          {
                            $$ = NULL;
                          }
                        | METHOD_DECLARATION
                          {
                            $$ = ast_make_leaf (BODYOP, NULL, $1);
                          }
                        | METHOD_DECLARATION_LIST METHOD_DECLARATION
                          {
                            $$ = ast_make_leaf (BODYOP, NULL, NULL);
                            $$ = ast_set_right_subtree ($$, $2);
                            if ($1 != NULL)
                              {
                                $$ = ast_set_left_subtree ($1, $$);
                              }
                          }
  ;

METHOD_DECLARATION :
  ;

COMPARISON_OPERATOR : LT
                    | LE
                    | EQ
                    | NE
                    | GE
                    | GT
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
  ;

SIMPLE_EXPRESSION :
  ;

%%
