#include <stdio.h>

#include <errors.h>
#include <source.h>

int
fatal_error_line_column (int         line,
                         int         column,
                         const char* message)
{
  fprintf (stderr, "%s:%d:%d: fatal error: %s", sourcefile, line, column,
		   message);
  return JJC_ERR_FATAL;
}

int
error_line_column (int         line,
                   int         column,
                   const char* message)
{
  fprintf (stderr, "%s:%d:%d: error: %s", sourcefile, line, column, message);
  return JJC_ERR_ERROR;
}

int
warning_line_column (int         line,
                     int         column,
                     const char* message)
{
  fprintf (stderr, "%s:%d:%d: warning: %s", sourcefile, line, column, message);
  return JJC_ERR_WARNING;
}
