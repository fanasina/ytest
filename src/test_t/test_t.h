#ifndef __TEST_C_H__
#define __TEST_C_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
//#include <time.h>
#include <pthread.h>
//#include <unistd.h>

#include "src/tools_t/tools_t.h"


#define DEFAULT_K "\033[0m" //Resets the text to default color
#define GREEN_K "\033[0;32m" 
#define RED_K "\033[0;31m"

#ifdef HK
  #define HK_EQ "[==========]"
  #define HK_TR "[----------]"
  #define HK_RN "[RUN       ]"
  #define HK_DN "[      DONE]"
  #define HK_OK "[       OK ]"
  #define HK_FL "[  FAILED  ]"
  #define HK_PS "[  PASSED  ]"
  #define HK_SK "[  SKIP    ]"
#else
  #define HK_EQ "=========="
  #define HK_TR "----------"
  #define HK_RN "====== RUN"
  #define HK_DN "===== DONE"
  #define HK_OK "======= OK"
  #define HK_FL "===== FAIL"
  #define HK_PS "===== PASS"
  #define HK_SK "===== SKIP"
#endif

/*
// F_OUT file (stream) to log
#ifndef F_OUT
  #define F_OUT stdout
#endif
*/

#ifndef NOT_COLORED
  #define NOT_COLORED 0
#endif

#if 0
#ifndef PARALLEL
  #define PARALLEL 1
  #define LOCK(mut)
  #define UNLOCK(mut)
#else /*PARALLEL defined*/
  #define LOCK(mutex_var)  pthread_mutex_lock(&mutex_var);
  #define UNLOCK(mutex_var) pthread_mutex_unlock(&mutex_var);
  #define is_parallel 1
#endif
#endif

#define LOCK(mutex_var)  pthread_mutex_lock(&mutex_var);
#define UNLOCK(mutex_var) pthread_mutex_unlock(&mutex_var);


/*
 * print [ HK_NAME ] with color
 */
#define PRINT_HK_C(color,hk,format,...)\
  do{ if(!NOT_COLORED) fprintf(F_OUT, color hk DEFAULT_K format, __VA_ARGS__); \
      else fprintf(F_OUT, hk format, __VA_ARGS__); } while(0) \
      
/*
 * to skip the bloc test function
 */
