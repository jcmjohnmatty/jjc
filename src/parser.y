%file-prefix "parser"
%defines "parser.h"

%code requires
{
  #include <ast.h>
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

PROGRAM
: PROGRAM ID SEMI CLASS_DECLARATION_LIST
{
  $$ = ast_new (PROGRAMOP,
                $4,
                ast_make_leaf (IDNODE, $2));
  printtree ($$, 0);
}
;

CLASS_DECLARATION_LIST
: CLASS_DECLARATION
{
  $$ = $1;
}
| CLASS_DECLARATION_LIST CLASS_DECLARATION
{
  ast* t = ast_make_leaf (CLASSOP, NULL, $2);
  $$ = ast_set_left_subtree ($1, t);
}
;

CLASS_DECLARATION
: CLASS ID CLASS_BODY
{
  $$  = ast_new (CLASSDEFOP, $3, ast_make_leaf (IDNODE, $2));
}
;

CLASS_BODY
: RBRACE DECLARATION_LIST METHOD_DECLARATION_LIST LBRACE
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

DECLARATION_LIST
: /* empty */
{
  $$ = NULL;
}
| FIELD_DECLARATION_LIST
{
  $$ = ast_new (BODYOP, NULL, $1);
}
| DECLARATION_LIST FIELD_DECLARATION_LIST
{
  $$ = ast_new (BODYOP, NULL, NULL);
  $$ = ast_set_right_subtree ($$, $2);
  if ($1 != NULL)
    {
      $$ = ast_set_left_subtree ($1, $$);
    }
}
;

FIELD_DECLARATION_LIST
: VARIABLE_DECLARATION_OR_INITIALIZATION_STATEMENT
{
  $$ = ast_new (DECLOP, NULL, $1);
}
| FIELD_DECLARATION_LIST VARIABLE_DECLARATION_OR_INITIALIZATION_STATEMENT
{
  $$ = ast_new (DECLOP, NULL, NULL);
  $$ = ast_set_right_subtree ($$, $2);
  if ($1 != NULL)
    {
      $$ = ast_set_left_subtree ($1, $$);
    }
}
;


VARIABLE_DECLARATION_OR_INITIALIZATION_STATEMENT
: TYPE VARIABLE_DECLARATION_OR_INITIALIZATION_LIST SEMI
{
  field_declaration_type = $1;
  $$ = $2;
}
;

VARIABLE_DECLARATION_OR_INITIALIZATION_LIST

: VARIABLE_DECLARATION_OR_INITIALIZATION
{
  $$ = $1;
}
| VARIABLE_DECLARATION_OR_INITIALIZATION COMMA VARIABLE_DECLARATION_OR_INITIALIZATION_LIST
{
  $$ = ast_new (DECLOP, NULL, NULL);
  $$ = ast_set_right_subtree ($$, $1);
  if ($3 != NULL)
    {
      $$ = ast_set_left_subtree ($3, $$);
    }
}
;

VARIABLE_DECLARATION_OR_INITIALIZATION
: VARIABLE_DECLARATION_ID
{
  $$ = ast_new (COMMAOP, $1, ast_new (COMMAOP, field_declaration_type, NULL));
}
| VARIABLE_INITIALIZATION
{
  $$ = $1;
}
;

VARIABLE_DECLARATION_ID
: ID
{
  $$ = ast_make_leaf (IDNODE, $1);
}
| ID LBRAC RBRAC
{
  $$ = ast_make_leaf (IDNODE, $1);
}
;

VARIABLE_INITIALIZATION
: VARIABLE_DECLARATION_ID EQUAL VARIABLE_INITALIZER
{
  $$ = ast_new (COMMAOP, $1, ast_new (COMMAOP, field_declaration_type, $3));
}
;

VARIABLE_INITALIZER
: EXPRESSION
{
  $$ = $1;
}
| ARRAY_INITALIZER
{
  $$ = $1;
}
| ARRAY_CREATION_EXPRESSION
{
  $$ = $1;
}
;

ARRAY_INITALIZER
: LBRACE VARIABLE_INITALIZER_LIST RBRACE
{
  $$ = ast_new (ARRAYTYPEOP, $2, field_declaration_type);
}
;

VARIABLE_INITALIZER_LIST
: VARIABLE_INITALIZER
{
  $$ = ast_new (COMMAOP, NULL, $1);
}
| VARIABLE_INITALIZER_LIST VARIABLE_INITALIZER
{
  $$ = ast_new (COMMAOP, NULL, NULL);
  $$ = ast_set_right_subtree ($$, $2);
  if ($1 != NULL)
    {
      $$ = ast_set_left_subtree ($1, $$);
    }
}
;

ARRAY_CREATION_EXPRESSION
: INT EXPRESSION_BRACKET_LIST
{
  $$ = ast_new (ARRAYTYPEOP, $2, ast_make_leaf (INTEGERTNODE, $1));
}
;

EXPRESSION_BRACKET_LIST
: LBRAC EXPRESSION RBRAC
{
  $$ = ast_new (COMMAOP, NULL, $2);
}
| EXPRESSION_LIST LBRAC EXPRESSION RBRAC
{
  $$ = ast_new (COMMAOP, NULL, NULL);
  $$ = ast_set_right_subtree ($$, $3);
  if ($1 != NULL)
    {
      $$ = ast_set_left_subtree ($1, $$);
    }
}

METHOD_DECLARATION_LIST
: /* empty */
{
  $$ = NULL;
}
| METHOD_DECLARATION
{
  $$ = ast_new (BODYOP, NULL, $1);
}
| METHOD_DECLARATION_LIST METHOD_DECLARATION
{
  $$ = ast_new (BODYOP, NULL, NULL);
  $$ = ast_set_right_subtree ($$, $2);
  if ($1 != NULL)
    {
      $$ = ast_set_left_subtree ($1, $$);
    }
}
;

VOID
:
{
  $$ = VOID;
}
;

TYPE_OR_VOID
: TYPE
| VOID
{
  method_declaration_type = $1;
}
;

METHOD_DECLARATION
: METHOD TYPE_OR_VOID ID LPAREN FORMAL_PARAMETER_LIST RPAREN BLOCK
{
  ast* head_op = ast_new (HEADOP, $3, $4);
  $$ = ast_new (METHODOP, head_op, $4);
}
;

FORMAL_PARAMETER_LIST
: PARTIAL_PARAMETER_LIST
{
  $$ = ast_new (SPECOP, $1, method_declaration_type);
}
| FORMAL_PARAMETER_LIST SEMI PARTIAL_PARAMETER_LIST
{
  $$ = ast_set_right_subtree ($1, $3);
}
;

IDENTIFIER_LIST
: ID
{
  /** @todo Is this duplication right/required? */
  ast* id_node = ast_make_leaf (IDNODE, $1);
  $$ = ast_new (COMMAOP, id_node $1);
  /**
   * @todo Whether to use RARGTYPEOP or VARGTYPEOP is ambiguous based on
   *       the description we were given.
   */
  $$ = ast_new (RARGTYPEOP, $$, NULL);
}
: ID_LIST COMMA ID
{
  /** @todo Is this duplication right/required? */
  ast* id_node = ast_make_leaf (IDNODE, $3);
  ast* third_id_node = ast_new (COMMAOP, id_node $3);
  /**
   * @todo Whether to use RARGTYPEOP or VARGTYPEOP is ambiguous based on
   *       the description we were given.
   */
  third_id_node = ast_new (RARGTYPEOP, third_id_node, NULL);
  $$ = ast_set_right_subtree ($1, third_id_node);
}
;
;

PARTIAL_PARAMETER_LIST
: VAL INT IDENTIFIER_LIST
{
  $$ = $3;
}
| INT IDENTIFIER_LIST
{
  $$ = $2;
}
;

BLOCK
: DECLARATION_LIST STATEMENT_LIST
{
  $$ = ast_new ($1, $2);
}
;

TYPE
: ID_DOTS
{
  $$ = $1;
}
| INT
{
  $$ = ast_new (TYPEOPID, $1, NODE);
}
| ID_ARR
{
  $$ = $1;
}
| INT_ARR
{
  $$ = $1;
}
;

ID_ARR
: ID LBRAC RBRAC
{
  ast* terminal_node = ast_new (INDEXOP, NULL, NULL);
  ast* indexed_node = ast_new (INDEXOP, NULL, terminal_node);
  $$ = ast_new (TYPEOPID, $1, indexed_node);
}
;

INT_ARR
: INT LBRAC RBRAC
{
  ast* terminal_node = ast_new (INDEXOP, NULL, NULL);
  ast* indexed_node = ast_new (INDEXOP, NULL, terminal_node);
  $$ = ast_new (TYPEOPID, $1, indexed_node);
}
;

ID_DOTS
: ID_OR_ID_ARRAY
{
  $$ = $1;
}
| ID DOT ID_DOTS
{
  ast* id_node = ast_new (TYPEOPID, $1, NULL);
  ast* dot_node = ast_new (FIELDOP, $3, NULL);
  $$ = ast_set_right_subtree (id_node, dot_node);
}
;

ID_OR_ID_ARRAY
: ID
{
  $$ = ast_new (TYPEOPID, $1, NULL);
}
: ID LBRAC RBRAC
{
  ast* terminal_node = ast_new (INDEXOP, NULL, NULL);
  ast* indexed_node = ast_new (INDEXOP, NULL, terminal_node);
  $$ = ast_new (TYPEOPID, $1, indexed_node);
}
;

STATEMENT_LIST
:
{
}
;

COMPARISON_OPERATOR
: LT
| LE
| EQ
| NE
| GE
| GT
;

EXPRESSION
: SIMPLE_EXPRESSION
{
  $$ = $1;
}
| SIMPLE_EXPRESSION COMPARISON_OPERATOR SIMPLE_EXPRESSION
{
  ast_set_left_subtree ($1, $2);
  $$ = ast_set_right_subtree ($3, $2);
}
;

SIMPLE_EXPRESSION
:
{
}
;

%%
