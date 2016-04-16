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

    case FUNC_MISMATCH:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "symbol %s is not a function\n", s);
      break;

    case VAR_VAL:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "function %s: reference/value type of ", s);
      fprintf (stderr, "argument %s differ from previous declaration\n",
               symtbl_ordinal_abbreviation (symtbl_number));
      break;

    case CONSTANT_VAR:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "function %s: ", s);
      fprintf (stderr, "argument %s is a reference argument\n",
               symtbl_ordinal_abbreviation (symtbl_number));
      break;

    case CONSTANT_ASSIGN:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "symbol %s is constant\n", s);
      break;

    case VARIABLE_MIS:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "symbol %s can't be used as a variable\n", s);
      break;

    case TYPE_MIS:
      s = string_table->buffer + id + 1;
      fprintf (stderr, "incorrect type usage for symbol %s\n", s);
      break;

    case MULTI_MAIN:
      fprintf (stderr, "redeclaration of method main\n");
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
