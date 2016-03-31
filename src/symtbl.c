#include <stdio.h>

#include <ast.h>
#include <source.h>
#include <strtbl.h>
#include <symtbl.h>

/*
 * symbol table array
 */
int symtbl_array[SYMTBL_SIZE];

/*
 * attribute array.  attributes are allocated from this array instead of from
 * dynamically allocated space.  It may easy testing and debugging.
 */
attribute_type attribute_array[ATTR_SIZE];

/*
 * The stack of symbol tables.
 */
symtbl_stack stack[STACK_SIZE];

/* stack top counter */
int stack_top = 0;
/* symbol table top counter */
int symtbl_top = 0;
/* nesting level counter */
int nesting = 0;
/* attribute array counter */
int attr_top = 0;

extern int yyline;
extern int yycolumn;

/* string table in table.c */
extern char strg_tbl[];

void
symtbl_init ()
{
  int strtbl_index;
  int symtbl_index;

  strtbl_index = strtbl_get_index (string_table, "system");

  if (strtbl_index != -1)
    {
      symtbl_index = symtbl_insert_entry (strtbl_index);
      symtbl_set_attribute (symtbl_index, PREDE_ATTR, true);
      symtbl_set_attribute (symtbl_index, KIND_ATTR, CLASS);
    }

  strtbl_index = strtbl_get_index (string_table, "readln");
  if (strtbl_index != -1)
    {
      symtbl_index = symtbl_insert_entry (strtbl_index);
      symtbl_set_attribute (symtbl_index, NEST_ATTR, nesting + 1);
      symtbl_set_attribute (symtbl_index, PREDE_ATTR, true);
      symtbl_set_attribute (symtbl_index, KIND_ATTR, PROCE);
    }

  strtbl_index = strtbl_get_index (string_table, "println");
  if (strtbl_index != -1)
    {
      symtbl_index = symtbl_insert_entry (strtbl_index);
      symtbl_set_attribute (symtbl_index, NEST_ATTR, nesting + 1);
      symtbl_set_attribute (symtbl_index, PREDE_ATTR, true);
      symtbl_set_attribute (symtbl_index, KIND_ATTR, PROCE);
    }
}

void
semantic_error (int type, int action, int id, int symtbl_number)
{
  char* s;

  fprintf (stderr, "%s:%d:%d: error: ", sourcefile, yyline, yycolumn);
  switch (type)
    {
    case STACK_OVERFLOW:
      fprintf (stderr, "stack overflow.\n");
      break;

    case REDECLARATION:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "symbol %s: redeclared.\n", s);
      break;

    case ST_OVERFLOW:
      fprintf (stderr, "symbol table overflow.\n");
      break;

    case UNDECLARATION:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "symbol %s: undeclared.\n", s);
      break;

    case ATTR_OVERFLOW:
      fprintf (stderr, "attribute array overflowed.\n");
      break;

    case BOUND:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "symbol %s: not declared as a constant, can't be used as subrange bound.\n", s);
      break;

    case ARGUMENTS_NUM1:
      fprintf (stderr, "routine %s: argument number in definition is different with the previous forward declaration. \n", s);
      break;

    case ARGUMENTS_NUM2:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "routine %s: argument number is different with the previous declaration. \n", s);
      break;

    case FORW_REDECLARE:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "routine %s: forward redeclaration.\n", s);
      break;

    case PROCE_MISMATCH:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "symbol %s: can't act as a procedure call.\n", s);
      break;

    case FUNC_MISMATCH:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "symbol %s: can't act as a function call.\n", s);
      break;

    case VAR_VAL:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "routine %s: reference/value type of the ", s);
      fprintf (stderr, "%s parameter different with previous forward declaration.\n",
               symtbl_ordinal_abbreviation (symtbl_number));
      break;

    case CONSTANT_VAR:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "routine %s: the ", s);
      fprintf (stderr, "%s parameter is a reference argument, can't be a constant.\n",
               symtbl_ordinal_abbreviation (symtbl_number));
      break;

    case EXPR_VAR:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "routine %s: reference argument of the ", s);
      fprintf (stderr, "%s parameter can't be a expression. \n",
               symtbl_ordinal_abbreviation (symtbl_number));
      break;

    case CONSTANT_ASSIGN:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "symbol %s: declared to be a constant, can't be assigned a new value.\n", s);
      break;

    case ARR_TYPE_MIS:
      s = string_table->buffer + id + 1;
      if (symtbl_number == 0)
        {
          fprintf (stderr, "symbol %s: isn't defined as an array.\n", s);
        }
      else
        {
          fprintf (stderr, "symbol %s: the ", s);
          fprintf (stderr, "%s index isn't defined as an array.\n",
                   symtbl_ordinal_abbreviation (symtbl_number));
        }
      break;

    case ARR_DIME_MIS:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "symbol %s: inappropriate usage of arry element.\n", s);
      break;

    case REC_TYPE_MIS:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "symbol %s: illegal usage of a field name. \n", s);
      break;

    case INDX_MIS:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "symbol %s: has incorrect number of dimensions.\n", s);
      break;

    case FIELD_MIS:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "symbol %s: is an undeclared field name.\n", s);
      break;

    case VARIABLE_MIS:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "symbol %s: can't be used as a variable.\n", s);
      break;

    case NOT_TYPE:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "symbol %s: is not declared as a type.\n", s);
      break;

    case TYPE_MIS:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "symbol %s: incorrect type usage.\n", s);
      break;

    case MULTI_MAIN:
      fprintf (stderr, "main() method already declared.\n");
      break;

    default:
      fprintf (stderr, "error type: %d.\n", type);
    }

  if (action == ABORT)
    {
      exit (0);
    }
}

