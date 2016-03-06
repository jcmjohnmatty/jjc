/*
  proj2.c

  This file consists of 4 parts
  a. the data structure of a tree node
  b. the tree operation functions, from "CopyTree"
  to "SetRightChild"
  c. the tree printing function

  The functions in this file are contributed by Chunmin Qiao and
  Aggelos Varvitsiotis.
*/

#include <stdio.h>

#include <ast.h>

ILTree dummy = { DUMMYNode, 0, 0, 0, 0 };
tree root;

tree
NullExp (void)
{
  return &dummy;
}

tree
MakeLeaf (int Kind, int N)
{
  tree p;

  p = (tree) malloc (sizeof (ILTree));
  p->NodeKind = Kind;
  p->IntVal = N;
  return p;
}

tree
MakeTree (int NodeOp, tree Left, tree Right)
{
  tree p;

  p = (tree) malloc (sizeof (ILTree));
  p->NodeKind = EXPRNode;
  p->NodeOpType = NodeOp;
  p->LeftC = Left;
  p->RightC = Right;
  return p;
}

tree
LeftChild (tree T)
{
  if (T->NodeKind != EXPRNode)
    {
      return NullExp();
    }
  else
    {
      return T->LeftC;
    }
}

tree
RightChild (tree T)
{
  if (T->NodeKind != EXPRNode)
    {
      return NullExp ();
    }
  else
    {
      return T->RightC;
    }
}

tree
MkLeftC (tree T1, tree T2)
{
  tree p;
  tree q;

  if (IsNull (T2))
    {
      return T1;
    }

  p = T2;
  q = LeftChild (p);

  /* Replace the leftmost DUMMYNode. */
  while (!IsNull (q))
    {
      p = q;
      q = LeftChild(p);
    }
  p->LeftC = T1;

  return T2;
}

tree
MkRightC (tree T1, tree T2)
{
  tree p,q;

  if (IsNull (T2))
    {
      return T1;
    }

  p = T2;
  q = RightChild (p);

  /* Replace the rightmost DUMMYNode. */
  while (!IsNull (q))
    {
      p = q;
      q = RightChild (p);
    }
  p->RightC = T1;

  return T2;
}

int
NodeOp (tree T)
{
  if (T->NodeKind != EXPRNode)
    {
      printf ("NodeOP(): This node must be an EXPRNode!\n");
      return 0;
    }
  return T->NodeOpType;
}

int
IntVal (tree T)
{
  if (T->NodeKind == EXPRNode)
    {
      printf ("IntVal(): This node must be a leaf node!\n");
      return -1;
    }
  return T->IntVal;
}

int
IsNull (tree T)
{
  return T->NodeKind == DUMMYNode;
}

void
SetNode (tree Target, tree Source)
{
  Target->NodeKind = Source->NodeKind;
  if (Target->NodeKind != EXPRNode)
    {
      Target->IntVal = Source->IntVal;
      Target->LeftC = NullExp ();
      Target->RightC = NullEx p();
    }
  else
    {
      Target->NodeOpType = Source->NodeOpType;
      Target->LeftC = Source->LeftC;
      Target->RightC = Source->RightC;
    }
}

void
SetNodeOp (tree T, int Op)
{
  if (T->NodeKind != EXPRNode)
    {
      printf ("SetNodeOp(): This node must be an EXPRNode!\n");
    }
  else
    {
      T->NodeOpType = Op;
    }
}

void
SetLeftTreeOp (tree T, int Op)
{
  tree p;

  p = T;
  do
    {
      SetNodeOp (p, Op);
      p = LeftChild (p);
    }
  while (!IsNull (p));
}

void
SetRightTreeOp (tree T, int Op)
{
  tree  p;

  p = T;
  do
    {
      SetNodeOp (p, Op);
      p = RightChild (p);
    }
  while (!IsNull (p));
}

void
SetLeftChild (tree T, tree NewC)
{
  if (T->NodeKind != EXPRNode)
    {
      printf ("SetLeftChild(): This node must be an EXPRNode!\n");
    }
  else
    {
      T->LeftC = NewC;
    }
}

void
SetRightChild (tree T, tree NewC)
{
  if (T->NodeKind != EXPRNode)
    {
      printf ("SetRightChild(): This node must be an EXPRNode!\n");
    }
  else
    {
      T->RightC = NewC;
    }
}

