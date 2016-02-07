#ifndef __HASHTABLE_H_
#define __HASHTABLE_H_

#include <uba.h>

/**
 * @brief A struct representing an entry in a hashtable.
 */
struct entry
{
  /** The value of the hash function. */
  int hash;

  /** The key. */
  void* key;

  /** The value. */
  void* value;

  /** The next entry. */
  struct entry* next;
};

typedef struct entry entry;

/**
 * @brief A hashtable implementation.
 */
struct hashtable
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

typedef struct hashtable hashtable;

/**
 * @brief The function to use when hashing.
 * Use the default function, but the hash function could be changed.
 * CHANGE THE HASH FUNCTION BEFORE INSERTING VALUES IF YOU PLAN TO!
 * @param k the key to hash.
 * @param len the size of the key.
 * @return the hashcode of k given size sz.
 */
int
htable_hash (void* k, int len);

/**
 * @brief Create a new hashtable.
 * @return A pointer to the new hashtable.
 */
hashtable*
htable_new ();

/**
 * @brief Free all of the memmory in a hashtable.
 * @param htable The hashtable to free the memory of.
 */
void
htable_delete (hashtable* htable);

/**
 * @brief Check if a hastable contains a key.
 * @param htable a pointer to the hashtable to check in.
 * @param k the key to check for in the hashtable.
 * @return true if k is a key in htable and false otherwise.
 */
int
htable_contains_key (hashtable* htable, void* k);

/**
 * @brief Check if a hashtable contains a value.
 * @param htable a pointer to the hashtable to check in.
 * @param v the value to check for in the hashtable.
 * @return true if v is a value in htable and false otherwise.
 */
int
htable_contains_value (hashtable* htable, void* v);

/**
 * @brief Get a value assosiated with a key.
 * @param htable a pointer to the hashtable to get from
 * @param k the key to get the value assosiated with
 * @return the value assosiated with key, or NULL if no value is assisiated with
 * v
 */
void*
htable_get (hashtable* htable, void* k);

/**
 * @brief Put a value assosiated with a key.
 * @param htable a pointer to the htable to insert into.
 * @param k the key.
 * @param v the value.
 * @return true on success, and false otherwise.
 */
int
htable_put (hashtable* htable, void* k, void* v);

/**
 * @brief Remove a value assisiated with a key.
 * @param htable a pointer to the hashtable to remove from.
 * @param k the key of the (key, value) pair to remove.
 * @return true on success.
 */
int
htable_remove (hashtable* htable, void* k);

/**
 * @brief Get all keys in a hashtable.
 * @param htable a pointer to the hashtable to get keys from.
 * @return the keys in htable.
 */
void**
htable_get_keys (hashtable* htable);

/**
 * @brief Get all values in a hastable.
 * @param htable a pointer to the hashtable to get values from.
 * @return the values in htable.
 */
void**
htable_get_values (hashtable* htable);

#endif  /* __HASHTABLE_H_ */
