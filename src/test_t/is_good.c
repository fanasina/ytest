#include <stdio.h>
#include <stdlib.h>

#include "src/permutation_t/permutation_t.h"


int main(){
  PERMUTATION_TYPE_CHAR *p = CREATE_PERMUTATION_TYPE_CHAR(3);
  
  printf(" size = %u \n",p->size);

  return 0;
}