int
symtbl_insert_entry (int id)
{
  /* id is already declared in the current block. */
  if (symtbl_lookup_here (id))
  {
    semantic_error (REDECLARATION, CONTINUE, id, 0);
    return 0;
  }

  if (symtbl_top >= SYMTBL_SIZE - 1)
    {
      semantic_error (ST_OVERFLOW, ABORT, 0 ,0);
    }

  ++symtbl_top;
  symtbl_array[symtbl_top] = 0;
  symtbl_set_attribute (symtbl_top, NAME_ATTR, id);
  symtbl_set_attribute (symtbl_top, NEST_ATTR, nesting);
  symtbl_push (false, id, symtbl_top, false);
  return symtbl_top;
}

int
symtbl_lookup (int id)
{
  int i;

  for (i = stack_top; i > 0; --i)
    {
      if (!stack[i].is_marker && stack[i].name == id)
        {
          stack[i].used = true;
          return stack[i].symtbl_index;
        }
    }

  /* id is undefined, push a dummy element onto stack. */
  semantic_error (UNDECLARATION, CONTINUE, id, 0);
  symtbl_push (false, id, 0, true);
  return 0;
}

int
symtbl_lookup_here (int id)
{
  int i;

  for (i = stack_top; !stack[i].is_marker; --i)
    {
      if (stack[i].name == id && !stack[i].is_dummy)
        {
          return stack[i].symtbl_index;
        }
    }

  return 0;
}

void
symtbl_open_block (void)
{
  ++nesting;
  /* Push a marker onto stack. */
  symtbl_push (true, 0, 0, false);
}

void
symtbl_close_block (void)
{
  int i;

  for (i = stack_top; !stack[i].is_marker; --i)
    {
      if (!stack[i].used && !stack[i].is_dummy)
        {
          /* semantic_error (NOT_USED, CONTINUE, stack[i].name, 0);*/
        }
    }

  --nesting;
  /* trim the stack */
  stack_top = i - 1;
}

int
symtbl_is_attribute (int symtbl_index, int attribute_number)
{
  int i;

  i = symtbl_array[symtbl_index];
  while (i)
  {
    if (attribute_array[i].attribute_number == attribute_number)
      {
        return i;
      }
    if (attribute_array[i].attribute_number > attribute_number)
      {
        break;
      }
    i = attribute_array[i].next_attribute_index;
  }
  return 0;
}

int
symtbl_get_attribute (int symtbl_index, int attribute_number)
{
  int i;

  i = symtbl_is_attribute (symtbl_index, attribute_number);
  if (!i)
    {
      printf ("DEBUG--The wanted attribute number %d does not exist\n", attribute_number);
      return 0;
    }

  return attribute_array[i].attribute_value;
}

