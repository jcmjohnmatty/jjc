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
%type <tree> METHOD_DECLARATION_LIST2
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
  root = ast_new (PROGRAMOP,
                  $4,
                  ast_make_leaf (IDNODE, $2));
  root->line = yyline;
  root->column = yycolumn;
}
;

CLASS_DECLARATION_LIST
: CLASS_DECLARATION
{
  $$ = ast_new (CLASSOP, NULL, $1);
  $$->line = yyline;
  $$->column = yycolumn;
}
| CLASS_DECLARATION_LIST CLASS_DECLARATION
{
  ast* t = ast_new (CLASSOP, NULL, $2);
  $$->line = yyline;
  $$->column = yycolumn;

  $$ = ast_set_left_subtree ($1, t);
}
;

CLASS_DECLARATION
: CLASS ID CLASS_BODY
{
  $$ = ast_new (CLASSDEFOP, $3, ast_make_leaf (IDNODE, $2));
  $$->line = yyline;
  $$->column = yycolumn;
}
;

CLASS_BODY
: LBRACE DECLARATION_LIST METHOD_DECLARATION_LIST2 RBRACE
{
  $$ = ast_set_left_subtree ($3, $2);
  /* $$ = ast_new (BODYOP, $2, $3); */
  /* $$->line = yyline; */
  /* $$->column = yycolumn; */
}
| LBRACE DECLARATION_LIST RBRACE
{
  $$ = ast_new (BODYOP, $2, NULL);
  $$->line = yyline;
  $$->column = yycolumn;
}
| LBRACE METHOD_DECLARATION_LIST2 RBRACE
{
  $$ = ast_new (BODYOP, NULL, $2);
  $$->line = yyline;
  $$->column = yycolumn;
}
| LBRACE RBRACE
{
  $$ = ast_null_node ();
  $$->line = yyline;
  $$->column = yycolumn;
}
;

DECLARATION_LIST
: DECLARATION ENDDECLARATION
{
  $$ = ast_null_node ();
  $$->line = yyline;
  $$->column = yycolumn;
}
| DECLARATION FIELD_DECLARATION_LIST ENDDECLARATION
{
  $$ = $2;
  /* $$ = ast_new (BODYOP, NULL, $2); */
  /* $$->line = yyline; */
  /* $$->column = yycolumn; */
}
;

FIELD_DECLARATION_LIST
: VARIABLE_DECLARATION_OR_INITIALIZATION_STATEMENT
{
  $$ = ast_new (BODYOP, NULL, $1);
  $$->line = yyline;
  $$->column = yycolumn;
}
| FIELD_DECLARATION_LIST VARIABLE_DECLARATION_OR_INITIALIZATION_STATEMENT
{
  $$ = ast_new (BODYOP, NULL, $2);
  $$->line = yyline;
  $$->column = yycolumn;
  if (!ast_is_null ($1))
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
  $$ = $2;
}
;

VARIABLE_DECLARATION_OR_INITIALIZATION_LIST
: VARIABLE_DECLARATION_OR_INITIALIZATION
{
  $$ = ast_new (DECLOP, NULL, $1);
  $$->line = yyline;
  $$->column = yycolumn;
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
  $$->line = yyline;
  $$->column = yycolumn;
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
  $$->line = yyline;
  $$->column = yycolumn;
}
| VARIABLE_DECLARATION_ID LBRAC RBRAC
{
  $$ = ast_new (INDEXOP, NULL, $1);
  $$->line = yyline;
  $$->column = yycolumn;
}
;

VARIABLE_INITIALIZATION
: VARIABLE_DECLARATION_ID EQUAL VARIABLE_INITALIZER
{
  ast* t = ast_new (COMMAOP, field_declaration_type, $3);
  t->line = yyline;
  t->column = yycolumn;
  $$ = ast_new (COMMAOP, $1, t);
  $$->line = yyline;
  $$->column = yycolumn;
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
  $$->line = yyline;
  $$->column = yycolumn;
}
;

