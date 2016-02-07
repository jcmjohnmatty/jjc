#include "uba.h"
#include <stdlib.h>
#ifdef __cplusplus
extern		"C" {
#endif

	//not visible outside of this file
	void		ubaEnsureCapacity(uba * u, int newLength){
		if (sizeof(u->data) / sizeof(u->data[0]) >= newLength) {
			void          **oldData = u->data;
					u           ->data = malloc(2 * newLength);
			int		i;
			for             (i = 0; i < sizeof(oldData) / sizeof(oldData[0]); i++) {
				u->data[i] = oldData[i];
			}
			free(oldData);
		}
	}

	uba            *ubaNew() {
		uba            *newUba = malloc(sizeof(uba));
				newUba      ->length = 0;
				newUba      ->data = malloc(sizeof(void **) * 15);
				return	newUba;
	}

	void		ubaDelete (uba * u) {
		free(u->data);
		free(u);
	}

	void		ubaAdd    (uba * u, void **el){
		ubaEnsureCapacity(u, u->length + 1);
		u->data[u->length] = el;
		u->length++;
	}

	void		ubaAddIndex(uba * u, int index, void **el){
		ubaEnsureCapacity(u, u->length + 1);
		int		i;
		for             (i = u->length; i > index; i--) {
			u->data[i] = u->data[i - 1];
		}
				u           ->data[index] = el;
		u->length++;
	}

	void		ubaAddAll (uba * usrc, uba * udst) {
		ubaEnsureCapacity(udst, usrc->length + udst->length);
		int		i;
		for             (i = udst->length; i < usrc->length + udst->length; i++) {
			udst->data[i] = usrc->data[i - udst->length];
		}
				udst        ->length += usrc->length;
	}

	void		ubaAddAllIndex(uba * usrc, uba * udst, int index){
		ubaEnsureCapacity(udst, udst->length + usrc->length);
		int		i;
		for             (i = udst->length + usrc->length; i > index; i--) {
			udst->data[i] = udst->data[i - usrc->length];
		}
		for             (i = index; i < usrc->length; i++) {
			udst->data[i] = usrc->data[i - usrc->length];
		}
		udst->length += usrc->length;
	}

	void		ubaClear  (uba * u) {
		free(u->data);
		u->length = 0;
	}

	int		ubaContains(uba * u, void **el){
		return ubaIndexOf(u, el);
	}

	int		ubaContainsAll(uba * uout, uba * uin) {
		if (uout->length < uin->length) {
			return 0;
		}
		int		i;
		for             (i = 0; i < uout->length; i++) {
			if (uout->data[i] == uin->data[0]) {
				int		j;
				for             (j = i; j < uin->length; j++) {
					if (uout->data[j] != uin->data[j - i]) {
						break;
					}
				}
				if              (uout->data[j] == uin->data[j - 1]) {
					return 1;
				}
			}
		}
		return -1;
	}

	int		ubaEquals  (uba * u1, uba * u2) {
		if (u1->length != u2->length) {
			return 0;
		}
		int		i;
		for             (i = 0; i < u1->length; i++) {
			if (u1->data[i] != u2->data[i]) {
				return 0;
			}
		}
				return	1;
	}

	void          **ubaGet(uba * u, int index){
		return u->data[index];
	}

	int		ubaIndexOf (uba * u, void **el){
		int		i;
		for             (i = 0; i < u->length; i++) {
			if (u->data[i] == el) {
				return i;
			}
		}
				return       -1;
	}

	int		ubaLastIndexOf(uba * u, void **el){
		int		i;
		for             (i = u->length - 1; i >= 0; i--) {
			if (u->data[i] == el) {
				return i;
			}
		}
				return       -1;
	}

	void          **ubaRemove(uba * u, int index){
		void          **data = u->data[index];
		int		i;
		for             (i = index; i < u->length; i++) {
			u->data[i] = u->data[i + 1];
		}
				u           ->length--;
		return data;
	}

	int		ubaRemoveElement(uba * u, void **el){
		int		indexOfEl = ubaIndexOf(u, el);
		if              (indexOfEl < 0) {
			return 0;
		} else {
			ubaRemove(u, indexOfEl);
			return indexOfEl;
		}
	}

	void          **ubaSet(uba * u, int index, void **el){
		void          **elold = u->data[index];
				u           ->data[index] = el;
				return	elold;
	}

	int		ubaLength  (uba * u) {
		return u->length;
	}

#ifdef __cplusplus
}
#endif
