
#include "src/set_theoric/set_theoric.h"

set_theoric * create_set_theoric(unsigned int id)
{
  if(id == 0) return NULL;
    
  set_theoric *ret_set = malloc(sizeof(set_theoric));
  ret_set.set=malloc(id*sizeof(unsigned int));
  ret_set.id = id;
  for(int i = 0; i < id; ++i)
    ret_set.set[i] = i;

  return ret_set;

}

bool is_set_theoric(set_theoric *st)
{
  if(st == NULL) return true;

  for(int i = 0; i < st->id; ++i)
    if(st->set[i] != i) return false;

  return true;
}