VARIABLE_INITALIZER_LIST
: VARIABLE_INITALIZER
{
  $$ = ast_new (COMMAOP, NULL, $1);
  $$->line = yyline;
  $$->column = yycolumn;
}
| VARIABLE_INITALIZER COMMA VARIABLE_INITALIZER_LIST
{
  $$ = ast_new (COMMAOP, NULL, NULL);
  $$ = ast_set_right_subtree ($$, $1);
  if (!ast_is_null ($1))
    {
      $$ = ast_set_left_subtree ($3, $$);
    }
}
;

ARRAY_CREATION_EXPRESSION
: INT EXPRESSION_INDEXING_LIST
{
  $$ = ast_new (ARRAYTYPEOP, $2, ast_make_leaf (INTEGERTNODE, $1));
  $$->line = yyline;
  $$->column = yycolumn;
}
;

EXPRESSION_INDEXING_LIST
: LBRAC EXPRESSION RBRAC
{
  $$ = ast_new (COMMAOP, NULL, $2);
  $$->line = yyline;
  $$->column = yycolumn;
}
| EXPRESSION_INDEXING_LIST LBRAC EXPRESSION RBRAC
{
  $$ = ast_new (COMMAOP, NULL, NULL);
  $$ = ast_set_right_subtree ($$, $3);
  if (!ast_is_null ($1))
    {
      $$ = ast_set_left_subtree ($1, $$);
    }
}

METHOD_DECLARATION_LIST2
: METHOD_DECLARATION_LIST
{
  $$ = $1;
}
;

METHOD_DECLARATION_LIST
: METHOD_DECLARATION
{
  $$ = ast_new (BODYOP, NULL, $1);
  $$->line = yyline;
  $$->column = yycolumn;
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
  $$->line = yyline;
  $$->column = yycolumn;
}
| METHOD TYPE_OR_VOID ID LPAREN RPAREN BLOCK
{
  ast* head_op = ast_new (HEADOP, ast_make_leaf (IDNODE, $3),
                          ast_new (SPECOP, $1, method_declaration_type));
  $$ = ast_new (METHODOP, head_op, $6);
  $$->line = yyline;
  $$->column = yycolumn;
}
;

FORMAL_PARAMETER_LIST
: PARTIAL_PARAMETER_LIST
{
  $$ = ast_new (SPECOP, $1, method_declaration_type);
  $$->line = yyline;
  $$->column = yycolumn;
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
  id_node->line = yyline;
  id_node->column = yycolumn;
  ast* t = ast_make_leaf (INTEGERTNODE, $1);
  t->line = yyline;
  t->column = yycolumn;
  $$ = ast_new (COMMAOP, id_node, t);
  $$ = ast_new (RARGTYPEOP, $$, NULL);
  $$->line = yyline;
  $$->column = yycolumn;
}
| ID_LIST COMMA ID
{
  /** @todo Is this duplication right/required? */
  ast* id_node = ast_make_leaf (IDNODE, $3);
  id_node->line = yyline;
  id_node->column = yycolumn;
  ast* t = ast_make_leaf (INTEGERTNODE, $3);
  t->line = yyline;
  t->column = yycolumn;
  ast* third_id_node = ast_new (COMMAOP, id_node, t);
  third_id_node->line = yyline;
  third_id_node->column = yycolumn;

  third_id_node = ast_new (RARGTYPEOP, third_id_node, NULL);
  third_id_node->line = yyline;
  third_id_node->column = yycolumn;

  $$ = ast_set_right_subtree ($1, third_id_node);
}
;

PARTIAL_PARAMETER_LIST
: VAL INT ID_LIST
{
  ast_set_operation ($3, VARGTYPEOP);
  ast_set_right_subtree_operation ($3, VARGTYPEOP);

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
  $$->line = yyline;
  $$->column = yycolumn;
}
| STATEMENT_LIST
{
  $$ = ast_new (BODYOP, NULL, $1);
  $$->line = yyline;
  $$->column = yycolumn;
}
;

TYPE
: ID_DOTS
{
  field_declaration_type = $1;
  $$ = $1;
}
| INT
{
  $$ = ast_new (TYPEIDOP, ast_make_leaf (INTEGERTNODE, $1), NULL);
  field_declaration_type = $$;
}
| INT_ARR
{
  $$ = $1;
  field_declaration_type = $1;
}
;

