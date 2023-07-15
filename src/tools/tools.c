#include "src/tools/tools.h"

int 
compare_int(void* a, void* b)
{ 
  return (*(int*)a - *(int*)b);
}
int 
compare_unsigned_int(void* a, void* b)
{
  return ((unsigned int*)a-*(unsigned int*)b);
}
int
compare_float(void *a, void *b)
{
  if (*(float*)a == *(float*)b) return 0;
  if (*(float*)a > *(float*)b) return 1;
  return -1;
}
int
compare_double(void *a, void *b)
{
  if (*(double*)a == *(double*)b) return 0;
  if (*(double*)a > *(double*)b) return 1;
  return -1;
}
int
compare_string(void *a, void *b)
{
  return strcmp((char*)a, (char*)b);
}

int
max_array_int(int * arr, size_t sz)
{
  if(sz == 0) return 0;
  int mx = arr[0];
  for(size_t i = 1; i < sz; ++i)
    if(mx < arr[i]) mx = arr[i];

  return mx;
}

unsigned int
max_array_unsigned_int(unsigned int *arr, size_t sz)
{
  if(sz == 0) return 0;
  unsigned int mx = arr[0];
  for(size_t i = 1; i < sz; ++i)
    if(mx < arr[i]) mx = arr[i];

  return mx;
}
int
min_array_int(int * arr, size_t sz)
{
  if(sz == 0) return 0;
  int mn = arr[0];
  for(size_t i = 1; i < sz; ++i)
    if(mn > arr[i]) mn = arr[i];

  return mn;
}
unsigned int
min_array_unsigned_int(unsigned int *arr, size_t sz)
{
  if(sz == 0) return 0;
  unsigned int mn = arr[0];
  for(size_t i = 1; i < sz; ++i)
    if(mn > arr[i]) mn = arr[i];

  return mn;
}

size_t
arg_max_array_int(int * arr, size_t sz)
{
  if(sz == 0) return 0;
  size_t i_mx = 0;
  for(size_t i = 1; i < sz; ++i)
    if(arr[i_mx] < arr[i]) i_mx = i;

  return i_mx;
}

size_t
arg_max_array_unsigned_int(unsigned int *arr, size_t sz)
{
  if(sz == 0) return 0;
  size_t i_mx = 0;
  for(size_t i = 1; i < sz; ++i)
    if(arr[i_mx] < arr[i]) i_mx = i;

  return i_mx;
}

size_t
arg_min_array_int(int * arr, size_t sz)
{
  if(sz == 0) return 0;
  size_t i_mn = 0;
  for(size_t i = 1; i < sz; ++i)
    if(arr[i_mn] > arr[i]) i_mn = i;

  return i_mn;
}

size_t
arg_min_array_unsigned_int(unsigned int *arr, size_t sz)
{
  if(sz == 0) return 0;
  size_t i_mn = 0;
  for(size_t i = 1; i < sz; ++i)
    if(arr[i_mn] > arr[i]) i_mn = i;

  return i_mn;
}

void
copy_array_unsigned_int(unsigned int *dst, const unsigned int *src, size_t size)
{
  for(size_t i=0; i< size; ++i)
    dst[i]=src[i];
}

/*
bool is_less_eq_than_i(int a, int b) { return a <= b; }
bool is_less_than_i(int a, int b) { return a < b; }
bool is_great_eq_than_i(int a, int b)  { return a >= b; }
bool is_great_than_i(int a, int b)  { return a > b; }
*/
int incr_i(int i)  { return i + 1; }
int decr_i(int i)  { return i - 1; }

/*
bool is_less_eq_than_u(unsigned int a, unsigned int b) { return a <= b; }
bool is_less_than_u(unsigned int a, unsigned int b) { return a < b; }
bool is_great_eq_than_u(unsigned int a, unsigned int b)  { return a >= b; }
bool is_great_than_u(unsigned int a, unsigned int b)  { return a > b; }
*/
unsigned int incr_u(unsigned int i)  { return i + 1; }
unsigned int decr_u(unsigned int i)  { return i - 1; }


