#ifndef __PERMUTATION_T_C_H__
#define __PERMUTATION_T_C_H__

#include "src/tools_t/tools_t.h"
#include "src/set_theoric_t/set_theoric_t.h"

#define STRUCT_PERMUTATION(type)\
  struct PERMUTATION_##type{\
    size_t size;\
    type * perm; };
STRUCT_PERMUTATION(TYPE_CHAR)
STRUCT_PERMUTATION(TYPE_U_CHAR)
STRUCT_PERMUTATION(TYPE_INT)
STRUCT_PERMUTATION(TYPE_U_INT)
STRUCT_PERMUTATION(TYPE_L_INT)
STRUCT_PERMUTATION(TYPE_U_L_INT)
STRUCT_PERMUTATION(TYPE_FLOAT)
STRUCT_PERMUTATION(TYPE_DOUBLE)
STRUCT_PERMUTATION(TYPE_L_DOUBLE)
STRUCT_PERMUTATION(TYPE_STRING)

typedef struct PERMUTATION_TYPE_CHAR PERMUTATION_TYPE_CHAR;
typedef struct PERMUTATION_TYPE_U_CHAR PERMUTATION_TYPE_U_CHAR;
typedef struct PERMUTATION_TYPE_INT PERMUTATION_TYPE_INT;
typedef struct PERMUTATION_TYPE_U_INT PERMUTATION_TYPE_U_INT;
typedef struct PERMUTATION_TYPE_L_INT PERMUTATION_TYPE_L_INT;
typedef struct PERMUTATION_TYPE_U_L_INT PERMUTATION_TYPE_U_L_INT;
typedef struct PERMUTATION_TYPE_FLOAT PERMUTATION_TYPE_FLOAT;
typedef struct PERMUTATION_TYPE_DOUBLE PERMUTATION_TYPE_DOUBLE;
typedef struct PERMUTATION_TYPE_L_DOUBLE PERMUTATION_TYPE_L_DOUBLE;
typedef struct PERMUTATION_TYPE_STRING PERMUTATION_TYPE_STRING;

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

#endif /*__PERMUTATION_T_C_H__*/
