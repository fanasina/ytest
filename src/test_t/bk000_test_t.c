#include "src/test_t/test_t.h"

/*
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

*/

#ifndef SECOND
  #define SECOND 0
#endif
#ifndef NANOSECOND
  #define NANOSECOND 0
#endif



#ifndef PARALLEL
  #define PARALLEL 1
  #define LOCK(mut) 
  #define UNLOCK(mut)
#else /*PARALLEL defined*/
  #define LOCK(mutex_var)  pthread_mutex_lock(&mutex_var);
  #define UNLOCK(mutex_var) pthread_mutex_unlock(&mutex_var);
#endif



#define INCREMENT(variable)\
  LOCK(mut_##variable); \
  ++variable;\
  UNLOCK(mut_##variable);


pthread_mutex_t mut_count_pass_global;
pthread_mutex_t mut_count_pass_local;
pthread_mutex_t mut_count_fail_global;
pthread_mutex_t mut_count_fail_local;



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

size_t count_pass_global = 0;
size_t count_pass_local = 0;

size_t count_fail_global = 0;
size_t count_fail_local = 0;


struct func *f_beging=NULL; 

/*
 * Tname format name is TEST_name_f____NUM, we extract NUM here
 * to have hash_table of the count when parallel test!
 */
size_t extract_num_test__f(char *name_f){
  size_t len = strlen(name_f);
  size_t val = 0, p = 1;
  for(size_t i= len-1; i>=0; --i){
    if(name_f[i]=='_') break;
    else if(name_f[i] >= '0' && name_f[i] <= '9'){
      val += p * (name_f[i]-'0');
    }
  }
  return val;
}


#define EXPECTED_EXPECT_F(expect, not_expect)                                          \
bool expected_##expect##_f(bool val){                                                     \
  if(val == expect) {                                                                     \
    INCREMENT(count_pass_local); /*++count_pass_local*/                                                                 \
    return true;                                                                          \
  }else {                                                                                 \
    INCREMENT(count_fail_local); /*++count_fail_local*/                                                                 \
    return false;                                                                         \
  }                                                                                       \
}     

EXPECTED_EXPECT_F(true,false)
EXPECTED_EXPECT_F(false,true)

#define EXPECTED_EQ_TYPE(type)                                                            \
bool expected_eq_##type(type var1, type var2){                                            \
  if(COMPARE_N_##type(&var1, &var2) == 0){                                                \
    INCREMENT(count_pass_local); /*++count_pass_local*/                                                                 \
    return true;                                                                          \
  }else {                                                                                 \
    INCREMENT(count_fail_local); /*++count_fail_local*/                                                                 \
    return false;                                                                         \
  }                                                                                       \
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
  if(f_beging == NULL){
    f_beging = malloc(sizeof(struct func));
    f_static = f_beging;
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
  PRINT_HK_C(GREEN_K,HK_RN," %s\n", fun_ame); 
  count_pass_local = 0;
  count_fail_local = 0;
}

#define PRINT_TIMESTAMP_STAT(color)\
    if(SECOND) PRINT_HK_C(color,HK_DN," %lu tests passed from %s (%lf s)\n\n",count_pass_local,fun_ame, diff_timespec_seconds(end_t, start_t));\
    else if(NANOSECOND) PRINT_HK_C(color,HK_DN," %lu tests passed from %s (%ld ns)\n\n",count_pass_local,fun_ame, diff_timespec_nanoseconds(end_t, start_t));\
    else PRINT_HK_C(color,HK_DN," %lu tests passed from %s (%lf ms)\n\n",count_pass_local,fun_ame, diff_timespec_milliseconds(end_t, start_t));

void end_execute_func(char *fun_ame, struct timespec start_t){
  struct timespec end_t; clock_gettime(CLOCK_REALTIME, &end_t);
  if(count_fail_local){
    INCREMENT(count_fail_global); /*++count_fail_global*/
    append_failed_list(fun_ame); 
    PRINT_HK_C(RED_K, HK_FL, " %lu tests failed from %s\n",count_fail_local,fun_ame);
    PRINT_TIMESTAMP_STAT(RED_K);
  }
  else 
  {
    INCREMENT(count_pass_global); /*++count_pass_global*/
    PRINT_TIMESTAMP_STAT(GREEN_K);
  }
}

void head_run(size_t nbtest, struct timespec *start_t){
  clock_gettime(CLOCK_REALTIME, start_t);
  PRINT_HK_C(GREEN_K, HK_EQ," Running %lu tests.\n",nbtest);
}

void list_failed_test(struct failed_lists *failed_lst){
  PRINT_HK_C(RED_K, HK_FL," %s\n",failed_lst->name);
  if(failed_lst->next) list_failed_test(failed_lst->next);
}


void
stat_end_run(size_t ntst, struct timespec start_t){
  struct timespec end_t; clock_gettime(CLOCK_REALTIME, &end_t);
  
  if(SECOND) PRINT_HK_C(GREEN_K, HK_EQ," %lu tests ran. (%lf s total)\n",ntst, diff_timespec_seconds(end_t, start_t));
  else if(NANOSECOND) PRINT_HK_C(GREEN_K, HK_EQ," %lu tests ran. (%ld ns total)\n",ntst, diff_timespec_nanoseconds(end_t, start_t));
  else PRINT_HK_C(GREEN_K, HK_EQ," %lu tests ran. (%lf ms total)\n",ntst, diff_timespec_milliseconds(end_t, start_t));
  
  PRINT_HK_C(GREEN_K, HK_PS," %lu tests\n", count_pass_global);
  if(failed_l != NULL){
    PRINT_HK_C(RED_K, HK_FL," %lu tests, listed below:\n",count_fail_global); 
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
  //PRINT_HK_C(GREEN_K, HK_EQ," Running %lu tests.\n",count_tests);
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
      begin_execute_func(tmp->name, &start_t);
      tmp->run();
      end_execute_func(tmp->name, start_t);
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
    execute_one_test(f_beging, va_arg(args, size_t));
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

   execute_some_tests_ordered(f_beging, cnt, array);
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

   execute_all_tests_exept(f_beging, cnt, array);
   va_end(args);
   if(count_tests >= cnt)
     stat_end_run(count_tests - cnt, start_t);
}


void
run_all_tests()
{
   struct timespec start_t;
   head_run(count_tests, &start_t);
   execute_all(f_beging);
   stat_end_run(count_tests, start_t);
}

/*
 *
 */


void execute_div_test(struct func *fun, size_t num){
  size_t cur = 0;
  struct timespec start_t;
  struct func *tmp = fun;
  while(tmp){
    if(cur %PARALLEL == num){
      PRINT_DEBUG("thread N° %ld, cur = %ld , cur%PARA = %ld , funcname = %s \n", num,cur, cur%PARALLEL, tmp->name);
      begin_execute_func(tmp->name, &start_t);
      tmp->run();
      end_execute_func(tmp->name, start_t);
    }
    tmp = tmp->next;
    ++cur;
  }
}

void*
run_all_div_tests(void *id)
{
   size_t id_th=*(size_t*)id;
   struct timespec start_t;
   head_run(count_tests/PARALLEL, &start_t);
   execute_div_test(f_beging, id_th);
   stat_end_run(count_tests/PARALLEL, start_t);
}

void run_parallel_tests()
{
  pthread_mutex_init(&mut_count_pass_global, NULL);
  pthread_mutex_init(&mut_count_pass_local, NULL);
  pthread_mutex_init(&mut_count_fail_global, NULL);
  pthread_mutex_init(&mut_count_fail_local, NULL);

  pthread_t *thrd = malloc(PARALLEL * sizeof(pthread_t));
  size_t *id_th = malloc( PARALLEL * sizeof(size_t));

  for(size_t i = 0; i < PARALLEL; ++i){
    id_th[i]=i;
    pthread_create(&thrd[i], NULL, run_all_div_tests, (void*)&id_th[i]);
  }

  for(size_t i=0; i<PARALLEL; ++i){
    pthread_join(thrd[i], NULL);
  }

  free(id_th);
  free(thrd);
  
  pthread_mutex_destroy(&mut_count_pass_global);
  pthread_mutex_destroy(&mut_count_pass_local);
  pthread_mutex_destroy(&mut_count_fail_global);
  pthread_mutex_destroy(&mut_count_fail_local);
}




void 
clear_all_func(struct func **fun)
{
  struct func *tmp = *fun, *ttmp;
  while(tmp != NULL){
    ttmp = tmp;
    tmp = tmp->next;
    free(ttmp);
  }
}
/*
 * to purge func list!
 * optionnal but good practice
 */
void
purge_tests()
{
  struct func *tmp = f_beging;
  clear_all_func(&tmp); 
  PRINT_DEBUG("%s\n","purge done"); 
}

