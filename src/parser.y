%file-prefix "parser"
%output "parser.c"
%defines "parser.h"

%code requires
{
  #include <ast.h>
  #include <errors.h>
}

%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <lexer.h>

  extern int yyline, yycolumn;
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

%type <tree> PROGRAM_DECLARATION
%type <tree> CLASS_DECLARATION_LIST
%type <tree> CLASS_DECLARATION
%type <tree> CLASS_BODY
%type <tree> ID_LIST
%type <tree> EXPRESSION_BRACKET_LIST
%type <tree> EXPRESSION_LIST
%type <tree> VARIABLE_DECLARATION_ID
%type <tree> VARIABLE_INITIALIZATION
%type <tree> VARIABLE_INITALIZER
%type <tree> VARIABLE_INITALIZER_LIST
%type <tree> VARIABLE_DECLARATION_OR_INITIALIZATION_LIST
%type <tree> VARIABLE_DECLARATION_OR_INITIALIZATION_STATEMENT
%type <tree> VARIABLE_DECLARATION_OR_INITIALIZATION
%type <tree> FIELD_DECLARATION_LIST
%type <tree> ARRAY_INITALIZER
%type <tree> DECLARATION_LIST
%type <tree> ARRAY_CREATION_EXPRESSION
%type <tree> EXPRESSION_INDEXING_LIST
%type <tree> METHOD_DECLARATION_LIST
%type <tree> METHOD_DECLARATION
%type <tree> FORMAL_PARAMETER_LIST
%type <tree> PARTIAL_PARAMETER_LIST
%type <tree> BLOCK
%type <tree> TYPE
%type <tree> ID_DOTS
%type <tree> INT_ARR
%type <tree> ID_OR_ID_ARRAY
%type <tree> STATEMENT_LIST
%type <tree> STATEMENT_COMMA_LIST
%type <tree> STATEMENT
%type <tree> ASSIGNMENT_STATEMENT
%type <tree> METHOD_CALL_STATEMENT
%type <tree> RETURN_STATEMENT
%type <tree> IF_STATEMENT
%type <tree> WHILE_STATEMENT
%type <tree> COMPARISON_OPERATOR
%type <tree> EXPRESSION
%type <tree> SIMPLE_EXPRESSION
%type <tree> EXPRESSION_PAREN_LIST
%type <tree> TERM
%type <tree> BINARY_OPERATOR_TERM_LIST
%type <tree> FACTOR
%type <tree> BINARY_OPERATOR_FACTOR_LIST
%type <tree> UNSIGNED_CONSTANT
%type <tree> VARIABLE
%type <tree> VARIABLE_LIST

%%

PROGRAM_DECLARATION
: PROGRAM ID SEMI CLASS_DECLARATION_LIST
{
  $$ = ast_new (PROGRAMOP,
                $4,
                ast_make_leaf (IDNODE, $2));
  ast_print ($$);
}
;

