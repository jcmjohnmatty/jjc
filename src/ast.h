#ifndef __AST_H_
#define __AST_H_

#define ProgramOp       100
#define BodyOp          101
#define DeclOp          102
#define CommaOp         103
#define ArrayTypeOp     104
#define TypeIdOp        105
#define BoundOp         106
#define RecompOp        107
#define ToOp            108
#define DownToOp        109
#define ConstantIdOp    110
#define ProceOp         111
#define FuncOp          112
#define HeadOp          113
#define RArgTypeOp      114
#define VArgTypeOp      115
#define StmtOp          116
#define IfElseOp        117
#define LoopOp          118
#define SpecOp          119
#define RoutineCallOp   120
#define AssignOp        121
#define ReturnOp        122
#define AddOp           123
#define SubOp           124
#define MultOp          125
#define DivOp           126
#define LTOp            127
#define GTOp            128
#define EQOp            129
#define NEOp            130
#define LEOp            131
#define GEOp            132
#define AndOp           133
#define OrOp            134
#define UnaryNegOp      135
#define NotOp           136
#define VarOp           137
#define SelectOp        138
#define IndexOp         139
#define FieldOp         140
#define SubrangeOp      141
#define ExitOp          142
#define ClassOp         143
#define MethodOp        144
#define ClassDefOp      145

#define IDNode          200
#define NUMNode         201
#define CHARNode        202
#define STRINGNode      203
#define DUMMYNode       204
#define EXPRNode        205
#define INTEGERTNode    206
#define CHARTNode       207
#define BOOLEANTNode    208
#define STNode          209

/**
 * @brief Syntax tree node struct.
*/
struct treenode
{
  /** */
  int NodeKind;
  /** */
  int NodeOpType
  /** */
  int IntVal;
  /** */
  struct treenode* LeftC;
  /** */
  struct treenode* RightC;
};

/** */
typedef struct treenode treenode;
/** */
typedef struct treenode ILTree;
/** */
typedef struct treenode* tree;

/** */
extern tree Root;

/**
 * @brief YYSTYPE.
 */
typedef union
{
  int intg;
  tree tptr;
} YYSTYPE;

/**
 * @brief Return a DUMMYNode.
 *
 * All the dummy nodes correspond to the memory location of the NULL node.
 * Therefore, any attampt to use it for the other purposes will be problematic.
 */
tree
NullExp (void);

/**
 * @brief Create a leaf node with a given NodeKind and IntVal.
 */
tree
MakeLeaf (int Kind, int N);

/**
 * @brief Create an interior node with a given NodeOpType and left and right
 *        subtrees.
 */
tree
MakeTree (int NodeOp, tree Left, tree Right);

/**
 * @brief Get a treenode's left subtree.
 */
tree
LeftChild (tree T);

/**
 * @brief Get a treenode's right subtree.
 */
tree
RightChild (tree T);

/**
 * @brief Set the leftmost child of T2 to T1.
 *
 * @return T2.
 *
 * @todo Change so that callers just look at T2 for the return value.
 */
tree
MkLeftC (tree T1, tree T2);

/**
 * @brief Get a tree's NodeOpType.
 */
int
NodeOp (tree T);

/**
 * @brief Get a tree's IntVal.
 */
int
IntVal (tree T);

/**
 * @brief Check whether or not a tree is a NULL tree.
 */
int
IsNull (tree T);

/**
 * @brief Set the rightmost child of T2 to T1.
 *
 * @return T2.
 *
 * @todo Change so that callers just look at T2 for the return value.
 */
tree
MkRightC (tree T1, tree T2);

/**
 * @brief Copy the contents of a souce node to a destination node.
 */
void
SetNode (tree, tree);

/**
 * @brief Set a tree's operation.
 *
 * This is only for interior EXPRNode nodes.
 */
void
SetNodeOp (tree, int);

/**
 * @brief Set a tree's root and all left subtrees to a NewOp node.
 *
 * This is only used during the construction of record component trees, where.
 */
void
SetLeftTreeOp (tree T, int Op);

/**
 * @brief Set a tree's root and all right subtrees to a NewOp node.
 *
 * This is only used during the construction of function call trees, where
 * the function takes arguments.
 */
void
SetRightTreeOp (tree T, int Op);

/**
 * @brief Set T's LeftChild.
 */
void
SetLeftChild (tree T, tree NewC);

/**
 * @brief Set T's RightChild.
 */
void
SetRightChild (tree T, tree NewC);

#endif /* __AST_H_ */
