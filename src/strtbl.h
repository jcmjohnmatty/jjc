#ifndef __STRTBL_H_
#define __STRTBL_H_

/**
 * @brief A simple string table representation.
 */
struct strtbl
{
  /** The buffer to store the strings in the buffer. */
  char* buffer;
  /** The length of <code>buffer</code>. */
  int buffer_length;
  /** The indices of the separate strings in this strtbl. */
  int* str_indices;
  /** The number of strings in this string table. */
  int n_strings;
};

typedef struct strtbl strtbl;

/**
 * @brief Create a new string table.
 * @return A pointer to the new string table.
 */
strtbl*
strtbl_new (void);

/**
 * @brief Deallocate a string table.
 * @param stbl The string table to deallocate.
 */
void
strtbl_delete (strtbl* stbl);

/**
 * @brief Insert a string into a string table.
 * @param stbl The string table to insert a string into.
 * @param string The string to insert.
 * @return The index of string in stbl.
 */
int
strtbl_put (strtbl* stbl, char* string);

#endif /* __STRTBL_H_ */