/*
 * This is syntax tree printer, "treelst" is the output file  pointer.
 *
 * Call printtree with the root node pointer and the depth level.
 * The depth level can be 0 if it is not desired to indent the root.
 *
 * WRITING "getname()" AND "getstring()" IS YOUR RESPONSIBILITY!!!
 */

extern FILE *treelst;

char* opnodenames[] =
  {
    "ProgramOp", "BodyOp", "DeclOp", "CommaOp", "ArrayTypeOp", "TypeIdOp",
    "BoundOp", "RecompOp",
    "ToOp", "DownToOp", "ConstantIdOp", "ProceOp", "FuncOp",
    "HeadOp", "RArgTypeOp", "VargTypeOp", "StmtOp", "IfElseOp",
    "LoopOp", "SpecOp", "RoutineCallOp", "AssignOp", "ReturnOp",
    "AddOp", "SubOp", "MultOp", "DivOp",
    "LTOp", "GTOp", "EQOp", "NEOp", "LEOp", "GEOp", "AndOp", "OrOp",
    "UnaryNegOp", "NotOp", "VarOp", "SelectOp", "IndexOp", "FieldOp",
    "SubrangeOp", "ExitOp", "ClassOp", "MethodOp", "ClassDefOp"
  };

static int crosses[162];

void
indent (int x)
{
  int i;
  for (i = 0; i < x; i++)
    {
      fprintf (treelst,"%s", crosses [i]? "| " : "  ");
    }

  fprintf (treelst,"%s", x ? "+-" : "R-");

  if (x)
    {
      crosses[x] = (crosses[x] + 1) % 2;
    }
}

void
zerocrosses ()
{
  int i;
  for (i = 0; i < 162; i++)
    {
      crosses [i] = 0;
    }
}

extern char strg_tbl[];

/**
 * @brief Return ID name or String.
 *
 * @param[in] i The index of the string table (passed through yylval).
 */
char*
getname (int i)
{
  /* Return string table indexed at i. */
  return strg_tbl + i;
}

char*
getstring (int i)
{
  /* Return string table indexed at i. */
  return strg_tbl + i;
}

void
printtree (tree nd, int depth)
{
  int id;
  int indx;

  if (!depth)
    {
      zerocrosses ();
      fprintf (treelst,"************* SYNTAX TREE PRINTOUT ***********\n\n");
    }
  if (IsNull (nd))
    {
      indent (depth);
      fprintf (treelst,"[DUMMYnode]\n");
      return;
    }
  if (nd->NodeKind == EXPRNode)
    {
      printtree (RightChild (nd), depth + 1);
    }

  indent (depth);

  switch (nd->NodeKind)
    {
    case IDNode:
      indx = IntVal (nd);
      if (indx >= 0)
        {
          /* GetAttr (indx, NAME_ATTR); */
          id = indx;
          fprintf (treelst, "[IDNode,%d,\"%s\"]\n", IntVal (nd),
                   getname (id));
        }
      else
        {
          fprintf (treelst, "[IDNode,%d,\"%s\"]\n", indx, "err");
        }
      break;

    case STNode:
      indx = IntVal (nd);
      if (indx > 0)
        {
          /* GetAttr (indx, NAME_ATTR); */
          id = indx;
          fprintf (treelst, "[STNode,%d,\"%s\"]\n", IntVal (nd),
                   getname (id));
        }
      else
        {
          fprintf (treelst, "[IDNode,%d,\"%s\"]\n", indx, "err");
        }
      break;

    case INTEGERTNode:
      fprintf (treelst, "[INTEGERTNode]\n");
      break;

    case NUMNode:
      fprintf (treelst, "[NUMNode,%d]\n", IntVal (nd));
      break;

    case CHARNode:
      if (isprint (IntVal (nd)))
        {
          fprintf (treelst, "[CHARNode,%d,\'%c\']\n", IntVal (nd),
                   IntVal (nd));
        }
      else
        {
          fprintf (treelst,"[CHARNode,%d,\'\\%o\']\n", IntVal (nd),
                   IntVal (nd));
        }
      break;

    case STRINGNode:
      fprintf (treelst,"[STRINGNode,%d,\"%s\"]\n", IntVal (nd),
               getstring (IntVal (nd)));
      break;

    case EXPRNode:
      fprintf (treelst, "[%s]\n", opnodenames[NodeOp (nd) - ProgramOp]);
      break;

    default:
      fprintf (treelst, "INVALID!!!\n");
      break;
    }
  if (nd->NodeKind == EXPRNode)
    {
      printtree (LeftChild (nd), depth + 1);
    }
}
