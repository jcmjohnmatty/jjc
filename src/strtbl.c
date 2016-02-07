#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <strtbl.h>

strtbl*
strtbl_new (void)
{
  strtbl* stbl = malloc (sizeof (strtbl));
  stbl->buffer = NULL;
  stbl->str_indices = NULL;
  stbl->buffer_length = 0;
  stbl->n_strings = 0;
  return stbl;
}

void
strtbl_delete (strtbl* stbl)
{
  if (stbl->buffer != NULL)
	{
	  free (stbl->buffer);
	}
  if (stbl->str_indices != NULL)
	{
	  free (stbl->str_indices);
	}
  free (stbl);
}

int
strtbl_put (strtbl* stbl, char* string)
{
  if (stbl == NULL)
	{
	  return -1;
	}

  if (stbl->buffer == NULL)
	{
	  stbl->buffer = malloc ((strlen (string) + 1) * sizeof (char));
	  stbl->str_indices = malloc (sizeof (int));
	}
  else
	{
	  char* tmp = realloc (stbl->buffer,
						   (stbl->buffer_length + strlen (string) + 1));
	  if (tmp == NULL)
		{
		  tmp = malloc (stbl->buffer_length + strlen (string) + 1);
		  int i;
		  for (i = 0; i < stbl->buffer_length; ++i)
			{
			  tmp[i] = stbl->buffer[i];
			}
		  free (stbl->buffer);
		}
	  stbl->buffer = tmp;

	  /// @todo This is stupid...
	  int* tmpi = realloc (stbl->str_indices,
						   (stbl->n_strings + 1) * sizeof (int));
	  if (tmpi == NULL)
		{
		  tmpi = malloc ((stbl->n_strings + 1) * sizeof (int));
		  int i;
		  for (i = 0; i < stbl->n_strings; ++i)
			{
			  tmpi[i] = stbl->str_indices[i];
			}
		  free (stbl->str_indices);
		}
	  stbl->str_indices = tmpi;
	}

  /* Copy the string onto the end. */
  strcpy ((char*) ((size_t) stbl->buffer + stbl->buffer_length + 1), string);
  if (stbl->buffer_length == 0)
	{
	  stbl->str_indices[stbl->n_strings] = 0;
	}
  else
	{
	  stbl->str_indices[stbl->n_strings] = stbl->buffer_length;
	}

  /* Increment counters. */
  ++stbl->n_strings;
  stbl->buffer_length += strlen (string) + 1;
  stbl->buffer[stbl->buffer_length] = ' ';

  return stbl->str_indices[stbl->n_strings - 1];
}

int
strtbl_contains_value (strtbl* stbl, char* string)
{
  int i;
  int length = strlen (string);
  int start = 0;
  for (i = 0; i < stbl->n_strings; ++i)
	{
	  if (start + length >= stbl->buffer_length)
		{
		  return 0;
		}
	  if (strncmp ((char*) (stbl->buffer + start), string, length) == 0)
		{
		  return 1;
		}
	  while (stbl->buffer[start] != ' ')
		{
		  ++start;
		}
	  ++start;
	}
  return 0;
}

void
strtbl_print (strtbl* stbl)
{
  int i;
  /* Skip the trailing space... */
  for (i = 0; i < stbl->buffer_length - 1; ++i)
	{
	  printf ("%c", stbl->buffer[i]);
	}
  /* Finish with newline... */
  printf ("\n");
}
