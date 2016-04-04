#include <stdio.h>
#include <stdlib.h>

#include <errors.h>
#include <source.h>
#include <strtbl.h>

int jjc_errno = -1;

extern int yyline;
extern int yycolumn;

int
fatal_error_line_column (int         line,
                         int         column,
                         const char* message)
{
  fprintf (stderr, "%s:%d:%d: fatal error: %s\n", sourcefile, line, column,
		   message);
  jjc_errno = JJC_ERR_FATAL;
  exit (1);
  return JJC_ERR_FATAL;
}

int
error_line_column (int         line,
                   int         column,
                   const char* message)
{
  fprintf (stderr, "%s:%d:%d: error: %s\n", sourcefile, line, column, message);
  jjc_errno = JJC_ERR_ERROR;
  return JJC_ERR_ERROR;
}

int
semantic_error (int type,
                int action,
                int id,
                int symtbl_number,
                int line,
                int column)
{
  char* s;

  fprintf (stderr, "%s:%d:%d: error: ", sourcefile, line, column);
  switch (type)
    {
    case STACK_OVERFLOW:
      fprintf (stderr, "stack overflow\n");
      break;

    case REDECLARATION:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "redeclaration of symbol %s\n", s);
      break;

    case SYMTBL_OVERFLOW:
      fprintf (stderr, "symbol table overflow\n");
      break;

    case UNDECLARATION:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "undeclared symbol %s\n", s);
      break;

    case ATTR_OVERFLOW:
      fprintf (stderr, "attribute array overflowed\n");
      break;

    case BOUND:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "symbol %s: not declared as a constant, can't be used as subrange bound\n", s);
      break;

    /* case ARGUMENTS_NUM1: */
    /*   fprintf (stderr, "routine %s: argument number in definition is different with the previous forward declaration\n", s); */
    /*   break; */

    /* case ARGUMENTS_NUM2: */
    /*   s = string_table->buffer + id + 1; */
    /*   fprintf (stderr, "routine %s: argument number is different with the previous declaration\n", s); */
    /*   break; */

    case FORW_REDECLARE:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "routine %s: forward redeclaration\n", s);
      break;

    case PROCE_MISMATCH:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "symbol %s: can't act as a procedure call\n", s);
      break;

    case FUNC_MISMATCH:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "symbol %s: can't act as a function call\n", s);
      break;

    case VAR_VAL:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "routine %s: reference/value type of the ", s);
      fprintf (stderr, "%s parameter different with previous forward declaration\n",
               symtbl_ordinal_abbreviation (symtbl_number));
      break;

    case CONSTANT_VAR:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "routine %s: the ", s);
      fprintf (stderr, "%s parameter is a reference argument, can't be a constant\n",
               symtbl_ordinal_abbreviation (symtbl_number));
      break;

    case EXPR_VAR:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "routine %s: reference argument of the ", s);
      fprintf (stderr, "%s parameter can't be a expression\n",
               symtbl_ordinal_abbreviation (symtbl_number));
      break;

    case CONSTANT_ASSIGN:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "symbol %s: declared to be a constant, can't be assigned a new value\n", s);
      break;

    case ARR_TYPE_MIS:
      s = string_table->buffer + id + 1;
      if (symtbl_number == 0)
        {
          fprintf (stderr, "symbol %s: isn't defined as an array\n", s);
        }
      else
        {
          fprintf (stderr, "symbol %s: the ", s);
          fprintf (stderr, "%s index isn't defined as an array\n",
                   symtbl_ordinal_abbreviation (symtbl_number));
        }
      break;

    case ARR_DIME_MIS:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "symbol %s: inappropriate usage of arry element\n", s);
      break;

    case REC_TYPE_MIS:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "symbol %s: illegal usage of a field name\n", s);
      break;

    case INDX_MIS:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "symbol %s: has incorrect number of dimensions\n", s);
      break;

    case FIELD_MIS:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "symbol %s: is an undeclared field name\n", s);
      break;

    case VARIABLE_MIS:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "symbol %s: can't be used as a variable\n", s);
      break;

    case NOT_TYPE:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "symbol %s: is not declared as a type\n", s);
      break;

    case TYPE_MIS:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "symbol %s: incorrect type usage\n", s);
      break;

    case MULTI_MAIN:
      fprintf (stderr, "main() method already declared\n");
      break;

    default:
      fprintf (stderr, "error type: %d\n", type);
    }

  if (action == ABORT)
    {
      exit (0);
    }

  jjc_errno = JJC_ERR_ERROR;
  return JJC_ERR_ERROR;
}

int
warning_line_column (int         line,
                     int         column,
                     const char* message)
{
  fprintf (stderr, "%s:%d:%d: warning: %s\n", sourcefile, line, column, message);
  jjc_errno = JJC_ERR_WARNING;
  return JJC_ERR_WARNING;
}
