#include <stdio.h>

#include <errors.h>

int
error_line_column (const char* sourcefile,
				   int         line,
				   const char* message)
{
  fprintf (stderr, "%s:%s:%s: %s", sourcefile, lineno, column, message);
  return 1;
}
