%{ /* definition */ #include "proj2.h" #include <stdio.h> %}
%token <intg> PROGRAMnum IDnum .... SCONSTnum %type <tptr> Program ClassDecl ..... Variable
%% /* yacc specification */
Program : PROGRAMnum IDnum SEMInum ClassDecl
{ $$ = MakeTree(ProgramOp, $4, MakeLeaf(IDNode, $2)); printtree($$, 0); }
;
/* other rules */
Expression : SimpleExpression {$$ = $1;}
| SimpleExpression Comp_op SimpleExpression
{ MkLeftC($1, $2); $$ = MkRightC($3, $2); }
%%
int yycolumn, yyline;
FILE *treelst;
main() { treelst = stdout; yyparse(); }
yyerror(char *str) { printf("yyerror: %s at line %d\n", str, yyline); } #include "lex.yy.c"
