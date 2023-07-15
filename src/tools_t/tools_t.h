#ifndef __TOOLS_T_C_H__
#define __TOOLS_T_C_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TYPE_CHAR char
#define TYPE_U_CHAR unsigned char
#define TYPE_INT int
#define TYPE_U_INT unsigned int
#define TYPE_L_INT long int
#define TYPE_U_L_INT unsigned long int
#define TYPE_FLOAT float
#define TYPE_DOUBLE double
#define TYPE_L_DOUBLE long double
#define TYPE_STRING char*

#define FREE(x)  { free((x)); (x) = NULL;} 

#define FOREACH(array, size, function)\
  for(size_t _ind = 0; _ind < size; ++_ind) function(array[_ind]);


int COMPARE_N_TYPE_CHAR(const void *,const    void*);
int COMPARE_N_TYPE_U_CHAR(const void *,const    void*);
int COMPARE_N_TYPE_INT(const void *,const    void*);
int COMPARE_N_TYPE_U_INT(const void *,const    void*);
int COMPARE_N_TYPE_L_INT(const void *,const    void*);
int COMPARE_N_TYPE_U_L_INT(const void *,const    void*);
int COMPARE_N_TYPE_FLOAT(const void *,const    void*);
int COMPARE_N_TYPE_DOUBLE(const void *,const    void*);
int COMPARE_N_TYPE_L_DOUBLE(const void *,const    void*);
int COMPARE_N_TYPE_STRING(const void *,const    void*);

void COPY_ARRAY_TYPE_CHAR(TYPE_CHAR* dst, const TYPE_CHAR* src, size_t size); 
void COPY_ARRAY_TYPE_U_CHAR(TYPE_U_CHAR* dst, const TYPE_U_CHAR* src, size_t size); 
void COPY_ARRAY_TYPE_INT(TYPE_INT* dst, const TYPE_INT* src, size_t size); 
void COPY_ARRAY_TYPE_U_INT(TYPE_U_INT* dst, const TYPE_U_INT* src, size_t size); 
void COPY_ARRAY_TYPE_L_INT(TYPE_L_INT* dst, const TYPE_L_INT* src, size_t size); 
void COPY_ARRAY_TYPE_U_L_INT(TYPE_U_L_INT* dst, const TYPE_U_L_INT* src, size_t size); 
void COPY_ARRAY_TYPE_FLOAT(TYPE_FLOAT* dst, const TYPE_FLOAT* src, size_t size); 
void COPY_ARRAY_TYPE_DOUBLE(TYPE_DOUBLE* dst, const TYPE_DOUBLE* src, size_t size); 
void COPY_ARRAY_TYPE_L_DOUBLE(TYPE_L_DOUBLE* dst, const TYPE_L_DOUBLE* src, size_t size); 
void COPY_ARRAY_TYPE_STRING(TYPE_STRING* dst, const TYPE_STRING* src, size_t size); 

TYPE_CHAR MAX_ARRAY_TYPE_CHAR(const  TYPE_CHAR *array, size_t size);
TYPE_U_CHAR MAX_ARRAY_TYPE_U_CHAR(const  TYPE_U_CHAR *array, size_t size);
TYPE_INT MAX_ARRAY_TYPE_INT(const  TYPE_INT *array, size_t size);
TYPE_U_INT MAX_ARRAY_TYPE_U_INT(const  TYPE_U_INT *array, size_t size);
TYPE_L_INT MAX_ARRAY_TYPE_L_INT(const  TYPE_L_INT *array, size_t size);
TYPE_U_L_INT MAX_ARRAY_TYPE_U_L_INT(const  TYPE_U_L_INT *array, size_t size);
TYPE_FLOAT MAX_ARRAY_TYPE_FLOAT(const  TYPE_FLOAT *array, size_t size);
TYPE_DOUBLE MAX_ARRAY_TYPE_DOUBLE(const  TYPE_DOUBLE *array, size_t size);
TYPE_L_DOUBLE MAX_ARRAY_TYPE_L_DOUBLE(const  TYPE_L_DOUBLE *array, size_t size);
TYPE_STRING MAX_ARRAY_TYPE_STRING(const  TYPE_STRING *array, size_t size);