INT_ARR
: INT LBRAC RBRAC
{
  $$ = ast_make_leaf (INTEGERTNODE, $1);
  $$->line = yyline;
  $$->column = yycolumn;
  $$ = ast_new (INDEXOP, $$, NULL);
  $$->line = yyline;
  $$->column = yycolumn;
}
| INT_ARR LBRAC RBRAC
{
  $$ = ast_new (INDEXOP, $1, NULL);
  $$->line = yyline;
  $$->column = yycolumn;
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
  id_node->line = yyline;
  id_node->column = yycolumn;
  ast* dot_node = ast_new (FIELDOP, $1, NULL);
  dot_node->line = yyline;
  dot_node->column = yycolumn;
  $$ = ast_set_right_subtree (dot_node, id_node);
}
;

ID_OR_ID_ARRAY
: ID
{
  ast* t = ast_make_leaf (IDNODE, $1);
  t->line = yyline;
  t->column = yycolumn;
  $$ = ast_new (TYPEIDOP, t, NULL);
  $$->line = yyline;
  $$->column = yycolumn;
}
| ID LBRAC RBRAC
{
  ast* indexed_node = ast_new (INDEXOP, NULL, NULL);
  indexed_node->line = yyline;
  indexed_node->column = yycolumn;
  $$ = ast_new (TYPEIDOP, ast_make_leaf (IDNODE, $1), indexed_node);
  $$->line = yyline;
  $$->column = yycolumn;
}
;

STATEMENT_LIST
: LBRACE STATEMENT_COMMA_LIST RBRACE
{
  $$ = $2;
}
| LBRACE RBRACE
{
  $$ = ast_null_node ();
  $$->line = yyline;
  $$->column = yycolumn;
}
;

STATEMENT_COMMA_LIST
: STATEMENT
{
  if (ast_is_null ($1))
    {
      $$ = ast_null_node ();
      $$->line = yyline;
      $$->column = yycolumn;
    }
  else
    {
      $$ = ast_new (STMTOP, NULL, $1);
      $$->line = yyline;
      $$->column = yycolumn;
    }
}
| STATEMENT STATEMENT_COMMA_LIST
{
  if (ast_is_null ($1))
    {
      if (ast_is_null ($2))
        {
          $$ = ast_null_node ();
          $$->line = yyline;
          $$->column = yycolumn;
        }
      else
        {
          $$ = $2;
        }
    }
  else
    {
      if (ast_is_null ($2))
        {
          $$ = ast_new (STMTOP, NULL, $1);
          $$->line = yyline;
          $$->column = yycolumn;
        }
      else
        {
          ast* t = ast_new (STMTOP, NULL, $1);
          t->line = yyline;
          t->column = yycolumn;
          $$ = ast_set_left_subtree (t, $2);
        }
    }
}
;

STATEMENT
: ASSIGNMENT_STATEMENT SEMI
{
  $$ = $1;
}
| METHOD_CALL_STATEMENT SEMI
{
  $$ = $1;
}
| RETURN_STATEMENT SEMI
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
| SEMI
{
  $$ = ast_null_node ();
  $$->line = yyline;
  $$->column = yycolumn;
}
;

ASSIGNMENT_STATEMENT
: VARIABLE ASSGN EXPRESSION
{
  ast* var_node = ast_new (ASSIGNOP, NULL, $1);
  var_node->line = yyline;
  var_node->column = yycolumn;
  $$ = ast_new (ASSIGNOP, var_node, $3);
  $$->line = yyline;
  $$->column = yycolumn;
}
;

METHOD_CALL_STATEMENT
: VARIABLE EXPRESSION_PAREN_LIST
{
  $$ = ast_new (ROUTINECALLOP, $1, $2);
  $$->line = yyline;
  $$->column = yycolumn;
}
;

RETURN_STATEMENT
: RETURN
{
  $$ = ast_new (RETURNOP, NULL, NULL);
  $$->line = yyline;
  $$->column = yycolumn;
}
| RETURN EXPRESSION
{
  $$ = ast_new (RETURNOP, $2, NULL);
  $$->line = yyline;
  $$->column = yycolumn;
}
;

