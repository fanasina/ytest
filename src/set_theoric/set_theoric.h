#ifndef __SET_THEORIC_C__H
#define __SET_THEORIC_C__H

#include <stdlib.h>

#include "src/tools/tools.h"

struct set_theoric
{
  unsigned int id;
  unsigned int *set;
};

typedef set_theoric set_theoric;

set_theoric * create_set_theoric(unsigned int id);

bool is_set_theoric(set_theoric *st);



#endif /*__SET_THEORIC_C__H*/