size_t ARG_MAX_ARRAY_TYPE_CHAR(const TYPE_CHAR *array, size_t size);
size_t ARG_MAX_ARRAY_TYPE_U_CHAR(const TYPE_U_CHAR *array, size_t size);
size_t ARG_MAX_ARRAY_TYPE_INT(const TYPE_INT *array, size_t size);
size_t ARG_MAX_ARRAY_TYPE_U_INT(const TYPE_U_INT *array, size_t size);
size_t ARG_MAX_ARRAY_TYPE_L_INT(const TYPE_L_INT *array, size_t size);
size_t ARG_MAX_ARRAY_TYPE_U_L_INT(const TYPE_U_L_INT *array, size_t size);
size_t ARG_MAX_ARRAY_TYPE_FLOAT(const TYPE_FLOAT *array, size_t size);
size_t ARG_MAX_ARRAY_TYPE_DOUBLE(const TYPE_DOUBLE *array, size_t size);
size_t ARG_MAX_ARRAY_TYPE_L_DOUBLE(const TYPE_L_DOUBLE *array, size_t size);
size_t ARG_MAX_ARRAY_TYPE_STRING(const TYPE_STRING *array, size_t size);

TYPE_CHAR MIN_ARRAY_TYPE_CHAR(const  TYPE_CHAR *array, size_t size);
TYPE_U_CHAR MIN_ARRAY_TYPE_U_CHAR(const  TYPE_U_CHAR *array, size_t size);
TYPE_INT MIN_ARRAY_TYPE_INT(const  TYPE_INT *array, size_t size);
TYPE_U_INT MIN_ARRAY_TYPE_U_INT(const  TYPE_U_INT *array, size_t size);
TYPE_L_INT MIN_ARRAY_TYPE_L_INT(const  TYPE_L_INT *array, size_t size);
TYPE_U_L_INT MIN_ARRAY_TYPE_U_L_INT(const  TYPE_U_L_INT *array, size_t size);
TYPE_FLOAT MIN_ARRAY_TYPE_FLOAT(const  TYPE_FLOAT *array, size_t size);
TYPE_DOUBLE MIN_ARRAY_TYPE_DOUBLE(const  TYPE_DOUBLE *array, size_t size);
TYPE_L_DOUBLE MIN_ARRAY_TYPE_L_DOUBLE(const  TYPE_L_DOUBLE *array, size_t size);
TYPE_STRING MIN_ARRAY_TYPE_STRING(const  TYPE_STRING *array, size_t size);

size_t ARG_MIN_ARRAY_TYPE_CHAR(const  TYPE_CHAR *array, size_t size);
size_t ARG_MIN_ARRAY_TYPE_U_CHAR(const  TYPE_U_CHAR *array, size_t size);
size_t ARG_MIN_ARRAY_TYPE_INT(const  TYPE_INT *array, size_t size);
size_t ARG_MIN_ARRAY_TYPE_U_INT(const  TYPE_U_INT *array, size_t size);
size_t ARG_MIN_ARRAY_TYPE_L_INT(const  TYPE_L_INT *array, size_t size);
size_t ARG_MIN_ARRAY_TYPE_U_L_INT(const  TYPE_U_L_INT *array, size_t size);
size_t ARG_MIN_ARRAY_TYPE_FLOAT(const  TYPE_FLOAT *array, size_t size);
size_t ARG_MIN_ARRAY_TYPE_DOUBLE(const  TYPE_DOUBLE *array, size_t size);
size_t ARG_MIN_ARRAY_TYPE_L_DOUBLE(const  TYPE_L_DOUBLE *array, size_t size);
size_t ARG_MIN_ARRAY_TYPE_STRING(const  TYPE_STRING *array, size_t size);


#endif /*__TOOLS_T_C_H__*/