IF_STATEMENT
: IF EXPRESSION STATEMENT_LIST ELSE STATEMENT_LIST
{
  ast* exp_statement_pair = ast_new (COMMAOP, $2, $3);
  exp_statement_pair->line = yyline;
  exp_statement_pair->column = yycolumn;
  ast* if_half = ast_new (IFELSEOP, NULL, exp_statement_pair);
  if_half->line = yyline;
  if_half->column = yycolumn;
  $$ = ast_new (IFELSEOP, if_half, $5);
  $$->line = yyline;
  $$->column = yycolumn;
}
| IF EXPRESSION STATEMENT_LIST
{
  ast* exp_statement_pair = ast_new (COMMAOP, $2, $3);
  exp_statement_pair->line = yyline;
  exp_statement_pair->column = yycolumn;
  $$ = ast_new (IFELSEOP, NULL, exp_statement_pair);
  $$->line = yyline;
  $$->column = yycolumn;
}
;

WHILE_STATEMENT
: WHILE EXPRESSION STATEMENT_LIST
{
  $$ = ast_new (LOOPOP, $2, $3);
  $$->line = yyline;
  $$->column = yycolumn;
}
;

EXPRESSION_PAREN_LIST
: LPAREN RPAREN
{
  $$ = ast_null_node ();
  $$->line = yyline;
  $$->column = yycolumn;
}
| LPAREN EXPRESSION_LIST RPAREN
{
  $$ = $2;
}

EXPRESSION_LIST
: EXPRESSION
{
  $$ = ast_new (COMMAOP, $1, NULL);
  $$->line = yyline;
  $$->column = yycolumn;
}
| EXPRESSION_LIST COMMA EXPRESSION
{
  ast* comma_node = ast_new (COMMAOP, $3, NULL);
  comma_node->line = yyline;
  comma_node->column = yycolumn;
  $$ = ast_set_right_subtree ($1, comma_node);
}
;

COMPARISON_OPERATOR
: LT
{
  $$ = ast_new (LTOP, NULL, NULL);
  $$->line = yyline;
  $$->column = yycolumn;
}
| LE
{
  $$ = ast_new (LEOP, NULL, NULL);
  $$->line = yyline;
  $$->column = yycolumn;
}
| EQ
{
  $$ = ast_new (EQOP, NULL, NULL);
  $$->line = yyline;
  $$->column = yycolumn;
}
| NE
{
  $$ = ast_new (NEOP, NULL, NULL);
  $$->line = yyline;
  $$->column = yycolumn;
}
| GE
{
  $$ = ast_new (GEOP, NULL, NULL);
  $$->line = yyline;
  $$->column = yycolumn;
}
| GT
{
  $$ = ast_new (GTOP, NULL, NULL);
  $$->line = yyline;
  $$->column = yycolumn;
}
;

EXPRESSION
: SIMPLE_EXPRESSION
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
: TERM
{
  $$ = $1;
}
| PLUS TERM
{
  $$ = $2;
}
| MINUS TERM
{
  ast* neg_node = ast_new (UNARYNEGOP, $2, NULL);
  $$ = neg_node;
  $$->line = yyline;
  $$->column = yycolumn;
}
| PLUS TERM BINARY_OPERATOR_TERM_LIST
{
  $$ = ast_set_left_subtree ($3, $2);
}
| MINUS TERM BINARY_OPERATOR_TERM_LIST
{
  ast* neg_node = ast_new (UNARYNEGOP, $2, NULL);
  neg_node->line = yyline;
  neg_node->column = yycolumn;
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
  $$->line = yyline;
  $$->column = yycolumn;
}
| MINUS TERM
{
  $$ = ast_new (SUBOP, NULL, $2);
  $$->line = yyline;
  $$->column = yycolumn;
}
| OR TERM
{
  $$ = ast_new (OROP, NULL, $2);
  $$->line = yyline;
  $$->column = yycolumn;
}
| BINARY_OPERATOR_TERM_LIST PLUS TERM
{
  ast* subterm = ast_new (ADDOP, NULL, $3);
  subterm->line = yyline;
  subterm->column = yycolumn;
  $$ = ast_set_left_subtree ($1, subterm);
}
| BINARY_OPERATOR_TERM_LIST MINUS TERM
{
  ast* subterm = ast_new (SUBOP, NULL, $3);
  subterm->line = yyline;
  subterm->column = yycolumn;
  $$ = ast_set_left_subtree ($1, subterm);
}
| BINARY_OPERATOR_TERM_LIST OR TERM
{
  ast* subterm = ast_new (OROP, NULL, $3);
  subterm->line = yyline;
  subterm->column = yycolumn;
  $$ = ast_set_left_subtree ($1, subterm);
}
;

