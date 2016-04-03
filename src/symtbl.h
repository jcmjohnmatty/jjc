#ifndef __SYMTBL_H_
#define __SYMTBL_H_

#define STACK_SIZE 100
#define SYMTBL_SIZE 500
#define ATTR_SIZE 2000

/*
 * These definitions correspond to the fields of a stack element.
 */

#define MARKER 1
#define NAME 2
#define ST_PTR 3
#define DUMMY 4
#define USED 5

/** Value: id lexeme pointer, set by InsertEntry. */
#define NAME_ATTR 1
/** Value: nesting level, set by InsertEntry. */
#define NEST_ATTR 2
/** Value: point back to the subtree. */
#define TREE_ATTR 3
/** Value: is this id predefined? */
#define PREDE_ATTR 4
/** Value: pointer to the type tree for a varible, constant id or function. */
#define TYPE_ATTR 6
/** Value: the value of a constant id (integer, charater or string pointer). */
#define VALUE_ATTR 7
#define OFFSET_ATTR 8
/** Value: see below. */
#define KIND_ATTR 5

#define DIMEN_ATTR   9
#define ARGNUM_ATTR 10

/*
 * The possible values of attribute kind_attr.
 */

#define CONST 1
#define VAR 2
#define FUNCFORWARD 3
#define FUNC 4
#define REF_ARG 5
#define VALUE_ARG 6
#define FIELD 7
#define TYPEDEF 8
#define PROCFORWARD 9
#define PROCE 10
#define CLASS_KIND 11
#define ARR 12

/**
 * @brief Struct of symbol table stack
 */
struct symtbl_stack
{
  /** Mark the beginning of a block. */
  int is_marker;
  /** Point to the lexeme of the id. */
  int name;
  /** Point to the <code>id</code>'s symbol table entry. */
  int symtbl_index;
  /** Dummy element to indicate an undeclared <code>id</code>. */
  int is_dummy;
  /** Is this <code>id</code> used? */
  int used;
};

typedef struct symtbl_stack symtbl_stack;

/* stack array */
extern symtbl_stack stack[STACK_SIZE];

/**
 * @brief Struct of element of attribute list.
 */

struct attribute_type
{
  /** Attribute number (< 256). */
  char attribute_number;
  /** Attribute value. */
  int attribute_value;
  int next_attribute_index;
};

typedef struct attribute_type attribute_type;

/**
 * @brief initialize the symbol table.
 *
 * Put predefined names into it and build trees for those names.
 */
void
symtbl_init (void);

/**
 * @brief Build a symbol table entry for id.
 *
 * The current block is searched for redeclaration error.  The id's name and
 * nesting level attributes are set by this function.
 */
int
symtbl_insert_entry (int id);

/**
 * @brief Search an id in the stack and return a pointer to the symbol table
 *        entry.
 *
 * If the id is not in the stack, report error and push a dummy item on the
 * stack so that the same error will not be reported again.
 */
int
symtbl_lookup (int id);

/**
 * @brief Search an id in the stack for the current block.
 *
 * Returns the symbol table pointer if the id is found (if not, return 0).
 * This routine can be used to check if there is a forward declaration for a
 * function.
 */
int
symtbl_lookup_here (int id);

/**
 * @brief Build a mark on the stack to indecate the beginning of a new block.
 *
 * Increment nesting level counter.  It is called when reserved words
 * "program", "procedure", "function" or "record" are encountered.  Note:
 * procedure or function name should be inserted into symbol table before
 * calling this routine.
 */
void
symtbl_open_block (void);

/**
 * @brief Decrement nesting level counter and remove the current block from the
 *        stack.
 *
 * This function is called when exitting from a program, procedure, function or
 * a record definition. each element is checked to see if it is used in the
 * block.
 */
void
symtbl_close_block (void);

/**
 * @brief Return the index to the searched attribute in attrarray if it is
 *        in (nonzero).  Otherwise, return false.
 */
int
symtbl_is_attribute (int symtbl_index, int attribute_number);

/**
 * @brief Return the specified attribute value for a symbol table entry if
 *        found.  Otherwise, report an error.
 *
 * Note, this error is not the fault of source program but of the compiler
 * writer.  It is printed for testing and debugging.
 */
int
symtbl_get_attribute (int symtbl_index, int attribute_number);

/**
 * @brief Set attribute.
 *
 * If the attribute is already there, print debugging message.  Attributes for
 * a symbol table entry are sorted by their attribute_number.
 */
void
symtbl_set_attribute (int symtbl_index, int attribute_number, int attribute_value);

/**
 * @brief Print symbol table.
 */
void
symtbl_print (void);

/**
 * @brief Push an element onto the stack.
 */
void
symtbl_push (int is_marker, int name, int symtbl_index, int is_dummy);

/**
 * @brief Get the ordinal abbreviation for an integer.
 *
 * @param i The integer to get the ordinal abbreviation of.
 *
 * @return The ordinal abbreviation of <code>i</code>.
 */
char*
symtbl_ordinal_abbreviation (int i);

#endif /* __SYMTBL_H_ */