void
symtbl_set_attribute (int symtbl_index, int attribute_number, int attribute_value)
{
  int* p;
  int next;
  int i;

  i = symtbl_is_attribute (symtbl_index, attribute_number);
  if (i)
    {
      attribute_array[i].attribute_value = attribute_value;
      return;
    }

  p = &symtbl_array[symtbl_index];
  next = symtbl_array[symtbl_index];

  /* Search the link list for the right insert position. */
  while (next)
    {
      if (attribute_array[next].attribute_number < attribute_number)
        {
          p = &(attribute_array[next].next_attribute_index);
          next = attribute_array[next].next_attribute_index;
        }
      else
        {
          break;
        }
    }

  if (attr_top >= ATTR_SIZE-1)
    {
      semantic_error (ATTR_OVERFLOW, ABORT, 0, 0);
    }

  ++attr_top;
  attribute_array[attr_top].attribute_number = attribute_number;
  attribute_array[attr_top].attribute_value = attribute_value;
  attribute_array[attr_top].next_attribute_index = next;
  *p = attr_top;
}

char* kind_name[] = {"constant", "variable", "funcforw", "function", "ref_arg",
                     "val_arg", "field", "typedef", "procforw", "procedure",
                     "class", "array"};

void
symtbl_print (void)
{
  int i;
  int attribute_number;
  int attribute_value;
  int attr_ptr;
  int type_attr;
  int tree_value = 0;
  ast* tree_ptr;
  char* s;

  printf ("********************************Symbol Table************************************\n\n");
  printf ("          Name Nest-Level  Tree-Node Predefined        Kind       Type      Value  Offset Dimension   Argnum\n\n");

  for (i = 1; i <= symtbl_top; ++i)
    {
      tree_value = 0;
      printf ("%3d", i);
      for (attribute_number = NAME_ATTR; attribute_number <= ARGNUM_ATTR; ++attribute_number)
        {
          if (attr_ptr = symtbl_is_attribute (i, attribute_number))
            {
              attribute_value = attribute_array[attr_ptr].attribute_value;
              switch (attribute_number)
                {
                case NAME_ATTR:
                  s = string_table->buffer + attribute_value + 1;
                  printf ("%11s", s);
                  break;

                case OFFSET_ATTR:
                case NEST_ATTR:
                case TREE_ATTR:
                  printf ("%11d", attribute_value);
                  break;

                case TYPE_ATTR:
                  tree_value = attribute_value;
                  printf ("%11d", attribute_value);
                  break;

                case PREDE_ATTR:
                  if (attribute_value == true)
                    {
                      printf ("%11s", "yes");
                    }
                  else
                    {
                      printf ("%11s", "no");
                    }
                  break;

                case KIND_ATTR:
                  printf ("%11s", kind_name[attribute_value - 1]);
                  break;

                case VALUE_ATTR:
                  if (symtbl_get_attribute (i, KIND_ATTR) == CONST)
                    {
                      tree_ptr = (ast*) attribute_value;
                      switch (NodeKind(tree_ptr))
                        {
                        case NUMNode:
                          printf ("%11d", ast_get_data (tree_ptr));
                          break;

                        case CHARNode:
                          if (isprint (ast_get_data (tree_ptr)))
                            {
                              printf ("%11c", ast_get_data (tree_ptr));
                            }
                          break;

                        case STRINGNode:
                          s = string_table->buffer + ast_get_data (tree_ptr) + 1;
                          printf ("%11s", s);
                          break;
                        }
                    }
                  break;

                case DIMEN_ATTR:
                  printf ("%11d", attribute_value);
                  break;

                case ARGNUM_ATTR:
                  printf ("%11d", attribute_value);
                  break;
                }
            }
          else
            {
              printf ("%11s", " ");
            }

          printf ("\n");
        }
    }
}

void
symtbl_push (int is_marker, int name, int symtbl_index, int is_dummy)
{

  if (stack_top >= STACK_SIZE - 1)
    {
      semantic_error (STACK_OVERFLOW, ABORT, 0, 0);
    }

  ++stack_top;

  stack[stack_top].is_marker = (bool) is_marker;
  stack[stack_top].name = name;
  stack[stack_top].symtbl_index = symtbl_index;
  stack[stack_top].is_dummy = (bool) is_dummy;
  stack[stack_top].used = false;

}

char*
symtbl_ordinal_abbreviation (int i)
{
  /** @todo This is bad, should not use local array. */
  char s[10];

  switch (i)
    {
    case 0:
      return("0th");

    case 1:
      return("1st");

    case 2:
      return("2nd");

    case 3:
      return("3rd");

    default:
      sprintf (s, "%dth", i);
      return s;
    }
}
