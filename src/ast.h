#ifndef __AST_H_
#define __AST_H_

#define PROGRAMOP       100
#define BODYOP          101
#define DECLOP          102
#define COMMAOP         103
#define ARRAYTYPEOP     104
#define TYPEIDOP        105
#define BOUNDOP         106
#define RECOMPOP        107
#define TOOP            108
#define DOWNTOOP        109
#define CONSTANTIDOP    110
#define PROCEOP         111
#define FUNCOP          112
#define HEADOP          113
#define RARGTYPEOP      114
#define VARGTYPEOP      115
#define STMTOP          116
#define IFELSEOP        117
#define LOOPOP          118
#define SPECOP          119
#define ROUTINECALLOP   120
#define ASSIGNOP        121
#define RETURNOP        122
#define ADDOP           123
#define SUBOP           124
#define MULTOP          125
#define DIVOP           126
#define LTOP            127
#define GTOP            128
#define EQOP            129
#define NEOP            130
#define LEOP            131
#define GEOP            132
#define ANDOP           133
#define OROP            134
#define UNARYNEGOP      135
#define NOTOP           136
#define VAROP           137
#define SELECTOP        138
#define INDEXOP         139
#define FIELDOP         140
#define SUBRANGEOP      141
#define EXITOP          142
#define CLASSOP         143
#define METHODOP        144
#define CLASSDEFOP      145

#define IDNODE          200
#define NUMNODE         201
#define STRINGNODE      202
#define DUMMYNODE       203
#define INTEGERTNODE    204
#define EXPRNODE        205

/**
 * @brief Syntax tree node struct.
*/
struct ast
{
  /** */
  int node_type;
  /** */
  int operation_type;
  /** */
  int data;
  /** */
  struct ast* left;
  /** */
  struct ast* right;
};

/** Typedef for struct ast. */
typedef struct ast ast;

/** The root node for our ast. */
extern ast* root;

/** The type for field declarations. */
extern ast* field_declaration_type;
extern ast* method_declaration_type;

/**
 * @brief YYSTYPE
 */
typedef union
{
  int constant;
  ast* tree;
} YYSTYPE;

/**
 * @brief Create a leaf node with a given node_type and data.
 *
 * @param[in] type The new type of the for the <code>ast</code>'s leaf.
 * @param[in] d    The data to store in the new leaf.
 */
ast*
ast_make_leaf (int type, int d);

/**
 * @brief Create an interior node with a given operation_type and left and
 *        right subtrees.
 *
 * @param[in] operation The operation for the new <code>ast</code>.
 * @param[in] l         The left subtree for the new <code>ast</code>.
 * @param[in] r         The right subtree for the new <code>ast</code>.
 *
 * @return A pointer to the newly constructed <code>ast</code>.
 */
ast*
ast_new (int operation, ast* l, ast* r);

/**
 * @brief Recursively free the memory used for an ast.
 *
 * @param[in] tree The <code>ast</code> to delete.
 */
void
ast_delete (ast* tree);

/**
 * @brief Get an <code>ast</code>'s left subtree.
 *
 * @param[in] tree The <code>ast</code> to get the left subtree of.
 *
 * @return <code>tree</code>'s left subtree.
 */
ast*
ast_get_left (ast* tree);

/**
 * @brief Get an <code>ast</code>'s right subtree.
 *
 * @param[in] tree The <code>ast</code> to get the right subtree of.
 *
 * @return <code>tree</code>'s right subtree.
 */
ast*
ast_get_right (ast* tree);

/**
 * @brief Set the leftmost subtree of an <code>ast</code>.
 *
 * @param[in, out] tree The tree whose left subtree to modify.
 * @param[in]      left The new leftmost subtree for <code>tree</code>.
 *
 * @return The new ast with the left subtree modified.
 */
ast*
ast_set_left_subtree (ast* tree, ast* left);

/**
 * @brief Set the rightmost child of an <code>ast</code>.
 *
 * @param[in, out] tree  The tree whose right subtree to modify.
 * @param[in]      right The new rightmost subtree for <code>tree</code>.
 *
 * @return The new right subtree.
 */
ast*
ast_set_right_subtree (ast* tree, ast* right);

/**
 * @brief Get an <code>ast</code>'s NodeOpType.
 *
 * @param[in] tree The <code>ast</code> to get the type of.
 *
 * @return The type of <code>tree</code>.
 */
int
ast_get_type (ast* tree);

/**
 * @brief Get an <code>ast</code>'s data.
 *
 * @param[in] tree The <code>ast</code> to get the data of.
 *
 * @return <code>tree</code>'s data.
 */
int
ast_get_data (ast* tree);

/**
 * @brief Copy the contents of a one <code>ast</code> to another.
 *
 * @param[in, out] tree  The <code>ast</code> to assign to.
 * @param[in]      other The <code>ast</code> to assign to <code>tree</code>.
 */
void
ast_assign (ast* tree, ast* other);

/**
 * @brief Set a tree's operation.
 *
 * This is only for interior EXPRNODE nodes.
 *
 * @param[in, out] tree      The <code>ast</code> to set the operation of.
 * @param[in]      operation The operation code.
 */
void
ast_set_operation (ast* tree, int operation);

/**
 * @brief Set an <code>ast</code>'s root and all left subtrees' operation type.
 *
 * This is only used during the construction of record component
 * <code>ast</code>s.
 *
 * @param[in, out] tree      The <code>ast</code> whose left subtree operation
 *                           to set.
 * @param[in]      operation The operation code.
 */
void
ast_set_left_subtree_operation (ast* T, int operation);

/**
 * @brief Set an <code>ast</code>'s root and all right subtrees' operation
 * type.
 *
 * This is only used during the construction of function call
 * <code>ast</code>s, where the function takes arguments.
 *
 * @param[in, out] tree      The <code>ast</code> whose right subtree operation
 *                           to set.
 * @param[in]      operation The operation code.
 */
void
ast_set_right_subtree_operation (ast* T, int operation);

/* /\** */
/*  * @brief Set an <code>ast</code>'s left subtree. */
/*  * */
/*  * @param[in, out] tree The <code>ast</code> to set the left subtree of. */
/*  * @param[in]      l    The new left subtree for <code>tree</code>. */
/*  *\/ */
/* void */
/* ast_set_left_subtree (ast* tree, ast* l); */

/* /\** */
/*  * @brief Set an <code>ast</code>'s right subtree. */
/*  * */
/*  * @param[in, out] tree The <code>ast</code> to set the right subtree of. */
/*  * @param[in]      r    The new right subtree for <code>tree</code>. */
/*  *\/ */
/* void */
/* ast_set_right_subtree (ast* tree, ast* r); */

/**
 * @brief Print an ast.
 *
 * @param[in] tree The ast to print.
 */
void
ast_print (ast* tree);

#endif /* __AST_H_ */