#define SKIP(msg)\
  PRINT_HK_C(GREEN_K, HK_SK," %s\n",#msg);\
  PRINT_LOC("%s\n\n"," Skiped"); return;

struct func {
  char *name;
  void (*run)(void);
  struct func *next;
};

extern bool is_parallel;

void run_all_tests();
void execute_all(struct func *fun);
void append_func(void (*run)(void), char *name);
//__attribute__((destructor))  void purge_tests();
void run_some_tests(size_t cnt, ... );
void run_all_tests_exept(size_t cnt, ... );
void run_some_tests_ordered(size_t cnt, ... );

void run_all_tests_parallel(size_t parallel);



bool expected_true_f(bool val);
bool expected_false_f(bool val);

bool expected_true_f_name(bool val, const char *name);
bool expected_false_f_name(bool val, const char *name);

#define GEN_EXPECTED_EQ_TYPE_FUNC(type)\
  bool expected_eq_##type(type var1, type var2);\
  bool expected_eq_name_##type(type var1, type var2, const char *name); 

GEN_EXPECTED_EQ_TYPE_FUNC(TYPE_CHAR)
GEN_EXPECTED_EQ_TYPE_FUNC(TYPE_U_CHAR)
GEN_EXPECTED_EQ_TYPE_FUNC(TYPE_INT)
GEN_EXPECTED_EQ_TYPE_FUNC(TYPE_U_INT)
GEN_EXPECTED_EQ_TYPE_FUNC(TYPE_L_INT)
GEN_EXPECTED_EQ_TYPE_FUNC(TYPE_U_L_INT)
GEN_EXPECTED_EQ_TYPE_FUNC(TYPE_SIZE_T)
GEN_EXPECTED_EQ_TYPE_FUNC(TYPE_FLOAT)
GEN_EXPECTED_EQ_TYPE_FUNC(TYPE_DOUBLE)
GEN_EXPECTED_EQ_TYPE_FUNC(TYPE_L_DOUBLE)
GEN_EXPECTED_EQ_TYPE_FUNC(TYPE_STRING)

  /**
   * is_assert : 0 for EXPECT and 1 for ASSERT 
   */
#define HANDLE_EXPECT_ASSERT(type,var1,var2,is_assert)                                                          \
do{      \
   if(is_parallel==0){\
      if(expected_eq_##type(var1, var2)){                                                                       \
        PRINT_HK_C(GREEN_K,HK_TR," 1 test passed from %s \n\n",__func__);                                       \
      }                                                                                                         \
      else{                                                                                                     \
        PRINT_LOC("Failure\nExpected equality of these values:\n   %s\n\tWhich is:  %s\n %s\n\tWhich is: %s\n\n"\
          ,#var1, type##_TO_STR(var1),  #var2, type##_TO_STR(var2));                                            \
        PRINT_HK_C(RED_K,HK_TR," 1 test failed from %s \n",__func__);                                           \
        if(is_assert) return;                                                                                   \
      }                                                                                                         \
   }else {                                                                                                         \
      if(expected_eq_name_##type(var1, var2, __func__)){                                                                       \
        PRINT_HK_C(GREEN_K,HK_TR," 1 test passed from %s \n\n",__func__);                                       \
      }                                                                                                         \
      else{                                                                                                     \
        PRINT_LOC("Failure\nExpected equality of these values:\n   %s\n\tWhich is:  %s\n %s\n\tWhich is: %s\n\n"\
          ,#var1, type##_TO_STR(var1),  #var2, type##_TO_STR(var2));                                            \
        PRINT_HK_C(RED_K,HK_TR," 1 test failed from %s \n",__func__);                                           \
        if(is_assert) return;                                                                                   \
      }                                                                                                         \
    }\
}while(0);


// ============== EXPECT ===================

#define EXPECT_EQ_TYPE_CHAR(var1, var2) 	HANDLE_EXPECT_ASSERT(TYPE_CHAR,var1, var2, 0)
#define EXPECT_EQ_TYPE_U_CHAR(var1, var2) 	HANDLE_EXPECT_ASSERT(TYPE_U_CHAR,var1, var2, 0)
#define EXPECT_EQ_TYPE_INT(var1, var2) 	HANDLE_EXPECT_ASSERT(TYPE_INT,var1, var2, 0)
#define EXPECT_EQ_TYPE_U_INT(var1, var2) 	HANDLE_EXPECT_ASSERT(TYPE_U_INT,var1, var2, 0)
#define EXPECT_EQ_TYPE_L_INT(var1, var2) 	HANDLE_EXPECT_ASSERT(TYPE_L_INT,var1, var2, 0)
#define EXPECT_EQ_TYPE_U_L_INT(var1, var2) 	HANDLE_EXPECT_ASSERT(TYPE_U_L_INT,var1, var2, 0)
#define EXPECT_EQ_TYPE_SIZE_T(var1, var2) 	HANDLE_EXPECT_ASSERT(TYPE_SIZE_T,var1, var2, 0)
#define EXPECT_EQ_TYPE_FLOAT(var1, var2) 	HANDLE_EXPECT_ASSERT(TYPE_FLOAT,var1, var2, 0)
#define EXPECT_EQ_TYPE_DOUBLE(var1, var2) 	HANDLE_EXPECT_ASSERT(TYPE_DOUBLE,var1, var2, 0)
#define EXPECT_EQ_TYPE_L_DOUBLE(var1, var2) 	HANDLE_EXPECT_ASSERT(TYPE_L_DOUBLE,var1, var2, 0)
#define EXPECT_EQ_TYPE_STRING(var1, var2) 	HANDLE_EXPECT_ASSERT(TYPE_STRING,var1, var2, 0)

#define EXPECT_EQ(var1, var2) HANDLE_EXPECT_ASSERT(TYPE_L_INT,var1, var2, 0)



// ============== ASERT =====================

#define ASSERT_EQ_TYPE_CHAR(var1, var2) 	HANDLE_EXPECT_ASSERT(TYPE_CHAR,var1, var2, 1)
#define ASSERT_EQ_TYPE_U_CHAR(var1, var2) 	HANDLE_EXPECT_ASSERT(TYPE_U_CHAR,var1, var2, 1)
#define ASSERT_EQ_TYPE_INT(var1, var2) 	HANDLE_EXPECT_ASSERT(TYPE_INT,var1, var2, 1)
#define ASSERT_EQ_TYPE_U_INT(var1, var2) 	HANDLE_EXPECT_ASSERT(TYPE_U_INT,var1, var2, 1)
#define ASSERT_EQ_TYPE_L_INT(var1, var2) 	HANDLE_EXPECT_ASSERT(TYPE_L_INT,var1, var2, 1)
#define ASSERT_EQ_TYPE_U_L_INT(var1, var2) 	HANDLE_EXPECT_ASSERT(TYPE_U_L_INT,var1, var2, 1)
#define ASSERT_EQ_TYPE_SIZE_T(var1, var2) 	HANDLE_EXPECT_ASSERT(TYPE_SIZE_T,var1, var2, 1)
#define ASSERT_EQ_TYPE_FLOAT(var1, var2) 	HANDLE_EXPECT_ASSERT(TYPE_FLOAT,var1, var2, 1)
#define ASSERT_EQ_TYPE_DOUBLE(var1, var2) 	HANDLE_EXPECT_ASSERT(TYPE_DOUBLE,var1, var2, 1)
#define ASSERT_EQ_TYPE_L_DOUBLE(var1, var2) 	HANDLE_EXPECT_ASSERT(TYPE_L_DOUBLE,var1, var2, 1)
#define ASSERT_EQ_TYPE_STRING(var1, var2) 	HANDLE_EXPECT_ASSERT(TYPE_STRING,var1, var2, 1)

#define ASSERT_EQ(var1, var2)  HANDLE_EXPECT_ASSERT(TYPE_L_INT,var1, var2, 1)

/*
 * ==============  bool ===================
 *
 */

#define HANDLE_EXPECT_NOT_EXPECT_ASSERT(expect,not_expect,var1,is_assert)                             \
do{                                                                                               \
   if(is_parallel==0){\
      if(expected_##expect##_f(var1)){                                                                \
        PRINT_HK_C(GREEN_K,HK_TR," 1 test passed from %s \n\n",__func__);                             \
      }                                                                                               \
      else{                                                                                           \
        PRINT_LOC("Failure\nValue of: %s\nActual: %s\nExpected: %s\n\n", #var1, #not_expect, #expect);\
        PRINT_HK_C(RED_K,HK_TR," 1 test failed from %s \n",__func__);                                 \
        if(is_assert) return;                                                                         \
      }                                                                                               \
  }\
  else{\
      if(expected_##expect##_f_name(var1, __func__)){                                                                \
        PRINT_HK_C(GREEN_K,HK_TR," 1 test passed from %s \n\n",__func__);                             \
      }                                                                                               \
      else{                                                                                           \
        PRINT_LOC("Failure\nValue of: %s\nActual: %s\nExpected: %s\n\n", #var1, #not_expect, #expect);\
        PRINT_HK_C(RED_K,HK_TR," 1 test failed from %s \n",__func__);                                 \
        if(is_assert) return;                                                                         \
      }                                                                                               \
  }\
}while(0);


#define EXPECT_TRUE(var1) HANDLE_EXPECT_NOT_EXPECT_ASSERT(true, false, var1, 0)
#define EXPECT_FALSE(var1) HANDLE_EXPECT_NOT_EXPECT_ASSERT(false, true, var1, 0)

#define ASSERT_TRUE(var1) HANDLE_EXPECT_NOT_EXPECT_ASSERT(true, false, var1, 1)
#define ASSERT_FALSE(var1) HANDLE_EXPECT_NOT_EXPECT_ASSERT(false, true, var1, 1)


#define CONCAT(x,y) x ## y
#define STRFY(x) # x 

#define test_label test

#define FTEST_(count, name_f)                                                 \
  void CONCAT(test_##name_f##____,count)(void);                               \
  __attribute__((constructor))                                                \
  void CONCAT(append_test_##name_f,count)(void){                              \
    append_func(CONCAT(test_##name_f##____,count),STRFY(name_f test count));  \
  }                                                                           \
  void CONCAT(test_##name_f##____,count)(void)
 
#define FTEST__(count, name_f)                                                \
  void CONCAT(TEST_##name_f##____,count)(void);                                       \
  __attribute__((constructor))                                                \
  void CONCAT(append_test_##name_f,count)(void){                              \
    append_func(CONCAT(TEST_##name_f##____,count),STRFY(TEST(name_f): test N° count ));          \
  }                                                                           \
  void CONCAT(TEST_##name_f##____,count)(void)


//#define TEST(name_f)\
  FTEST_(__COUNTER__,name_f)

#define TEST(name_f)                                            \
  FTEST__(__COUNTER__,name_f)

/*  
#define ASSERT_TRUE(val)\
  if(expected_true_f(val,#val,__func__) == false) {error_print("%s\n\n","Failure"); return;}
  
#define ASSERT_FALSE(val)\
  if(expected_false_f(val,#val,__func__) == false) {error_print("%s\n\n","Failure"); return;}
*/



#endif /* __TEST_C_H__ */
