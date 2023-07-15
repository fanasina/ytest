#ifndef __PERMUTATION_C_H__
#define __PERMUTATION_C_H__

#include "src/set_theoric/set_theoric.h"

/* struct of permutation of unsigned int  array, not necessarly set_theoric
 *
 * */
struct permutation
{
  size_t size;
  unsigned int *perm;
};

typedef struct permutation permutation;

permutation * create_permutation(size_t sz);
void assign_permutation(permutation *p, unsigned int *arr);

bool is_permutation_set_theoric(const permutation *p);

// more general! need translation and use is_permutation_set_theoric
bool is_permutation(const permutation *p);

#endif /*__PERMUTATION_C_H__*/
