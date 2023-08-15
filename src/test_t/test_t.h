#ifndef __TEST_C_H__
#define __TEST_C_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
//#include <time.h>

#include "src/tools_t/tools_t.h"


struct func {
  char *name;
  void (*run)(void);
  struct func *next;
};

//void begin_f();
//struct func f_beging; 

void vprintf_colored(char *color, char * format, ...);

void run_all_tests();
void execute_all(struct func *fun);
void append_func(void (*run)(void), char *name);
void clear_all_func();
void run_some_tests(size_t cnt, ... );
void run_all_tests_exept(size_t cnt, ... );
void run_some_tests_ordered(size_t cnt, ... );




bool expected_true_f(bool val, const char* var_name, const char * func_name);
bool expected_false_f(bool val, const char* var_name, const char * func_name);


#define GEN_EXPECTED_EQ_TYPE_FUNC(type)\
  bool expected_eq_##type(type var1, type var2,                                                   \
    const char *var1_name, const char *var2_name, const char *func_name);                       \

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

#define EXPECT_EQ_TYPE_CHAR(var1, var2) expected_eq_TYPE_CHAR(var1, var2, #var1, #var2, __func__) ;
#define EXPECT_EQ_TYPE_U_CHAR(var1, var2) expected_eq_TYPE_U_CHAR(var1, var2, #var1, #var2, __func__) ;
#define EXPECT_EQ_TYPE_INT(var1, var2) expected_eq_TYPE_INT(var1, var2, #var1, #var2, __func__) ;
#define EXPECT_EQ_TYPE_U_INT(var1, var2) expected_eq_TYPE_U_INT(var1, var2, #var1, #var2, __func__) ;
#define EXPECT_EQ_TYPE_L_INT(var1, var2) expected_eq_TYPE_L_INT(var1, var2, #var1, #var2, __func__) ;
#define EXPECT_EQ_TYPE_U_L_INT(var1, var2) expected_eq_TYPE_U_L_INT(var1, var2, #var1, #var2, __func__) ;
#define EXPECT_EQ_TYPE_SIZE_T(var1, var2) expected_eq_TYPE_SIZE_T(var1, var2, #var1, #var2, __func__) ;
#define EXPECT_EQ_TYPE_FLOAT(var1, var2) expected_eq_TYPE_FLOAT(var1, var2, #var1, #var2, __func__) ;
#define EXPECT_EQ_TYPE_DOUBLE(var1, var2) expected_eq_TYPE_DOUBLE(var1, var2, #var1, #var2, __func__) ;
#define EXPECT_EQ_TYPE_L_DOUBLE(var1, var2) expected_eq_TYPE_L_DOUBLE(var1, var2, #var1, #var2, __func__) ;
#define EXPECT_EQ_TYPE_STRING(var1, var2) expected_eq_TYPE_STRING(var1, var2, #var1, #var2, __func__) ;

#define EXPECT_EQ(var1, var2) expected_eq_TYPE_L_INT(var1, var2, #var1, #var2, __func__) ;



#define ASSERT_EQ_TYPE_CHAR(var1, var2) do{ if(expected_eq_TYPE_CHAR(var1, var2, #var1, #var2, __func__) == false) return ;} while(0)
#define ASSERT_EQ_TYPE_U_CHAR(var1, var2) do{ if(expected_eq_TYPE_U_CHAR(var1, var2, #var1, #var2, __func__) == false) return ;} while(0)
#define ASSERT_EQ_TYPE_INT(var1, var2) do{ if(expected_eq_TYPE_INT(var1, var2, #var1, #var2, __func__) == false) return ;} while(0)
#define ASSERT_EQ_TYPE_U_INT(var1, var2) do{ if(expected_eq_TYPE_U_INT(var1, var2, #var1, #var2, __func__) == false) return ;} while(0)
#define ASSERT_EQ_TYPE_L_INT(var1, var2) do{ if(expected_eq_TYPE_L_INT(var1, var2, #var1, #var2, __func__) == false) return ;} while(0)
#define ASSERT_EQ_TYPE_U_L_INT(var1, var2) do{ if(expected_eq_TYPE_U_L_INT(var1, var2, #var1, #var2, __func__) == false) return ;} while(0)
#define ASSERT_EQ_TYPE_SIZE_T(var1, var2) do{ if(expected_eq_TYPE_SIZE_T(var1, var2, #var1, #var2, __func__) == false) return ;} while(0)
#define ASSERT_EQ_TYPE_FLOAT(var1, var2) do{ if(expected_eq_TYPE_FLOAT(var1, var2, #var1, #var2, __func__) == false) return ;} while(0)
#define ASSERT_EQ_TYPE_DOUBLE(var1, var2) do{ if(expected_eq_TYPE_DOUBLE(var1, var2, #var1, #var2, __func__) == false) return ;} while(0)
#define ASSERT_EQ_TYPE_L_DOUBLE(var1, var2) do{ if(expected_eq_TYPE_L_DOUBLE(var1, var2, #var1, #var2, __func__) == false) return ;} while(0)
#define ASSERT_EQ_TYPE_STRING(var1, var2) do{ if(expected_eq_TYPE_STRING(var1, var2, #var1, #var2, __func__) == false) return ;} while(0)

#define ASSERT_EQ(var1, var2) do{ if(expected_eq_TYPE_L_INT(var1, var2, #var1, #var2, __func__) == false) return ;} while(0)


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
  void CONCAT(name_f##___,count)(void);                                       \
  __attribute__((constructor))                                                \
  void CONCAT(append_test_##name_f,count)(void){                              \
    append_func(CONCAT(name_f##___,count),STRFY(name_f test count));          \
  }                                                                           \
  void CONCAT(name_f##___,count)(void)


#define TEST(name_f)\
  FTEST_(__COUNTER__,name_f)

//#define TEST(name_f)                                            \
  FTEST__(__COUNTER__,name_f)

#define ASSERT_TRUE(val)\
  if(expected_true_f(val,#val,__func__) == false) return;
  
#define ASSERT_FALSE(val)\
  if(expected_false_f(val,#val,__func__) == false) return;




#endif /* __TEST_C_H__ */
