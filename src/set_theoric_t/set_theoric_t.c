
#include "src/set_theoric_t/set_theoric_t.h"

#define CREATE_SET_THEORIC(type, id)\
  type * CREATE_SET_THEORIC_##type(type id){\
    if(id == 0) return NULL;\
    SET_THEORIC_##type *ret_set = malloc(sizeof(SET_THEORIC_##type));\
    ret_set->id = id;\
    ret_set->set = malloc(id*sizeof(type));\
    for(type i = 0; i < id; ++i) ret_set->set[i]=i;\
  return ret_set; }
CREATE_SET_THEORIC(TYPE_U_CHAR)
CREATE_SET_THEORIC(TYPE_U_INT)
CREATE_SET_THEORIC(TYPE_U_LONG_INT)

#define IS_SET_THEORIC(type, st)\
    bool IS_SET_THEORIC_##type(type *st){\
      for(type i = 0; i < st->id; ++i){\
        if(st->set[i] != i) return false;\
      return true; }
IS_SET_THEORIC(TYPE_U_CHAR,st)
IS_SET_THEORIC(TYPE_U_INT,st)
IS_SET_THEORIC(TYPE_U_LONG_INT,st)

