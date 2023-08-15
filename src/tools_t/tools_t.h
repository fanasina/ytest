#ifndef __TOOLS_T_C_H__
#define __TOOLS_T_C_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TYPE_CHAR char
#define TYPE_U_CHAR unsigned char
#define TYPE_INT int
#define TYPE_U_INT unsigned int
#define TYPE_L_INT long int
#define TYPE_U_L_INT unsigned long int
#define TYPE_SIZE_T size_t
#define TYPE_FLOAT float
#define TYPE_DOUBLE double
#define TYPE_L_DOUBLE long double
#define TYPE_STRING char*

#define FREE(x)  { free((x)); (x) = NULL;} 

#define FOREACH(array, size, function)\
  for(size_t _ind = 0; _ind < size; ++_ind) function(array[_ind]);


#define GENERATE_ALL(type)\
  int COMPARE_N_##type(const void *,const    void*);\
  void COPY_ARRAY_##type(type* dst, const type* src, size_t size);\
  type MAX_ARRAY_##type(const  type *array, size_t size);\
  size_t ARG_MAX_ARRAY_##type(const type *array, size_t size);\
  type MIN_ARRAY_##type(const  type *array, size_t size);\
  size_t ARG_MIN_ARRAY_##type(const  type *array, size_t size);\
  TYPE_STRING type##_TO_STR(type var);\
  

GENERATE_ALL(TYPE_CHAR)
GENERATE_ALL(TYPE_U_CHAR)
GENERATE_ALL(TYPE_INT)
GENERATE_ALL(TYPE_U_INT)
GENERATE_ALL(TYPE_L_INT)
GENERATE_ALL(TYPE_U_L_INT)
GENERATE_ALL(TYPE_SIZE_T)
GENERATE_ALL(TYPE_FLOAT)
GENERATE_ALL(TYPE_DOUBLE)
GENERATE_ALL(TYPE_L_DOUBLE)
GENERATE_ALL(TYPE_STRING)

#endif /*__TOOLS_T_C_H__*/

