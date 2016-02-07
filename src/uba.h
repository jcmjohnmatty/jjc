#ifndef UBA_H
#define UBA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

typedef struct uba {
	void** data;
	int length;
} uba;

/**
 * create a new empty uba with inital size 15
 * @return a new empty uba
 */
uba* ubaNew(void);

/**
 * free the memory used for an uba
 * @param u the uba to delete
 */
void ubaDelete(uba* u);

/**
 * add an void**ent to the end of the list
 * @param u the uba to add to
 * @param el the void**ent to add
 */
void ubaAdd(uba* u, void** el);

/**
 * add an void**ent to the specified index
 * @param u the uba to add to
 * @param index the index to add at
 * @param el the void**ent to add
 */
void ubaAddIndex(uba* u, int index, void** el);

/**
 * add all the contents of one uba to the end of another
 * @param usrc the uba that's contents will be added
 * @param udst the uba that will be appended onto
*/
void ubaAddAll(uba* usrc, uba* udst);

/**
 * add all the contents of one uba to another starting at an index
 * @param usrc the uba that's contents will be added
 * @param udst the uba that will be appended onto
 * @param index the index to start adding at in udst
 */
void ubaAddAllIndex(uba* usrc, uba* udst, int index);

/**
 * clear the contents of an uba
 * @param u the uba to clear the contents of
 */
void ubaClear(uba* u);

/**
 * check if an uba contains an void**ent
 * @param u the uba to check in
 * @param el the void**ent to check for in u
 * @return nonzero if u contains el
 */
int ubaContains(uba* u, void** el);

/**
 * check if an uba contains another uba
 * @param uout the uba to check in
 * @param uin the (sub)uba to check for in uout
 * @return nonzero if uin is in uout
 */
int ubaContainsAll(uba* uout, uba* uin);

/**
 * check if two ubas are ==
 * @pararm u1 the first uba
 * @param u2 the second uba
 * @return nonzero if the ubas are ==
 */
int ubaEquals(uba* u1, uba* u2);

/**
 * get an item at an index in the uba
 * @param u the uba to get from
 * @param index the index of the item to get
 * @return u[index]
 */
void** ubaGet(uba* u, int index);

/**
 * get the index of an void**ent
 * @param u the uba to get the index from
 * @param el the void**ent to get the index of in u
 * @return the index of el in u, or -1 if el is not found in u
 */
int ubaIndexOf(uba* u, void** el);

/**
 * get the last index of an void**ent
 * @param u the uba to get the index from
 * @param el the void**ent to get the last index of in u
 * @return the last index of el in u, or -1 if el is not found in u
 */
int ubaLastIndexOf(uba* u, void** el);

/**
 * remore an index form the uba
 * @param u the uba to remove from
 * @param index the index to remove
 * @return the void**ent that was removed, or (void***)0 if the void**ent was not
 * found
 */
void** ubaRemove(uba* u, int index);

/**
 * remove the first occurance of an void**ent from the uba
 * @param u the uba to remove from
 * @param el the void**ent to remove
 * @return nonzero if the remove was successful
 */
int ubaRemoveElement(uba* u, void** el);

/**
 * set an index to an void**ent
 * @param u the uba to set an index
 * @param index the index to set
 * @param el the void**ent to set u[index] to
 * @return whatever was at u[index] before executing u[index] = el
 */
void** ubaSet(uba* u, int index, void** el);

/**
 * get the length of an uba
 * @param u the uba to get the length of
 * @return the length of u
 */
int ubaLength(uba* u);

#ifdef __cplusplus
}
#endif

#endif	// uba.h