CLASS_DECLARATION_LIST
: CLASS_DECLARATION
{
  $$ = ast_new (CLASSOP, NULL, $1);
}
| CLASS_DECLARATION_LIST CLASS_DECLARATION
{
  ast* t = ast_new (CLASSOP, NULL, $2);
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
: LBRACE DECLARATION_LIST METHOD_DECLARATION_LIST RBRACE
{
  $$ = ast_new (BODYOP, $2, $3);
}
| LBRACE DECLARATION_LIST RBRACE
{
  $$ = ast_new (BODYOP, $2, NULL);
}
| LBRACE METHOD_DECLARATION_LIST RBRACE
{
  $$ = ast_new (BODYOP, NULL, $2);
}
| LBRACE RBRACE
{
  $$ = NULL;
}
;

DECLARATION_LIST
: DECLARATION ENDDECLARATION
{
  $$ = NULL;
}
| DECLARATION FIELD_DECLARATION_LIST ENDDECLARATION
{
  $$ = ast_new (BODYOP, NULL, $2);
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
  else
    {
      $$ = $2;
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
  $$ = ast_new (DECLOP, NULL, $1);
}
| VARIABLE_DECLARATION_OR_INITIALIZATION_LIST COMMA VARIABLE_DECLARATION_OR_INITIALIZATION
{
  $$ = ast_new (DECLOP, NULL, $3);
  $$ = ast_set_left_subtree ($1, $$);
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
: INT EXPRESSION_INDEXING_LIST
{
  $$ = ast_new (ARRAYTYPEOP, $2, ast_make_leaf (INTEGERTNODE, $1));
}
;

EXPRESSION_INDEXING_LIST
: LBRAC EXPRESSION RBRAC
{
  $$ = ast_new (COMMAOP, NULL, $2);
}
| EXPRESSION_INDEXING_LIST LBRAC EXPRESSION RBRAC
{
  $$ = ast_new (COMMAOP, NULL, NULL);
  $$ = ast_set_right_subtree ($$, $3);
  if ($1 != NULL)
    {
      $$ = ast_set_left_subtree ($1, $$);
    }
}

METHOD_DECLARATION_LIST
: METHOD_DECLARATION
{
  $$ = ast_new (BODYOP, NULL, $1);
}
| METHOD_DECLARATION_LIST METHOD_DECLARATION
{
  $$ = ast_new (BODYOP, NULL, $2);
  $$ = ast_set_left_subtree ($1, $$);
}
;

TYPE_OR_VOID
: TYPE
{
  method_declaration_type = $1;
}
| VOID
{
  method_declaration_type = $1;
}
;

METHOD_DECLARATION
: METHOD TYPE_OR_VOID ID LPAREN FORMAL_PARAMETER_LIST RPAREN BLOCK
{
  ast* head_op = ast_new (HEADOP, ast_make_leaf (IDNODE, $3), $5);
  $$ = ast_new (METHODOP, head_op, $7);
}
| METHOD TYPE_OR_VOID ID LPAREN RPAREN BLOCK
{
  ast* head_op = ast_new (HEADOP, ast_make_leaf (IDNODE, $3), NULL);
  $$ = ast_new (METHODOP, head_op, $6);
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

ID_LIST
: ID
{
  /** @todo Is this duplication right/required? */
  ast* id_node = ast_make_leaf (IDNODE, $1);
  $$ = ast_new (COMMAOP, id_node, ast_make_leaf (INTEGERTNODE, $1));
  /**
   * @todo Whether to use RARGTYPEOP or VARGTYPEOP is ambiguous based on
   *       the description we were given.
   */
  $$ = ast_new (RARGTYPEOP, $$, NULL);
}
| ID_LIST COMMA ID
{
  /** @todo Is this duplication right/required? */
  ast* id_node = ast_make_leaf (IDNODE, $3);
  ast* third_id_node = ast_new (COMMAOP, id_node, $1);

  /**
   * @todo Whether to use RARGTYPEOP or VARGTYPEOP is ambiguous based on
   *       the description we were given.
   */
  third_id_node = ast_new (RARGTYPEOP, third_id_node, NULL);
  $$ = ast_set_right_subtree ($1, third_id_node);
}
;

PARTIAL_PARAMETER_LIST
: VAL INT ID_LIST
{
  $$ = $3;
}
| INT ID_LIST
{
  $$ = $2;
}
;

BLOCK
: DECLARATION_LIST STATEMENT_LIST
{
  $$ = ast_new (BODYOP, $1, $2);
}
| STATEMENT_LIST
{
  $$ = ast_new (BODYOP, NULL, $1);
}
;

TYPE
: ID_DOTS
{
  $$ = $1;
}
| INT
{
  $$ = ast_new (TYPEIDOP, ast_make_leaf (INTEGERTNODE, $1), NULL);
}
| INT_ARR
{
  $$ = $1;
}
;

INT_ARR
: INT LBRAC RBRAC
{
  ast* terminal_node = ast_new (INDEXOP, NULL, NULL);
  ast* indexed_node = ast_new (INDEXOP, NULL, terminal_node);
  $$ = ast_new (TYPEIDOP, ast_make_leaf (INTEGERTNODE, $1), indexed_node);
}
;

ID_DOTS
: ID_OR_ID_ARRAY
{
  $$ = $1;
}
| ID_DOTS DOT ID
{
  ast* id_node = ast_new (TYPEIDOP, ast_make_leaf (INTEGERTNODE, $3), NULL);
  ast* dot_node = ast_new (FIELDOP, $1, NULL);
  $$ = ast_set_right_subtree (dot_node, id_node);
}
;

ID_OR_ID_ARRAY
: ID
{
  $$ = ast_new (TYPEIDOP, ast_make_leaf (IDNODE, $1), NULL);
}
| ID LBRAC RBRAC
{
  ast* terminal_node = ast_new (INDEXOP, NULL, NULL);
  ast* indexed_node = ast_new (INDEXOP, NULL, terminal_node);
  $$ = ast_new (TYPEIDOP, ast_make_leaf (IDNODE, $1), indexed_node);
}
;

STATEMENT_LIST
: LBRACE STATEMENT_COMMA_LIST RBRACE
{
  $$ = $2;
}
| LBRACE RBRACE
{
  $$ = NULL;
}
;

STATEMENT_COMMA_LIST
: STATEMENT SEMI
{
  $$ = ast_new (STMTOP, NULL, $1);
}
| STATEMENT SEMI STATEMENT_COMMA_LIST
{
  $$ = ast_set_left_subtree ($1, $3);
}
;

STATEMENT
: ASSIGNMENT_STATEMENT
{
  $$ = $1;
}
| METHOD_CALL_STATEMENT
{
  $$ = $1;
}
| RETURN_STATEMENT
{
  $$ = $1;
}
| IF_STATEMENT
{
  $$ = $1;
}
| WHILE_STATEMENT
{
  $$ = $1;
}
;

ASSIGNMENT_STATEMENT
: VARIABLE EQUAL EXPRESSION
{
  ast* var_node = ast_new (ASSIGNOP, NULL, $1);
  $$ = ast_new (ASSIGNOP, var_node, $3);
}
;

METHOD_CALL_STATEMENT
: VARIABLE EXPRESSION_PAREN_LIST
{
  $$ = ast_new (ROUTINECALLOP, $1, $2);
}
;

RETURN_STATEMENT
: RETURN
{
  $$ = ast_new (RETURNOP, NULL, NULL);
}
| RETURN EXPRESSION
{
  $$ = ast_new (RETURNOP, $2, NULL);
}
;

IF_STATEMENT
: IF LPAREN EXPRESSION RPAREN STATEMENT_LIST ELSE STATEMENT_LIST
{
  ast* exp_statement_pair = ast_new (COMMAOP, $3, $5);
  ast* if_half = ast_new (IFELSEOP, NULL, exp_statement_pair);
  $$ = ast_new (IFELSEOP, if_half, $7);
}
| IF LPAREN EXPRESSION RPAREN STATEMENT_LIST
{
  ast* exp_statement_pair = ast_new (COMMAOP, $3, $5);
  $$ = ast_new (IFELSEOP, NULL, exp_statement_pair);
}
;

WHILE_STATEMENT
: WHILE LPAREN EXPRESSION RPAREN STATEMENT_LIST
{
  $$ = ast_new (LOOPOP, $3, $5);
}
;

EXPRESSION_PAREN_LIST
: LPAREN RPAREN
{
  $$ = NULL;
}
| LPAREN EXPRESSION_LIST RPAREN
{
  $$ = $2;
}

EXPRESSION_LIST
: EXPRESSION
{
  $$ = ast_new (COMMAOP, $1, NULL);
}
| EXPRESSION_LIST COMMA EXPRESSION
{
  ast* comma_node = ast_new (COMMAOP, $3, NULL);
  $$ = ast_set_right_subtree ($1, comma_node);
}
;

COMPARISON_OPERATOR
: LT
{
  $$ = ast_new (LT, NULL, NULL);
}
| LE
{
  $$ = ast_new (LE, NULL, NULL);
}
| EQ
{
  $$ = ast_new (EQ, NULL, NULL);
}
| NE
{
  $$ = ast_new (NE, NULL, NULL);
}
| GE
{
  $$ = ast_new (GE, NULL, NULL);
}
| GT
{
  $$ = ast_new (GT, NULL, NULL);
}
;

EXPRESSION
: UNSIGNED_CONSTANT
{
  $$ = $1;
}
| VARIABLE
{
  $$ = $1;
}
| SIMPLE_EXPRESSION
{
  $$ = $1;
}
| SIMPLE_EXPRESSION COMPARISON_OPERATOR SIMPLE_EXPRESSION
{
  ast_set_left_subtree ($2, $1);
  $$ = ast_set_right_subtree ($2, $3);
}
;

SIMPLE_EXPRESSION
: PLUS TERM BINARY_OPERATOR_TERM_LIST
{
  $$ = ast_set_left_subtree ($3, $2);
}
| MINUS TERM BINARY_OPERATOR_TERM_LIST
{
  ast* neg_node = ast_new (UNARYNEGOP, $2, NULL);
  $$ = ast_set_left_subtree ($3, neg_node);
}
| TERM BINARY_OPERATOR_TERM_LIST
{
  $$ = ast_set_left_subtree ($2, $1);
}
;

BINARY_OPERATOR_TERM_LIST
: PLUS TERM
{
  $$ = ast_new (ADDOP, NULL, $2);
}
| MINUS TERM
{
  $$ = ast_new (SUBOP, NULL, $2);
}
| OR TERM
{
  $$ = ast_new (OROP, NULL, $2);
}
| BINARY_OPERATOR_TERM_LIST PLUS TERM
{
  ast* subterm = ast_new (ADDOP, NULL, $3);
  $$ = ast_set_left_subtree ($1, subterm);
}
| BINARY_OPERATOR_TERM_LIST MINUS TERM
{
  ast* subterm = ast_new (SUBOP, NULL, $3);
  $$ = ast_set_left_subtree ($1, subterm);
}
| BINARY_OPERATOR_TERM_LIST OR TERM
{
  ast* subterm = ast_new (OROP, NULL, $3);
  $$ = ast_set_left_subtree ($1, subterm);
}
;

TERM
: FACTOR BINARY_OPERATOR_FACTOR_LIST
{
  if ($2 == NULL)
    {
      $$ = $1;
    }
  else
    {
      $$ = ast_set_right_subtree ($2, $1);
    }
}
;

BINARY_OPERATOR_FACTOR_LIST
: /* empty */
{
  $$ = NULL;
}
| TIMES FACTOR
{
  $$ = ast_new (MULTOP, $2, NULL);
}
| DIVIDE FACTOR
{
  $$ = ast_new (DIVOP, $2, NULL);
}
| AND FACTOR
{
  $$ = ast_new (ANDOP, $2, NULL);
}
| BINARY_OPERATOR_FACTOR_LIST TIMES FACTOR
{
  ast* subterm = ast_new (MULTOP, $3, NULL);
  $$ = ast_set_right_subtree ($1, subterm);
}
| BINARY_OPERATOR_FACTOR_LIST DIVIDE FACTOR
{
  ast* subterm = ast_new (DIVOP, $3, NULL);
  $$ = ast_set_right_subtree ($1, subterm);
}
| BINARY_OPERATOR_FACTOR_LIST AND FACTOR
{
  ast* subterm = ast_new (ANDOP, $3, NULL);
  $$ = ast_set_right_subtree ($1, subterm);
}
;

FACTOR
: UNSIGNED_CONSTANT
{
  $$ = $1;
}
| VARIABLE
{
  $$ = $1;
}
| METHOD_CALL_STATEMENT
{
  $$ = $1;
}
| LPAREN EXPRESSION RPAREN
{
  $$ = $2;
}
| NOT FACTOR
{
  $$ = $2;
}
;

UNSIGNED_CONSTANT
: ICONST
{
  $$ = ast_make_leaf (NUMNODE, $1);
}
| SCONST
{
  $$ = ast_make_leaf (STRINGNODE, $1);
}
;

VARIABLE
: ID VARIABLE_LIST
{
  if ($2 != NULL)
    {
      $$ = ast_new (VAROP, ast_make_leaf (IDNODE, $1), NULL);
    }
  else
    {
      $$ = ast_make_leaf (IDNODE, $1);
    }
}
;

VARIABLE_LIST
: /* empty */
{
  $$ = NULL;
}
| VARIABLE_LIST EXPRESSION_BRACKET_LIST
{
  if ($1 == NULL)
    {
      $$ = $1;
    }
  else
    {
      $$ = ast_set_right_subtree ($1, $2);
    }
}
| VARIABLE_LIST DOT ID
{
  ast* field_node = ast_new (FIELDOP, ast_make_leaf (IDNODE, $3), NULL);
  if ($1 == NULL)
    {
      $$ = field_node;
    }
  else
    {
      ast_set_right_subtree ($1, field_node);
    }
}
;

EXPRESSION_BRACKET_LIST
: LBRAC EXPRESSION_LIST RBRAC
{
  ast_set_right_subtree_operation ($2, INDEXOP);
  $$ = $2;
}
;

%%

int
yyerror (char const* s)
{
  return error_line_column (yyline, yycolumn, s);
}
