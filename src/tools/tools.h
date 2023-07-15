#ifndef __TOOLS_C_H__
#define __TOOLS_C_H__

#include <stdlib.h>
#include <string.h>

#define FREE(x)  { free((x)); (x) = NULL;} 

int compare_int(void* a, void* b);
int compare_unsigned_int(void* a, void* b);
int compare_float(void *a, void *b);
int compare_double(void *a, void *b);
int compare_string(void *a, void *b);

int max_array_int(int *arr, size_t sz);
unsigned int max_array_unsigned_int(unsigned int *arr, size_t sz);
int min_array_int(int *arr, size_t sz);
unsigned int min_array_unsigned_int(unsigned int *arr, size_t sz);

size_t arg_max_array_int(int *arr, size_t sz);
size_t arg_max_array_unsigned_int(unsigned int *arr, size_t sz);
size_t arg_min_array_int(int *arr, size_t sz);
size_t arg_min_array_unsigned_int(unsigned int *arr, size_t sz);

void copy_array_unsigned_int(unsigned int *dst, const unsigned int *src, size_t size);

/*
bool is_less_eq_than_i(int a, int b); // { return a <= b; }
bool is_less_than_i(int a, int b); // { return a < b; }
bool is_great_eq_than_i(int a, int b); //  { return a >= b; }
bool is_great_than_i(int a, int b); //  { return a > b; }
*/
int incr_i(int i); //  { return i + 1; }
int decr_i(int i); //  { return i - 1; }


/*
bool is_less_eq_than_u(unsigned int a, unsigned int b); // { return a <= b; }
bool is_less_than_u(unsigned int a, unsigned int b); // { return a < b; }
bool is_great_eq_than_u(unsigned int a, unsigned int b); //  { return a >= b; }
bool is_great_than_u(unsigned int a, unsigned int b); //  { return a > b; }
*/
unsigned int incr_u(unsigned int i); //  { return i + 1; }
unsigned int decr_u(unsigned int i); //  { return i - 1; }



#endif /*__TOOLS_C_H__*/
