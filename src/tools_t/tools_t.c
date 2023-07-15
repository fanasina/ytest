#include "src/tools_t/tools_t.h"

#define COMPARE_N(type,a,b)\
  int COMPARE_N_##type(const void *a, const  void *b){ \
    if (*(type*)a == *(type*)b) return 0; \
    if (*(type*)a > *(type*)b) return 1; \
    return -1; }

COMPARE_N(TYPE_CHAR,a,b)
COMPARE_N(TYPE_U_CHAR,a,b)
COMPARE_N(TYPE_INT,a,b)
COMPARE_N(TYPE_U_INT,a,b)
COMPARE_N(TYPE_L_INT,a,b)
COMPARE_N(TYPE_U_L_INT,a,b)
COMPARE_N(TYPE_FLOAT,a,b)
COMPARE_N(TYPE_DOUBLE,a,b)
COMPARE_N(TYPE_L_DOUBLE,a,b)
int 
COMPARE_N_TYPE_STRING(const void *a,const  void* b)
{
  return strcmp(( char*)a,( char*)b);
}

#define COPY_ARRAY(type, dst, src, size)\
  void COPY_ARRAY_##type(type *dst, const type *src, size_t size){\
    for(size_t i = 0; i < size; ++i) dst[i]=src[i]; }

COPY_ARRAY(TYPE_CHAR,dst,src,size);
COPY_ARRAY(TYPE_U_CHAR,dst,src,size);
COPY_ARRAY(TYPE_INT,dst,src,size);
COPY_ARRAY(TYPE_U_INT,dst,src,size);
COPY_ARRAY(TYPE_L_INT,dst,src,size);
COPY_ARRAY(TYPE_U_L_INT,dst,src,size);
COPY_ARRAY(TYPE_FLOAT,dst,src,size);
COPY_ARRAY(TYPE_DOUBLE,dst,src,size);
COPY_ARRAY(TYPE_L_DOUBLE,dst,src,size);
void COPY_ARRAY_TYPE_STRING(char** dst, const char** src, size_t size)
{
  for(size_t i = 0; i < size; ++i) strcpy(dst[i],src[i]);
}

#define MAX_ARRAY(type, array, size, compare)\
  type MAX_ARRAY_##type(const type *array, size_t size){\
    if(array == NULL) return 0;\
    type mx =(type)array[0];\
    for(size_t i = 0; i < size; ++i)\
      if(compare(&mx,&array[i]) < 0) mx =(type)array[i];\
    return mx;}
MAX_ARRAY(TYPE_CHAR,array,size,COMPARE_N_TYPE_CHAR);
MAX_ARRAY(TYPE_U_CHAR,array,size,COMPARE_N_TYPE_U_CHAR);
MAX_ARRAY(TYPE_INT,array,size,COMPARE_N_TYPE_INT);
MAX_ARRAY(TYPE_U_INT,array,size,COMPARE_N_TYPE_U_INT);
MAX_ARRAY(TYPE_L_INT,array,size,COMPARE_N_TYPE_L_INT);
MAX_ARRAY(TYPE_U_L_INT,array,size,COMPARE_N_TYPE_U_L_INT);
MAX_ARRAY(TYPE_FLOAT,array,size,COMPARE_N_TYPE_FLOAT);
MAX_ARRAY(TYPE_DOUBLE,array,size,COMPARE_N_TYPE_DOUBLE);
MAX_ARRAY(TYPE_L_DOUBLE,array,size,COMPARE_N_TYPE_L_DOUBLE);
MAX_ARRAY(TYPE_STRING,array,size,COMPARE_N_TYPE_STRING);

#define ARG_MAX_ARRAY(type, array, size, compare)\
  size_t ARG_MAX_ARRAY_##type(const type *array, size_t size){\
    if(array == NULL) return 0;\
    size_t i_mx = 0;\
    for(size_t i = 0; i < size; ++i)\
      if(compare(&array[i_mx],&array[i]) < 0) i_mx = i;\
    return i_mx;}
ARG_MAX_ARRAY(TYPE_CHAR,array,size,COMPARE_N_TYPE_CHAR);
ARG_MAX_ARRAY(TYPE_U_CHAR,array,size,COMPARE_N_TYPE_U_CHAR);
ARG_MAX_ARRAY(TYPE_INT,array,size,COMPARE_N_TYPE_INT);
ARG_MAX_ARRAY(TYPE_U_INT,array,size,COMPARE_N_TYPE_U_INT);
ARG_MAX_ARRAY(TYPE_L_INT,array,size,COMPARE_N_TYPE_L_INT);
ARG_MAX_ARRAY(TYPE_U_L_INT,array,size,COMPARE_N_TYPE_U_L_INT);
ARG_MAX_ARRAY(TYPE_FLOAT,array,size,COMPARE_N_TYPE_FLOAT);
ARG_MAX_ARRAY(TYPE_DOUBLE,array,size,COMPARE_N_TYPE_DOUBLE);
ARG_MAX_ARRAY(TYPE_L_DOUBLE,array,size,COMPARE_N_TYPE_L_DOUBLE);
ARG_MAX_ARRAY(TYPE_STRING,array,size,COMPARE_N_TYPE_STRING);

