#include <strlib.h>
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

  realloc (stbl->buffer, stbl->buffer_length + strlen (string) + 1);
  /// @todo This is stupid...
  realloc (stbl->str_indices, stbl->n_strings + 1);

  /* Copy the string onto the end. */
  stbl->buffer[stbl->buffer_length] = ' ';
  strcpy ((char*) (stbl->buffer + stbl->buffer_length + 1), string);
  if (stbl->buffer_length == 0)
	{
	  stbl->str_indices[stbl->n_strings] = 0;
	}
  else
	{
	  stbl->str_indices[stbl->n_strings] = stbl->buffer_length + 1;
	}

  /* Increment counters. */
  ++stbl->n_strings;
  stbl->buffer_length += strlen (string) + 1;
}
