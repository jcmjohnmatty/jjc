#include <stdio.h>
#include <stdlib.h>

#include <ast.h>
#include <strtbl.h>

ast dummy = { DUMMYNODE, 0, 0, 0, 0 };
ast* root;
ast* field_declaration_type = NULL;
ast* method_declaration_type = NULL;

ast*
ast_null (void)
{
  return &dummy;
}

ast*
ast_make_leaf (int type, int d)
{
  ast* p;

  p = (ast*) malloc (sizeof (ast));
  p->node_type = type;
  p->data = d;
  return p;
}

ast*
ast_new (int operation, ast* l, ast* r)
{
  ast* p;

  p = (ast*) malloc (sizeof (ast));
  p->node_type = EXPRNODE;
  p->operation_type = operation;
  p->left = l;
  p->right = r;
  return p;
}

void
ast_delete (ast* tree)
{
  if (tree == NULL)
    {
      return;
    }

  ast_delete (tree->left);
  ast_delete (tree->right);
  free (tree);
}

ast*
ast_get_left (ast* tree)
{
  if (tree->node_type != EXPRNODE)
    {
      return NULL;
    }
  else
    {
      return tree->left;
    }
}

ast*
ast_get_right (ast* tree)
{
  if (tree->node_type != EXPRNODE)
    {
      return NULL;
    }
  else
    {
      return tree->right;
    }
}

ast*
ast_set_left_subtree (ast* tree, ast* left)
{
  ast* p;
  ast* q;

  if (tree == NULL)
    {
      return left;
    }

  p = tree;
  q = ast_get_left (p);

  /* Replace the leftmost DUMMYNODE. */
  while (q != NULL)
    {
      p = q;
      q = ast_get_left (p);
    }
  p->left = left;

  return tree;
}

ast*
ast_set_right_subtree (ast* tree, ast* right)
{
  ast* p;
  ast* q;

  if (tree == NULL)
    {
      return right;
    }

  p = tree;
  q = ast_get_right (p);

  /* Replace the rightmost DUMMYNODE. */
  while (q != NULL)
    {
      p = q;
      q = ast_get_right (p);
    }
  p->right = right;

  return tree;
}

int
ast_get_type (ast* tree)
{
  if (tree->node_type != EXPRNODE)
    {
      printf ("ast_get_type(): This node must be an EXPRNODE!\n");
      return 0;
    }
  return tree->operation_type;
}

int
ast_get_data (ast* tree)
{
  if (tree->node_type == EXPRNODE)
    {
      printf ("ast_get_data(): This node must be a leaf node!\n");
      return -1;
    }
  return tree->data;
}

void
ast_assign (ast* tree, ast* other)
{
  tree->node_type = other->node_type;
  if (tree->node_type != EXPRNODE)
    {
      tree->data = other->data;
      tree->left = NULL;
      tree->right = NULL;
    }
  else
    {
      tree->operation_type = other->operation_type;
      tree->left = other->left;
      tree->right = other->right;
    }
}

void
ast_set_operation (ast* tree, int operation)
{
  if (tree->node_type != EXPRNODE)
    {
      printf ("ast_set_operation(): This node must be an EXPRNODE!\n");
    }
  else
    {
      tree->operation_type = operation;
    }
}

void
ast_set_left_subtree_operation (ast* tree, int operation)
{
  ast* p;

  p = tree;
  do
    {
      ast_set_operation (p, operation);
      p = ast_get_left (p);
    }
  while (p != NULL);
}

void
ast_set_right_subtree_operation (ast* tree, int operation)
{
  ast*  p;

  p = tree;
  do
    {
      ast_set_operation (p, operation);
      p = ast_get_right (p);
    }
  while (p != NULL);
}

/* void */
/* ast_set_left_subtree (ast* tree, ast* l) */
/* { */
/*   if (tree->node_type != EXPRNODE) */
/*     { */
/*       printf ("ast_set_left_subtree(): This node must be an EXPRNODE!\n"); */
/*     } */
/*   else */
/*     { */
/*       tree->left = l; */
/*     } */
/* } */

/* void */
/* ast_set_right_subtree (ast* tree, ast* r) */
/* { */
/*   if (tree->node_type != EXPRNODE) */
/*     { */
/*       printf ("ast_set_right_subtree(): This node must be an EXPRNODE!\n"); */
/*     } */
/*   else */
/*     { */
/*       tree->right = r; */
/*     } */
/* } */

char* opnodenames[] =
  {
    "PROGRAMOP", "BODYOP", "DECLOP", "COMMAOP", "ARRAYTYPEOP", "TYPEIDOP",
    "BOUNDOP", "RECOMPOP",
    "TOOP", "DOWNTOOP", "CONSTANTIDOP", "PROCEOP", "FUNCOP",
    "HEADOP", "RARGTYPEOP", "VARGTYPEOP", "STMTOP", "IFELSEOP",
    "LOOPOP", "SPECOP", "ROUTINECALLOP", "ASSIGNOP", "RETURNOP",
    "ADDOP", "SUBOP", "MULTOP", "DIVOP",
    "LTOP", "GTOP", "EQOP", "NEOP", "LEOP", "GEOP", "ANDOP", "OROP",
    "UNARYNEGOP", "NOTOP", "VAROP", "SELECTOP", "INDEXOP", "FIELDOP",
    "SUBRANGEOP", "EXITOP", "CLASSOP", "METHODOP", "CLASSDEFOP"
  };

static int crosses[162];

static void
indent (int x)
{
  int i;
  for (i = 0; i < x; i++)
    {
      printf ("%s", crosses[i] ? "| " : "  ");
    }

  printf ("%s", x ? "+-" : "R-");

  if (x)
    {
      crosses[x] = (crosses[x] + 1) % 2;
    }
}

static void
zerocrosses (void)
{
  int i;
  for (i = 0; i < 162; i++)
    {
      crosses[i] = 0;
    }
}

static void
ast_print2 (ast* tree, int depth)
{
  int id;
  int index;
  char* s;

  if (!depth)
    {
      zerocrosses ();
      printf ("************* SYNTAX TREE PRINTOUT ***********\n\n");
    }
  if (tree == NULL)
    {
      indent (depth);
      printf ("[DUMMYNODE]\n");
      return;
    }
  if (tree->node_type == EXPRNODE)
    {
      ast_print2 (ast_get_right (tree), depth + 1);
    }

  indent (depth);

  switch (tree->node_type)
    {
    case IDNODE:
      index = ast_get_data (tree);
      if (index >= 0)
        {
          id = index;
          s = string_table->buffer + string_table->str_indices[index];
          printf ("[IDNODE,%d,\"%s\"]\n", ast_get_data (tree), s);
        }
      else
        {
          printf ("[IDNODE,%d,\"%s\"]\n", index, "err");
        }
      break;

    case INTEGERTNODE:
      printf ("[INTEGERTNODE]\n");
      break;

    case NUMNODE:
      printf ("[NUMNODE,%d]\n", ast_get_data (tree));
      break;

    case STRINGNODE:
      id = ast_get_data (tree);
      char* s = string_table->buffer + string_table->str_indices[id];
      printf ("[STRINGNODE,%d,\"%s\"]\n", ast_get_data (tree), s);
      break;

    case EXPRNODE:
      printf ("[%s]\n", opnodenames[ast_get_type (tree) - PROGRAMOP]);
      break;

    default:
      printf ("INVALID!!!\n");
      break;
    }
  if (tree->node_type == EXPRNODE)
    {
      ast_print2 (ast_get_left (tree), depth + 1);
    }
}

void
ast_print (ast* tree)
{
  ast_print2 (tree, 0);
}
