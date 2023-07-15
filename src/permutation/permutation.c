#include "src/permutation/permutation.h"

permutation*
create_permutation(size_t sz)
{
  if(sz == 0) return NULL;
  permutation *p=malloc(sizeof(permutation));
  p->size = sz;
  p->perm = malloc(sz*sizeof(unsigned int));
}

/*void
copy_array_unsigned(unsigned int *dst, const unsigned int *src, size_t size)
{
  for(size_t i = 0; i < size ; ++i)
    dst[i]=src[i];
}*/

void
assign_permutation(permutation *p, unsigned int *arr)
{
  copy_array_unsigned(p->perm, arr, p->size);
}

bool
is_permutation_set_theoric(const permutation *p)
{
  if(p==NULL) return false;
  size_t size = p->size, j;
  unsigned int *count_array_i = calloc(size, sizeof(unsigned int));
  if(count_array_i == NULL)
    { 
      printf("can't allocate count_array_i\n");
      return false;
    }
  for(size_t i = 0; i < size; ++i)
    {
      j = p->perm[i];
      if((j >= size) || count_array_i[j])
        { 
          free(count_array_i);
          return false;
        }
      count_array_i[j]++;
    }
  return true;
}
/* 2,7,4,1 is a permutation of 1,2,4,7
 *it is equivalent of 1,3,2,0 in set_theoric(4)=0,1,2,3
 this function calculate the permutation equivalent in set_theoric
 * */
permutation *
translate_set_theoric(const permutation *p, permutation *translate_p)
{
  if(p==NULL) return NULL;
  size_t size = p->size;
  permutation *translate_p = create_permutation(size);
  unsigned int *temperm = malloc(size*sizeof(unsigned int));
  unsigned int *tmperm = malloc(size*sizeof(unsigned int));
  copy_array_unsigned_int(tmperm, p->perm, p->size); // copy
  qsort(tmperm, size, sizeof(unsigned int), compare_unsigned_int);
  // tmperm contain p->perm ordered

  size_t cur=0;
  for(size_t i=0; i< size; ++i)
    {
      for(size_t j=0; j<size; ++j)
        {
          if(p->perm[j] == tmperm[i])
            {
              bool found = false;
              for(size_t c=0; c<cur; ++c)
                {
                  if(j==temperm[c])
                    {
                      found = true;
                      break;
                    }
                }
              if(found == false)
                {
                  translate_p->perm[i]=j;
                  temperm[cur++]=j;
                  break;
                }
            }
        }
    }
  free(tmperm);
  free(temperm);
  return translate_p;
}
  
bool
is_permutation(const permutation *p)
{
  bool ret = is_permutation_set_theoric(p);
  if(ret == false)
    {
      permutation *t_p = translate_set_theoric(p);
      ret = is_permutation_set_theoric(t_p);
      free(t_p);
    }
  return ret;
}
