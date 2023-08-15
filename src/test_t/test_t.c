#include "src/test_t/test_t.h"


#define DEFAULT_K "\033[0m" //Resets the text to default color
#define GREEN_K "\033[0;32m" 
#define RED_K "\033[0;31m"


#define HK_EQ "[==========]"
#define HK_TR "[----------]"
#define HK_RN "[RUN       ]"
#define HK_DN "[      DONE]"
#define HK_OK "[       OK ]"
#define HK_FL "[  FAILED  ]"

#define INCR_PASS_CNT ++count_passed; ++count_passed_local;
#define INCR_FAIL_CNT ++count_failed; ++count_failed_local;

struct failed_lists{
  char *name;
  struct failed_lists *next;
} *failed_l = NULL;

void append_failed_list(const char *name_failed){
  static struct failed_lists *failed_static = NULL; 
  if(failed_static == NULL){
    failed_l = malloc(sizeof(struct failed_lists));
    failed_l->name = malloc(strlen(name_failed));
    strcpy(failed_l->name, name_failed);
    failed_l->next = NULL;
    failed_static = failed_l;
  }
  else{
    struct failed_lists *tmp = malloc(sizeof(struct failed_lists));
    tmp->name = malloc(strlen(name_failed));
    strcpy(tmp->name, name_failed);
    tmp->next = NULL;
    failed_static->next = tmp;
    failed_static = tmp;
  }
}


size_t count_tests = 0;

size_t count_passed = 0;
size_t count_passed_local = 0;

size_t count_failed = 0;
size_t count_failed_local = 0;


struct func f_beging; 


void vprintf_colored(char *color, char *format, ...) {
   va_list args;
   printf("%s",color);
   va_start(args, format);
   vprintf(format, args);
   va_end(args);
   printf(DEFAULT_K);
}

void printHK_color(char *color, char *HK, char * format, ...){
   printf("%s%s%s",color, HK, DEFAULT_K);
   va_list args;
   va_start(args, format);
   vprintf(format, args);
   va_end(args);
}

#define ASSERT_EXPECT_FROM(expect, not_expect)                                            \
bool assert_##expect##_from(bool val, const char * var_name, const char *func_name){      \
  if(val == expect) {                                                                     \
    INCR_PASS_CNT;                                                                        \
    printHK_color(GREEN_K,HK_TR," 1 test passed from %s \n\n",func_name);                 \
    return true;                                                                          \
  }else {                                                                                 \
    INCR_FAIL_CNT;                                                                        \
    printHK_color(RED_K,HK_TR," 1 test failed from %s \n",func_name);                     \
    append_failed_list(func_name);                                                        \
    printf("Value of: %s\nActual: %s\nExpected: %s\n\n",#not_expect, #expect, var_name);  \
    return false;                                                                         \
  }                                                                                       \
}     

ASSERT_EXPECT_FROM(true,false)
ASSERT_EXPECT_FROM(false,true)

