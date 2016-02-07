#ifndef __SYMTBL_H_
#define __SYMTBL_H_

#include <uba.h>

/**
 * @brief A struct representing an entry in a symtbl.
 */
struct entry
{
  /** The value of the hash function. */
  int hash;

  /** The key. */
  int key;

  /** The value. */
  char* value;

  /** The next entry. */
  struct entry* next;
};

typedef struct entry entry;

/**
 * @brief A symtbl implementation.
 */
struct symtbl
{
  /** An array storing all of the entries. */
  entry** entries;

  /** The total number of entries. */
  unsigned count;

  /** Entries is resized when the size exceeds the load factor. */
  int threshold;

  /** The load factor. */
  float load_factor;

  /**
   * The keys (less memory efficent than iterating through entries, but it is
   * faster.
   */
  uba* keys;

  /** @see {keys} */
  uba* values;
};

typedef struct symtbl symtbl;

/**
 * @brief The function to use when hashing.
 * Use the default function, but the hash function could be changed.
 * CHANGE THE HASH FUNCTION BEFORE INSERTING VALUES IF YOU PLAN TO!
 * @param k the key to hash.
 * @param len the size of the key.
 * @return the hashcode of k given size sz.
 */
int
symtbl_hash (int k, int len);

/**
 * @brief Create a new symtbl.
 * @return A pointer to the new symtbl.
 */
symtbl*
symtbl_new ();

/**
 * @brief Free all of the memmory in a symtbl.
 * @param stbl The symtbl to free the memory of.
 */
void
symtbl_delete (symtbl* stbl);

/**
 * @brief Check if a hastable contains a key.
 * @param stbl a pointer to the symtbl to check in.
 * @param k the key to check for in the symtbl.
 * @return true if k is a key in stbl and false otherwise.
 */
int
symtbl_contains_key (symtbl* stbl, int k);

/**
 * @brief Check if a symtbl contains a value.
 * @param stbl a pointer to the symtbl to check in.
 * @param v the value to check for in the symtbl.
 * @return true if v is a value in stbl and false otherwise.
 */
int
symtbl_contains_value (symtbl* stbl, char* v);

/**
 * @brief Get a value assosiated with a key.
 * @param stbl a pointer to the symtbl to get from
 * @param k the key to get the value assosiated with
 * @return the value assosiated with key, or NULL if no value is assisiated with
 * v
 */
char*
symtbl_get (symtbl* stbl, int k);

/**
 * @brief Put a value assosiated with a key.
 * @param stbl a pointer to the stbl to insert into.
 * @param k the key.
 * @param v the value.
 * @return true on success, and false otherwise.
 */
int
symtbl_put (symtbl* stbl, int k, char* v);

/**
 * @brief Remove a value assisiated with a key.
 * @param stbl a pointer to the symtbl to remove from.
 * @param k the key of the (key, value) pair to remove.
 * @return true on success.
 */
int
symtbl_remove (symtbl* stbl, int k);

#endif  /* __SYMTBL_H_ */
