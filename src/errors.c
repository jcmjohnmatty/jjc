#include <stdio.h>
#include <stdlib.h>

#include <errors.h>
#include <source.h>

int jjc_errno = -1;

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
warning_line_column (int         line,
                     int         column,
                     const char* message)
{
  fprintf (stderr, "%s:%d:%d: warning: %s\n", sourcefile, line, column, message);
  jjc_errno = JJC_ERR_WARNING;
  return JJC_ERR_WARNING;
}
