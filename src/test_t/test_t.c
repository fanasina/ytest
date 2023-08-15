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
#define HK_PS "[  PASSED  ]"


#ifndef SECOND
  #define SECOND 0
#endif
#ifndef NANOSECOND
  #define NANOSECOND 0
#endif



//#define INCR_PASS_CNT ++count_passed_local;
//#define INCR_FAIL_CNT ++count_failed_local;

/*
#define INCR_PASS_CNT ++count_passed; ++count_passed_local;
#define INCR_FAIL_CNT ++count_failed; ++count_failed_local;
*/
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


double diff_timespec_seconds(struct timespec time_stop, struct timespec time_start){
  return (time_stop.tv_sec - time_start.tv_sec) + 1.0e-9 * (time_stop.tv_nsec - time_start.tv_nsec);
}

double diff_timespec_milliseconds(struct timespec time_stop, struct timespec time_start){
  return 1.0e3 * (time_stop.tv_sec - time_start.tv_sec) + 1.0e-3 * (time_stop.tv_nsec - time_start.tv_nsec);
}
long diff_timespec_nanoseconds(struct timespec time_stop, struct timespec time_start){
  return 1.0e9 * (time_stop.tv_sec - time_start.tv_sec) + (time_stop.tv_nsec - time_start.tv_nsec);
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

#define EXPECTED_EXPECT_FROM(expect, not_expect)                                            \
bool expected_##expect##_f(bool val, const char * var_name, const char *func_name){      \
  if(val == expect) {                                                                     \
    ++count_passed_local; /*INCR_PASS_CNT;*/                                                                        \
    printHK_color(GREEN_K,HK_TR," 1 test passed from %s \n\n",func_name);                 \
    return true;                                                                          \
  }else {                                                                                 \
    ++count_failed_local; /*INCR_FAIL_CNT;*/                                                                        \
    printHK_color(RED_K,HK_TR," 1 test failed from %s \n",func_name);                     \
    /*append_failed_list(func_name);*/                                                        \
    printf("Value of: %s\nActual: %s\nExpected: %s\n\n", var_name, #not_expect, #expect);  \
    return false;                                                                         \
  }                                                                                       \
}     

EXPECTED_EXPECT_FROM(true,false)
EXPECTED_EXPECT_FROM(false,true)

#define EXPECTED_EQ_TYPE(type)\
bool expected_eq_##type(type var1, type var2,                                                       \
    const char *var1_name, const char *var2_name, const char *func_name){                         \
  if(COMPARE_N_##type(&var1, &var2) == 0){                                                        \
    ++count_passed_local; /*INCR_PASS_CNT;*/                                                                        \
    printHK_color(GREEN_K,HK_TR," 1 test passed from %s \n\n",func_name);                         \
    return true;                                                                                  \
  }else {                                                                                         \
    ++count_failed_local; /*INCR_FAIL_CNT;*/                                                                        \
    /*append_failed_list(func_name);*/                                                              \
    printHK_color(RED_K,HK_TR," 1 test failed from %s \n",func_name);                             \
    printf("Expected equality of these values:\n   %s\n\tWhich is:  %s\n %s\n\tWhich is: %s\n\n"  \
        ,var1_name, type##_TO_STR(var1),  var2_name, type##_TO_STR(var2));                        \
    return false;                                                                                 \
  }                                                                                               \
}


EXPECTED_EQ_TYPE(TYPE_CHAR)
EXPECTED_EQ_TYPE(TYPE_U_CHAR)
EXPECTED_EQ_TYPE(TYPE_INT)
EXPECTED_EQ_TYPE(TYPE_U_INT)
EXPECTED_EQ_TYPE(TYPE_L_INT)
EXPECTED_EQ_TYPE(TYPE_U_L_INT)
EXPECTED_EQ_TYPE(TYPE_SIZE_T)
EXPECTED_EQ_TYPE(TYPE_FLOAT)
EXPECTED_EQ_TYPE(TYPE_DOUBLE)
EXPECTED_EQ_TYPE(TYPE_L_DOUBLE)
EXPECTED_EQ_TYPE(TYPE_STRING)



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

void begin_execute_func(char *fun_ame, struct timespec *start_t){
  clock_gettime(CLOCK_REALTIME, start_t);
  vprintf_colored(GREEN_K,HK_RN); 
  printf(" %s\n", fun_ame); 
  count_passed_local = 0;
  count_failed_local = 0;
}

#define PRINT_TIMESTAMP_STAT(color)\
    if(SECOND) printHK_color(color,HK_DN," %lu tests passed from %s (%lf s)\n\n",count_passed_local,fun_ame, diff_timespec_seconds(end_t, start_t));\
    else if(NANOSECOND) printHK_color(color,HK_DN," %lu tests passed from %s (%ld ns)\n\n",count_passed_local,fun_ame, diff_timespec_nanoseconds(end_t, start_t));\
    else printHK_color(color,HK_DN," %lu tests passed from %s (%lf ms)\n\n",count_passed_local,fun_ame, diff_timespec_milliseconds(end_t, start_t));

void end_execute_func(char *fun_ame, struct timespec start_t){
  struct timespec end_t; clock_gettime(CLOCK_REALTIME, &end_t);
  if(count_failed_local){
    ++count_failed;
    append_failed_list(fun_ame); 
    printHK_color(RED_K, HK_FL, " %lu tests failed from %s\n",count_failed_local,fun_ame);
    PRINT_TIMESTAMP_STAT(RED_K);
  }
  else 
  {
    ++count_passed;
    PRINT_TIMESTAMP_STAT(GREEN_K);
  }
}

void head_run(size_t nbtest, struct timespec *start_t){
  clock_gettime(CLOCK_REALTIME, start_t);
  printHK_color(GREEN_K, HK_EQ," Running %lu tests.\n",nbtest);
}

void list_failed_test(struct failed_lists *failed_lst){
  printHK_color(RED_K, HK_FL," %s\n",failed_lst->name);
  if(failed_lst->next) list_failed_test(failed_lst->next);
}


void
stat_end_run(size_t ntst, struct timespec start_t){
  struct timespec end_t; clock_gettime(CLOCK_REALTIME, &end_t);
  
  if(SECOND) printHK_color(GREEN_K, HK_EQ," %lu tests ran. (%lf s total)\n",ntst, diff_timespec_seconds(end_t, start_t));
  else if(NANOSECOND) printHK_color(GREEN_K, HK_EQ," %lu tests ran. (%ld ns total)\n",ntst, diff_timespec_nanoseconds(end_t, start_t));
  else printHK_color(GREEN_K, HK_EQ," %lu tests ran. (%lf ms total)\n",ntst, diff_timespec_milliseconds(end_t, start_t));
  
  printHK_color(GREEN_K, HK_PS," %lu tests\n", count_passed);
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
  struct timespec start_t;
  //printHK_color(GREEN_K, HK_EQ," Running %lu tests.\n",count_tests);
  while(tmp){
    begin_execute_func(tmp->name, &start_t);
    tmp->run();
    end_execute_func(tmp->name, start_t);
    tmp = tmp->next;
  }
}

void execute_one_test(struct func *fun, size_t num){
  size_t cur = 0;
  struct timespec start_t;
  struct func *tmp = fun;
  while(tmp){
    if(cur++ == num){
      begin_execute_func(fun->name, &start_t);
      fun->run();
      end_execute_func(fun->name, start_t);
    }
    tmp = tmp->next;
  }
}


void execute_some_tests_ordered(struct func *fun, size_t cnt, size_t *array )
{   
  struct timespec start_t;
  struct func *tmp = fun;
  size_t cur = 0, index = 0;
  
  while(tmp){
    if((cur < cnt) && (index++ == array[cur])){
      begin_execute_func(tmp->name, &start_t);
      tmp->run();
      end_execute_func(tmp->name, start_t);
      ++cur;
    }   
    tmp = tmp->next;
  }
}

void
run_some_tests(size_t cnt, ...)
{
   struct timespec start_t;
   head_run(cnt, &start_t);
   va_list args;
   va_start(args, cnt);
   for(size_t i=0; i < cnt; ++i){
    execute_one_test(&f_beging, va_arg(args, size_t));
   }
   va_end(args);
   stat_end_run(cnt, start_t);
}

void 
run_some_tests_ordered(size_t cnt, ... )
{
   struct timespec start_t;
   head_run(cnt, &start_t);
   va_list args;
   va_start(args, cnt);
   size_t *array=malloc(cnt*sizeof(size_t));
   for(size_t i=0; i < cnt; ++i){
    array[i] = va_arg(args, size_t);
   }

   execute_some_tests_ordered(&f_beging, cnt, array);
   va_end(args);
   stat_end_run(cnt, start_t);
}




void execute_all_tests_exept(struct func *fun, size_t cnt, size_t *array )
{ 
  struct timespec start_t; 
  struct func *tmp = fun;
  size_t cur = 0;
  while(tmp){
    if(!is_in_array(array, cnt, cur++)){
      begin_execute_func(tmp->name, &start_t);
      tmp->run();
      end_execute_func(tmp->name, start_t);
    }
    tmp = tmp->next;
  }
}

void 
run_all_tests_exept(size_t cnt, ... )
{
   struct timespec start_t;
   if(count_tests >= cnt)
      head_run(count_tests - cnt, &start_t);
   va_list args;
   va_start(args, cnt);
   size_t *array=malloc(cnt*sizeof(size_t));
   for(size_t i=0; i < cnt; ++i){
      array[i] = va_arg(args, size_t);
   }

   execute_all_tests_exept(&f_beging, cnt, array);
   va_end(args);
   if(count_tests >= cnt)
     stat_end_run(count_tests - cnt, start_t);
}


void
run_all_tests()
{
   struct timespec start_t;
   head_run(count_tests, &start_t);
   execute_all(&f_beging);
   stat_end_run(count_tests, start_t);
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