#define ASSERT_EQ_TYPE(type)\
bool assert_eq_##type(type var1, type var2,                                                       \
    const char *var1_name, const char *var2_name, const char *func_name){                         \
  if(COMPARE_N_##type(&var1, &var2) == 0){                                                        \
    INCR_PASS_CNT;                                                                                \
    printHK_color(GREEN_K,HK_TR," 1 test passed from %s \n\n",func_name);                         \
    return true;                                                                                  \
  }else {                                                                                         \
    INCR_FAIL_CNT;                                                                                \
    printHK_color(RED_K,HK_TR," 1 test failed from %s \n",func_name);                             \
    append_failed_list(func_name);                                                                \
    printf("Expected equality of these values:\n   %s\n\tWhich is:  %s\n %s\n\tWhich is: %s\n\n"  \
        ,var1_name, type##_TO_STR(var1),  var2_name, type##_TO_STR(var2));                        \
    return false;                                                                                 \
  }                                                                                               \
}

ASSERT_EQ_TYPE(TYPE_CHAR)
ASSERT_EQ_TYPE(TYPE_U_CHAR)
ASSERT_EQ_TYPE(TYPE_INT)
ASSERT_EQ_TYPE(TYPE_U_INT)
ASSERT_EQ_TYPE(TYPE_L_INT)
ASSERT_EQ_TYPE(TYPE_U_L_INT)
ASSERT_EQ_TYPE(TYPE_SIZE_T)
ASSERT_EQ_TYPE(TYPE_FLOAT)
ASSERT_EQ_TYPE(TYPE_DOUBLE)
ASSERT_EQ_TYPE(TYPE_L_DOUBLE)
ASSERT_EQ_TYPE(TYPE_STRING)



void 
append_func(void (*run)(void), char *name){
  static struct func *f_static = NULL;
  if(f_static == NULL){
    f_static = &f_beging;
    f_static->name = malloc(strlen(name));
    strcpy(f_static->name,name);
    f_static->run = run;
    f_static->next = NULL;
  }
  else{
    struct func *tmp = malloc(sizeof(struct func));
    tmp->run = run;
    tmp->name = malloc(strlen(name));
    strcpy(tmp->name,name);
    tmp->next = NULL;
    f_static->next = tmp;
    f_static = tmp;
  }
  ++count_tests;
}

void begin_execute_func(char *fun_ame){
  vprintf_colored(GREEN_K,HK_RN); 
  printf(" %s\n", fun_ame); 
  count_passed_local = 0;
  count_failed_local = 0;
}
void end_execute_func(char *fun_ame){
  if(count_passed_local){
    printHK_color(GREEN_K,HK_DN," %lu tests passed from %s\n",count_passed_local,fun_ame);
  }
  if(count_failed_local){
    printHK_color(RED_K, HK_DN, " %lu tests failed from %s\n",count_failed_local,fun_ame);
  }
}

void head_run(size_t nbtest){
  printHK_color(GREEN_K, HK_EQ," Running %lu tests.\n",nbtest);
}

void list_failed_test(struct failed_lists *failed_lst){
  printHK_color(RED_K, HK_FL," %s\n",failed_lst->name);
  if(failed_lst->next) list_failed_test(failed_lst->next);
}


void
stat_end_run(){
  if(failed_l != NULL){
    printHK_color(RED_K, HK_FL," %lu tests, listed below:\n",count_failed); 
    list_failed_test(failed_l);
  }
}

bool is_in_array(size_t *array, size_t sz, size_t num){
  bool found = false;
  for(size_t i = 0; i < sz; ++i){
    if(array[i] == num){
      found = true;
      break;
    }
  }
  return found;
}

void execute_all(struct func *fun){
  struct func *tmp = fun;
  //printHK_color(GREEN_K, HK_EQ," Running %lu tests.\n",count_tests);
  while(tmp){
    begin_execute_func(tmp->name);
    tmp->run();
    end_execute_func(tmp->name);
    tmp = tmp->next;
  }
}

void execute_one_test(struct func *fun, size_t num){
  size_t cur = 0;
  struct func *tmp = fun;
  while(tmp){
    if(cur++ == num){
      begin_execute_func(fun->name);
      fun->run();
      end_execute_func(fun->name);
    }
    tmp = tmp->next;
  }
}


void execute_some_tests_ordered(struct func *fun, size_t cnt, size_t *array )
{   
  struct func *tmp = fun;
  size_t cur = 0, index = 0;
  
  while(tmp){
    if((cur < cnt) && (index++ == array[cur])){
      begin_execute_func(tmp->name);
      tmp->run();
      end_execute_func(tmp->name);
      ++cur;
    }   
    tmp = tmp->next;
  }
}

void
run_some_tests(size_t cnt, ...)
{
   head_run(cnt);
   va_list args;
   va_start(args, cnt);
   for(size_t i=0; i < cnt; ++i){
    execute_one_test(&f_beging, va_arg(args, size_t));
   }
   va_end(args);
   stat_end_run();
}

void 
run_some_tests_ordered(size_t cnt, ... )
{
   head_run(cnt);
   va_list args;
   va_start(args, cnt);
   size_t *array=malloc(cnt*sizeof(size_t));
   for(size_t i=0; i < cnt; ++i){
    array[i] = va_arg(args, size_t);
   }

   execute_some_tests_ordered(&f_beging, cnt, array);
   va_end(args);
   stat_end_run();
}




void execute_all_tests_exept(struct func *fun, size_t cnt, size_t *array )
{  
  struct func *tmp = fun;
  size_t cur = 0;
  while(tmp){
    if(!is_in_array(array, cnt, cur++)){
      begin_execute_func(tmp->name);
      tmp->run();
      end_execute_func(tmp->name);
    }
    tmp = tmp->next;
  }
}

void 
run_all_tests_exept(size_t cnt, ... )
{
   if(count_tests >= cnt)
      head_run(count_tests - cnt);
   va_list args;
   va_start(args, cnt);
   size_t *array=malloc(cnt*sizeof(size_t));
   for(size_t i=0; i < cnt; ++i){
      array[i] = va_arg(args, size_t);
   }

   execute_all_tests_exept(&f_beging, cnt, array);
   va_end(args);
   stat_end_run();
}


void
run_all_tests()
{
   head_run(count_tests);
   execute_all(&f_beging);
   stat_end_run();
}

void 
clear_all_func(struct func **fun)
{
  if(*fun != NULL)
    {
      clear_all_func(&((*fun)->next));
      free(*fun);
      *fun = NULL;
    }
}

