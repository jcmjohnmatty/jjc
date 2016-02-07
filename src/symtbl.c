#include <stdlib.h>
#include <string.h>

#include <symtbl.h>

int
symtbl_hash (int k, int len)
{
  return k % len;
}

symtbl*
symtbl_new ()
{
  symtbl* stbl = malloc (sizeof (symtbl));
  stbl->entries = malloc (sizeof (entry) * 11);
  int i;
  for (i = 0; i < 11; i++)
    {
      stbl->entries[i] = NULL;
    }
  stbl->threshold = 11;
  /* Default load factor is 0.75f. */
  stbl->load_factor = 0.75f;
  stbl->keys = ubaNew ();
  stbl->values = ubaNew ();

  return stbl;
}

void
symtbl_delete (symtbl* stbl)
{
  entry* e0;
  int sz = stbl->count;
  int i = 0;
  do
    {
      e0 = stbl->entries[i];
      entry* e1 = e0;
      while (e1 != NULL)
        {
          entry* e2 = e1;
          e1 = e1->next;
          free (e2->value);
          free (e2);
        }
    }
  while (++i < sz);

  ubaDelete (stbl->keys);
  ubaDelete (stbl->values);

  free (stbl);
}

int
symtbl_contains_key (symtbl* stbl, int k)
{
  int h = symtbl_hash (k, stbl->threshold);
  entry* e = stbl->entries[h];
  if (e == NULL)
    {
      return 0;
    }
  else
    {
      do
        {
          if (e->key == k)
            {
              return 1;
            }
        }
      while ((e = e->next) != NULL);
    }
  return 0;
}

int
symtbl_contains_value (symtbl* stbl, char* v)
{
  int i;
  int sz = stbl->count;
  for (i = 0; i < sz; i++)
    {
      entry* e;
      for (e = stbl->entries[i]; e != NULL; e = e->next)
        {
          if (e->value == v)
            {
              return 1;
            }
        }
    }
  return 0;
}

char*
symtbl_get (symtbl* stbl, int k)
{
  int h = symtbl_hash (k, stbl->threshold);
  entry* e = stbl->entries[h];
  if (e != NULL)
    {
      while ((e = e->next) != NULL)
        {
          if (e->key == k)
            {
              return e->value;
            }
        }
    }
  return NULL;
}

int
symtbl_put (symtbl* stbl, int k, char* v)
{
  int h = symtbl_hash (k, stbl->threshold);

  entry* e = stbl->entries[h];
  if (e == NULL)
    {
      e = malloc (sizeof (entry));
      e->key = k;
      e->value = malloc (strlen (v) * sizeof (char));
      strcpy (e->value, v);
      e->hash = h;
      e->next = NULL;
      stbl->entries[h] = e;
      stbl->count++;
    }
  else
    {
      entry* got = symtbl_get (stbl, k);
      if (got != NULL)
        {
          got->value = *(char*) got->value + *(char*) v;
          stbl->count++;
        }
      else
        {
          entry* newe = malloc (sizeof (entry));
          newe->key = k;
          newe->value = malloc (strlen (v) * sizeof (char));
          strcpy (newe->value, v);
          newe->hash = h;
          newe->next = e;
          stbl->entries[h] = e;
          stbl->count++;
        }
    }
  return 1;
}

//TODO add remove from uba keys and uba values
int
symtbl_remove (symtbl* stbl, int k)
{
  int h = symtbl_hash (k, stbl->threshold);
  entry* e = stbl->entries[h];
  if (e != NULL)
    {
      if (e->key == k)
        {
          entry* next = e->next;
          free (e->value);
          free (e);
          stbl->entries[h] = next;
          return 1;
        }
      while (e->next != NULL)
        {
          if (e->next->key == k)
            {
              free (e->next->value);
              free (e->next);
              e->next = e->next->next;
              return 1;
            }
        }
    }
  return 0;
}