TERM
: FACTOR BINARY_OPERATOR_FACTOR_LIST
{
  if (ast_is_null ($2))
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
  $$ = ast_null_node ();
  $$->line = yyline;
  $$->column = yycolumn;
}
| TIMES FACTOR
{
  $$ = ast_new (MULTOP, $2, NULL);
  $$->line = yyline;
  $$->column = yycolumn;
}
| DIVIDE FACTOR
{
  $$ = ast_new (DIVOP, $2, NULL);
  $$->line = yyline;
  $$->column = yycolumn;
}
| AND FACTOR
{
  $$ = ast_new (ANDOP, $2, NULL);
  $$->line = yyline;
  $$->column = yycolumn;
}
| BINARY_OPERATOR_FACTOR_LIST TIMES FACTOR
{
  ast* subterm = ast_new (MULTOP, $3, NULL);
  subterm->line = yyline;
  subterm->column = yycolumn;
  $$ = ast_set_right_subtree ($1, subterm);
}
| BINARY_OPERATOR_FACTOR_LIST DIVIDE FACTOR
{
  ast* subterm = ast_new (DIVOP, $3, NULL);
  subterm->line = yyline;
  subterm->column = yycolumn;
  $$ = ast_set_right_subtree ($1, subterm);
}
| BINARY_OPERATOR_FACTOR_LIST AND FACTOR
{
  ast* subterm = ast_new (ANDOP, $3, NULL);
  subterm->line = yyline;
  subterm->column = yycolumn;
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
  /// @todo NOTOP
  $$ = $2;
}
;

UNSIGNED_CONSTANT
: ICONST
{
  $$ = ast_make_leaf (NUMNODE, $1);
  $$->line = yyline;
  $$->column = yycolumn;
}
| SCONST
{
  $$ = ast_make_leaf (STRINGNODE, $1);
  $$->line = yyline;
  $$->column = yycolumn;
}
;

VARIABLE
: ID VARIABLE_LIST
{
  if (!ast_is_null ($2))
    {
      ast* right = ast_new (SELECTOP, $2, NULL);
      right->line = yyline;
      right->column = yycolumn;
      ast* t = ast_make_leaf (IDNODE, $1);
      t->line = yyline;
      t->column = yycolumn;
      $$ = ast_new (VAROP, t, right);
      $$->line = yyline;
      $$->column = yycolumn;
    }
  else
    {
      $$ = ast_make_leaf (IDNODE, $1);
      $$->line = yyline;
      $$->column = yycolumn;
    }
}
;

VARIABLE_LIST
: /* empty */
{
  $$ = ast_null_node ();
  $$->line = yyline;
  $$->column = yycolumn;
}
| EXPRESSION_BRACKET_LIST VARIABLE_LIST
{
  if (ast_is_null ($2))
    {
      $$ = $1;
      $$->line = yyline;
      $$->column = yycolumn;
    }
  else
    {
      $$ = ast_set_right_subtree ($1, $2);
    }
}
| VARIABLE_LIST DOT ID
{
  ast* t = ast_make_leaf (IDNODE, $3);
  t->line = yyline;
  t->column = yycolumn;

  ast* field_node = ast_new (FIELDOP, t, NULL);
  if (ast_is_null ($1))
    {
      $$ = field_node;
      $$->line = yyline;
      $$->column = yycolumn;
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
