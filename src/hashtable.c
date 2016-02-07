#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hashtable.h>

int
htable_hash (void* k, int len)
{
  int i;
  int hashed = 0;
  for (i = 0; i < len; ++i)
    {
	  hashed &= k[len];
    }
  hashed = hashed % len;
  return hashed;
}

hashtable*
htable_new ()
{
  hashtable* htable = malloc (sizeof (hashtable));
  htable->entries = malloc (sizeof (entry) * 11);
  int i;
  for (i = 0; i < 11; i++)
	{
	  htable->entries[i] = NULL;
	}
  htable->threshold = 11;
  /* Default load factor is 0.75f. */
  htable->load_factor = 0.75 f;
  htable->keys = ubaNew ();
  htable->values = ubaNew ();

  return htable;
}

void
htable_delete (hashtable* htable)
{
  entry* e0;
  int sz = htable->count;
  int i = 0;
  do
	{
	  e0 = htable->entries[i];
	  entry* e1 = e0;
	  do
		{
		  entry* e2 = e1;
		  e1 = e1->next;
		  free (e2->key);
		  free (e2->value);
		  free (e2);
		}
	  while (e1 != NULL);
	}
  while (++i < sz);

  ubaDelete (htable->keys);
  ubaDelete (htable->values);

  free (htable);
}

bool
htable_contains_key (hashtable* htable, void* k)
{
  int h = htable_hash (k, htable->threshold);
  entry* e = htable->entries[h];
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

bool
htable_contains_value (hashtable* htable, void* v)
{
  int i;
  int sz = htable->count;
  for (i = 0; i < sz; i++)
	{
	  entry* e;
	  for (e = htable->entries[i]; e != NULL; e = e->next)
		{
		  if (e->value == v)
			{
			  return 1;
			}
		}
	}
  return 0;
}

void*
htable_get (hashtable* htable, void* k)
{
  int h = htable_hash (k, htable->threshold);
  entry* e = htable->entries[h];
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

bool
htable_put (hashtable* htable, void* k, void* v)
{
  if (!ubaContains(htable->keys, k))
	{
	  ubaAdd(htable->keys, k);
	}
  if (!ubaContains(htable->values, v))
	{
	  ubaAdd(htable->values, v);
	}

  int h = htable_hash (k, htable->threshold);

  entry* e = htable->entries[h];
  if (e == NULL)
	{
	  printf ("1\n");
	  e = malloc (sizeof (entry));
	  e->key = k;
	  e->value = v;
	  e->hash = h;
	  e->next = NULL;
	  htable->entries[h] = e;
	  htable->count++;
	}
  else
	{
	  entry* got = htable_get (htable, k);
	  if (got != NULL)
		{
		  printf ("2%s\n", (char*) got->key);
		  got->value = *(int*) got->value + *(int*) v;
		  htable->count++;
		}
	  else
		{
		  printf ("3\n");
		  entry* newe = malloc (sizeof (entry));
		  newe->key = k;
		  newe->value = v;
		  newe->hash = h;
		  newe->next = e;
		  htable->entries[h] = e;
		  htable->count++;
		}
	}
  return 1;
}

//TODO add remove from uba keys and uba values
bool
htable_remove (hashtable * htable, void* k)
{
  int h = htable_hash (k, htable->threshold);
  entry * e = htable->entries[h];
  if (e != NULL)
	{
	  if (e->key == k)
		{
		  entry * next = e->next;
		  free (e->key);
		  free (e->value);
		  free (e);
		  htable->entries[h] = next;
		  return 1;
		}
	  while (e->next != NULL)
		{
		  if (e->next->key == k)
			{
			  free (e->next->key);
			  free (e->next->value);
			  free (e->next);
			  e->next = e->next->next;
			  return 1;
			}
		}
	}
  return 0;
}