#define MIN_ARRAY(type, array, size, compare)\
  type MIN_ARRAY_##type(const type *array, size_t size){\
    if(array == NULL) return 0;\
    type mn =(type)array[0];\
    for(size_t i = 0; i < size; ++i)\
      if(compare(&mn,&array[i]) > 0) mn =(type)array[i];\
    return mn;}
MIN_ARRAY(TYPE_CHAR,array,size,COMPARE_N_TYPE_CHAR);
MIN_ARRAY(TYPE_U_CHAR,array,size,COMPARE_N_TYPE_U_CHAR);
MIN_ARRAY(TYPE_INT,array,size,COMPARE_N_TYPE_INT);
MIN_ARRAY(TYPE_U_INT,array,size,COMPARE_N_TYPE_U_INT);
MIN_ARRAY(TYPE_L_INT,array,size,COMPARE_N_TYPE_L_INT);
MIN_ARRAY(TYPE_U_L_INT,array,size,COMPARE_N_TYPE_U_L_INT);
MIN_ARRAY(TYPE_FLOAT,array,size,COMPARE_N_TYPE_FLOAT);
MIN_ARRAY(TYPE_DOUBLE,array,size,COMPARE_N_TYPE_DOUBLE);
MIN_ARRAY(TYPE_L_DOUBLE,array,size,COMPARE_N_TYPE_L_DOUBLE);
MIN_ARRAY(TYPE_STRING,array,size,COMPARE_N_TYPE_STRING);

#define ARG_MIN_ARRAY(type, array, size, compare)\
  size_t ARG_MIN_ARRAY_##type(const type *array, size_t size){\
    if(array == NULL) return 0;\
    size_t i_mn = 0;\
    for(size_t i = 0; i < size; ++i)\
      if(compare(&array[i_mn],&array[i]) > 0) i_mn = i;\
    return i_mn;}
ARG_MIN_ARRAY(TYPE_CHAR,array,size,COMPARE_N_TYPE_CHAR);
ARG_MIN_ARRAY(TYPE_U_CHAR,array,size,COMPARE_N_TYPE_U_CHAR);
ARG_MIN_ARRAY(TYPE_INT,array,size,COMPARE_N_TYPE_INT);
ARG_MIN_ARRAY(TYPE_U_INT,array,size,COMPARE_N_TYPE_U_INT);
ARG_MIN_ARRAY(TYPE_L_INT,array,size,COMPARE_N_TYPE_L_INT);
ARG_MIN_ARRAY(TYPE_U_L_INT,array,size,COMPARE_N_TYPE_U_L_INT);
ARG_MIN_ARRAY(TYPE_FLOAT,array,size,COMPARE_N_TYPE_FLOAT);
ARG_MIN_ARRAY(TYPE_DOUBLE,array,size,COMPARE_N_TYPE_DOUBLE);
ARG_MIN_ARRAY(TYPE_L_DOUBLE,array,size,COMPARE_N_TYPE_L_DOUBLE);
ARG_MIN_ARRAY(TYPE_STRING,array,size,COMPARE_N_TYPE_STRING);


int main()
{
  unsigned int ui1 = 2545466;
  unsigned int ui2 = 2544566;
  printf(" %u >? %u = %d \n",ui1,ui2,COMPARE_N_TYPE_U_INT(&ui1,&ui2));
  printf(" %u >? %u = %d \n",ui1,ui1,COMPARE_N_TYPE_U_INT(&ui1,&ui1));
  printf(" %u >? %u = %d \n",ui2,ui1,COMPARE_N_TYPE_U_INT(&ui2,&ui1));
  int i1 = 2545466;
  int i2 = 2544566;
  printf(" %d >? %d = %d \n",i1,i2,COMPARE_N_TYPE_U_INT(&i1,&i2));
  printf(" %d >? %d = %d \n",i1,i1,COMPARE_N_TYPE_U_INT(&i1,&i1));
  printf(" %d >? %d = %d \n",i2,i1,COMPARE_N_TYPE_U_INT(&i2,&i1));
  
  int tabi[]={5,2,6,4,3,1};
  int tabr[6]={0};
  COPY_ARRAY_TYPE_INT(tabr,tabi,6);

  for(size_t i=0; i<6; ++i) printf(" %d \n", tabr[i]);

  printf("MIN = %d \n",MIN_ARRAY_TYPE_INT(tabr,6));

  return 0;
}
