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

/*
 * by default display in millisecond
 */
/*
#ifndef SECOND
  #define SECOND 0
#endif
#ifndef NANOSECOND
  #define NANOSECOND 0
#endif
*/

#define NANOSECOND  (timeunit[0]=='n')
#define SECOND  (timeunit[0]=='s')

#ifndef PROGRESS
  #define PROGRESS 0
#endif

#if 0
#ifndef SAVE_LOG
#define SAVE_LOG 0
#else
#ifndef ORDER_LOG
  #define ORDER_LOG 1
#endif
#endif
#endif

#if 0
#ifndef PARALLEL
  #define PARALLEL 1
  #define LOCK(mut) 
  #define UNLOCK(mut)
#else /*PARALLEL defined*/
  #define LOCK(mutex_var)  pthread_mutex_lock(&mutex_var);
  #define UNLOCK(mutex_var) pthread_mutex_unlock(&mutex_var);
#endif
#endif /* 0 */

  #define LOCK(mutex_var)  pthread_mutex_lock(&mutex_var);
  #define UNLOCK(mutex_var) pthread_mutex_unlock(&mutex_var);

#define INCREMENT(variable)\
  do{\
    if(is_parallel_nb){\
      LOCK(mut_##variable); \
      ++variable;\
      UNLOCK(mut_##variable);\
    }\
    else ++variable;\
  }while(0);



/*
 * struct to store tests failed
 */ 

struct failed_lists{
  char *name;
  struct failed_lists *next;
};

#define default_ordered 0
#define default_unicolour 0
#define default_removelog  0
#define default_parallel_nb 1


/*
 * global variables not exported
 * only exist in test_t.c
 */
/*
 * begin variable option
 */
bool some_thing_wrong = 0;

bool help=0;
bool ordered= 0;
bool unicolour = 0;
bool removelog = 0;
char *timeunit="ms";
char *savelog=NULL;
char *default_timeunit="ms";
char *default_savelog="log_all_tests";
/*
 * number of threads
 */ 
size_t parallel_nb = 0;

/*
 * end variable option
 */ 

bool is_parallel_nb = 0;
bool progress = false;

FILE **f_ou_th;

size_t count_tests = 0;

size_t count_pass_global = 0;
size_t count_pass_local = 0;


size_t count_fail_global = 0;
size_t count_fail_local = 0;

/*
 * count in local test
 * using array [count_test] global variable
 */
size_t *count_pass_test = NULL;
size_t *count_fail_test = NULL;
 
/*
 * count on each thread [PARALLEL]
 */
size_t *count_pass_thread = NULL;
size_t *count_fail_thread = NULL;

size_t *id_thread_self = NULL;
char **log_name_file_thrd = NULL;
/*
 * the first instance of the func struct, 
 * it containis the first test
 */

struct func *f_beging = NULL; 
/*
 * current test : used by parallel tests
 */ 
struct func *current_fn = NULL; 

/*
 * list of all failed tests
 */ 
struct failed_lists *failed_l = NULL;


/*
 * list of failed test on each thread
 */
struct failed_lists **thread_test_failed_l = NULL;

/*
 * mutex to add global failed test
 */ 
pthread_mutex_t mut_global_list_fail;
/*
 * mutex to have current test to do
 */ 
pthread_mutex_t mut_current_test;


pthread_mutex_t mut_count_pass_global;
pthread_mutex_t mut_count_fail_global;
pthread_mutex_t mut_count_pass_local;
pthread_mutex_t mut_count_fail_local;
/*
 * end of the global variables of test_t.c
 */

/*
 * 
 */ 

void append_failed_list(struct failed_lists **fn_failed_list ,const char *name_failed){

  if(*fn_failed_list){
    
    struct failed_lists *tmp_failed_l = *fn_failed_list, *rec_tmp;
    
    while(tmp_failed_l){
      rec_tmp = tmp_failed_l;
      tmp_failed_l = tmp_failed_l->next; 
    }
    tmp_failed_l = malloc(sizeof(struct failed_lists));
    tmp_failed_l->name = malloc(strlen(name_failed));
    strcpy(tmp_failed_l->name, name_failed);
    tmp_failed_l->next = NULL;
    rec_tmp->next = tmp_failed_l;
  
  }
  else{
    *fn_failed_list = malloc(sizeof(struct failed_lists));
    (*fn_failed_list)->name = malloc(strlen(name_failed));
    strcpy((*fn_failed_list)->name, name_failed);
    (*fn_failed_list)->next = NULL;
  }

}

/*
 * match the id global (gives by OS) of the thread with the local (the program) id of thread
 */ 
long int id_of_thread_executed(void){
  size_t id_from_self = pthread_self();
  for(size_t i=0; i<parallel_nb + 1; ++i){
    if(id_thread_self[i] == id_from_self)
      return i;
  }
  PRINT_ERROR("something wrong on %s\n",__func__);
  return -1;
}


/*
 * format name of TEST(name_f) is: 'TEST_name_f____NUM', 
 * and name attribute is 'TEST(name_f): test N° NUM!' (! at the end is random): 
 * we extract NUM here
 * to have hash_table of the count when parallel test!
 */
size_t extract_num__f(const char *name_f){
  size_t len = strlen(name_f);
  size_t val = 0, p = 1;
  for(long i= len-1; i>=0; --i){
    PRINT_DEBUG(" name_f[%ld] = %c\n",i,name_f[i]);
    if(name_f[i] >= '0' && name_f[i] <= '9'){
      val += p * (name_f[i]-'0');
      p *= 10;
    }
    else if( name_f[i] == ' ' ||  name_f[i] == '_' || name_f[i] == '=' ) break;
  }
  return val;
}

void usage(int argc, char **argv){
  printf("usage: %s [OPTIONS] [<ARGS>] \n\n  or : %s [OPTIONS]=[<ARGS>]\n\n",argv[0],argv[0]);
  printf("OPTIONS\n");
  printf("\t -h, --help \n\t\tprint help, options variables\n\n");
  printf("\t -p <NB>, --parallel <NB>, -p=<NB>, --parallel=<NB>\n\t\tby default the program ran in sequantial all test, \n\t\tif this option is set, the program run tests on NB threads.\n\t\tEach thread pull up one test out the list of all test not yet executed,\n\t\tand execute it, until the list is empty \n\n");
  printf("\t -t <unit>, --time <unit>, -t=<unit>, --time=<unit>  \n\t\tby default unit is millisecons ms, the other of unit are choices are second (or s), and nanosecond (or ns)\n\t\tex: -t ns or -t=nanosecond or --time=n to set nanosecond unit\n\n");
  printf("\t -u , --unicolour\n\t\tby default, the result is colored, if you choice this option, it prints with default color\n\n");
  printf("\t -o, --ordered\n\t\tthis option is usefull if you choose to use parallel tests,\n\t\tby default, each thread share the screen to print results,\n\t\tthis option create file to record log of each thread on file,\n\t\tand print on screen all results at the end of all tests\n\n");
  printf("\t -r , --remove\n\t\tif the option ordered is choosen if parallel tests,\n\t\tthis option remove the file logs of each thread after all tests.\n\n");
  printf("\t -s <file>, --savelog <file>, -s=file, --savelog=file\n\t\tthis option save the global ordered result in 'file',\n\t\this option active the option -o or --ordered. \n\n");

  if(some_thing_wrong){
    printf("invalid argument\n");
    exit(0);
  }
}


const char* extract_string_after_equal_symbole_in_string(const char * in_str){
  size_t len=strlen(in_str);
    
  for(long i=0; i<len-1; ++i){
    if(in_str[i]=='=') 
      return in_str+i+1;
  }
  return NULL; // check for '\0' or ' ' return !
}

long int extract_num_after_equal_symbole_in_string(char * in_str){
  size_t len=strlen(in_str);
  long int val=0, p=1;
  for(long i=len-1; i>=0; --i){
    PRINT_DEBUG("(%s)[%ld]=%c\n",in_str,i,in_str[i]);
    if(in_str[i]=='=') return val;
    if(in_str[i] >= '0' && in_str[i] <= '9' ){
      val += p * (in_str[i]-'0');
      p *= 10;
    }
  }
  if(val) return val;
  return -1;
} 

#define IF_OPTION_WITH_ARG_NUM(option)\
  if(argv[i][0]=='-'){\
    j=1;\
    if(argv[i][j]=='-') ++j;\
    if(argv[i][j] == #option[0]){\
      long ret_num=extract_num_after_equal_symbole_in_string(argv[i]);\
    PRINT_DEBUG("option=%s, ret_num = %ld, argv[%d]=%s\n",#option,ret_num,i,argv[i]);\
      is_##option = 1;\
      if(ret_num > -1)\
        option = ret_num;\
      else{\
        if(i<argc-1){\
          if(argv[i+1][0]=='-'){\
            help=1;\
            option = default_##option;\
            some_thing_wrong = 1;\
          }\
          else{\
            ret_num=extract_num_after_equal_symbole_in_string(argv[++i]);\
            if(ret_num>0)\
              option = ret_num;\
            else{ \
              help=1;\
              option = default_##option;\
              some_thing_wrong = 1;\
            }\
          }\
        }\
        else{\
          help=1;\
          option = default_##option;\
          some_thing_wrong = 1;\
        }\
      }\
    PRINT_DEBUG("option %s activated, its value is %ld\n",#option,option);\
      continue;\
    }\
  }\

#define IF_OPTION_WITH_ARG_STR(option)\
  if(argv[i][0]=='-'){\
    j=1;\
    if(argv[i][j]=='-') ++j;\
    if(argv[i][j] == #option[0]){\
      char* ret_str=(char*)extract_string_after_equal_symbole_in_string(argv[i]);\
    PRINT_DEBUG("option=%s, ret_str = %s, argv[%d]=%s\n",#option,ret_str,i,argv[i]);\
      if(ret_str ==NULL || strlen(ret_str)==0){\
        if(i<argc-1){\
          if(argv[i+1][0]=='-')\
            help=1;\
          else\
            option = argv[++i];\
        }\
        else{\
          help=1;\
        }\
      }\
      else option = ret_str;\
      continue;\
    }\
  }\
       
/*
 * if the variable option is boolean
 */

#define IF_OPTION_NO_ARG(option)\
  if(argv[i][0]=='-'){\
    j=1;\
    if(argv[i][j]=='-') ++j;\
    if(argv[i][j] == #option[0]){\
      option=1;\
      continue;\
    }\
  }\


void parse_options(int argc, char **argv){
  int j;
  for(int i=1; i<argc; ++i){
    PRINT_DEBUG("argc=%d, argv[%d]=%s\n",argc,i,argv[i]);
    IF_OPTION_NO_ARG(help)
    IF_OPTION_WITH_ARG_NUM(parallel_nb)
    IF_OPTION_WITH_ARG_STR(savelog)
    IF_OPTION_WITH_ARG_STR(timeunit)
    IF_OPTION_NO_ARG(ordered)
    IF_OPTION_NO_ARG(removelog)    
    IF_OPTION_NO_ARG(unicolour)    
  }

}



/*
 * print all TESTs failed
 */ 

#define LISTE_ALL_FAILED_TEST_IN_F_OUT\
  while(failed_lst){\
      PRINT_HK_C(RED_K, HK_FL," %s\n",failed_lst->name);\
      failed_lst = failed_lst->next;\
    }


void list_failed_test(struct failed_lists *test_failed){
  struct failed_lists *failed_lst = test_failed;
  if(is_parallel_nb){
    long int  id_thrd = id_of_thread_executed();
    if(id_thrd < 0 || id_thrd == parallel_nb ){
      LISTE_ALL_FAILED_TEST_IN_F_OUT
    }else{
      while(failed_lst){
        PRINT_HK_C(RED_K, HK_FL," %s, on thread[%ld]\n",failed_lst->name,id_thrd);
        failed_lst = failed_lst->next;
      }
    }
  }
  else{
    LISTE_ALL_FAILED_TEST_IN_F_OUT
  }
  PRINT_HK_C(DEFAULT_K, HK_EQ,"%s\n","");
}





#define INCREMENT_EXPECT(expect,name)\
  do{\
    size_t num_test=extract_num__f(name);\
    ++count_## expect ##_test[num_test];\
    PRINT_DEBUG("INCREMENT cout_%s_test[%ld] = %ld\n",#expect, num_test,count_## expect ##_test[num_test]); \
  }while(0);


#define EXPECTED_EXPECT_F(expect/*, not_expect*/)                                          \
  \
bool expected_##expect##_f(bool val){                                                     \
  if(val == expect) {                                                                     \
    INCREMENT(count_pass_local); /*++count_pass_local*/                                                                 \
    return true;                                                                          \
  }else {                                                                                 \
    INCREMENT(count_fail_local); /*++count_fail_local*/                                                                 \
    return false;                                                                         \
  }                                                                                       \
}  \
bool expected_##expect##_f_name(bool val, const char * name){                                                     \
  if(val == expect) {                                                                     \
    INCREMENT_EXPECT(pass,name);\
    return true;                                                                          \
  }else {                                                                                 \
    INCREMENT_EXPECT(fail,name);\
    return false;                                                                         \
  }                                                                                       \
}  \


EXPECTED_EXPECT_F(true)
EXPECTED_EXPECT_F(false)

#define EXPECTED_OP_TYPE(OP,type)                                                            \
    \
bool expected_##OP##_##type(type var1, type var2){                                            \
  if(COMPARE_N_##type(&var1, &var2) OP 0){                                                \
    INCREMENT(count_pass_local); /*++count_pass_local*/                                                                 \
    return true;                                                                          \
  }else {                                                                                 \
    INCREMENT(count_fail_local); /*++count_fail_local*/                                                                 \
    return false;                                                                         \
  }                                                                                       \
} \
bool expected_##OP##_name_##type(type var1, type var2,const char * name){                                            \
  if(COMPARE_N_##type(&var1, &var2) OP 0){                                                \
    INCREMENT_EXPECT(pass,name);\
    return true;                                                                          \
  }else {                                                                                 \
    INCREMENT_EXPECT(fail,name);\
    return false;                                                                         \
  }                                                                                       \
}

EXPECTED_OP_TYPE(EQ,TYPE_CHAR)
EXPECTED_OP_TYPE(EQ,TYPE_U_CHAR)
EXPECTED_OP_TYPE(EQ,TYPE_INT)
EXPECTED_OP_TYPE(EQ,TYPE_U_INT)
EXPECTED_OP_TYPE(EQ,TYPE_L_INT)
EXPECTED_OP_TYPE(EQ,TYPE_U_L_INT)
EXPECTED_OP_TYPE(EQ,TYPE_SIZE_T)
EXPECTED_OP_TYPE(EQ,TYPE_FLOAT)
EXPECTED_OP_TYPE(EQ,TYPE_DOUBLE)
EXPECTED_OP_TYPE(EQ,TYPE_L_DOUBLE)
EXPECTED_OP_TYPE(EQ,TYPE_STRING)

EXPECTED_OP_TYPE(LT,TYPE_CHAR)
EXPECTED_OP_TYPE(LT,TYPE_U_CHAR)
EXPECTED_OP_TYPE(LT,TYPE_INT)
EXPECTED_OP_TYPE(LT,TYPE_U_INT)
EXPECTED_OP_TYPE(LT,TYPE_L_INT)
EXPECTED_OP_TYPE(LT,TYPE_U_L_INT)
EXPECTED_OP_TYPE(LT,TYPE_SIZE_T)
EXPECTED_OP_TYPE(LT,TYPE_FLOAT)
EXPECTED_OP_TYPE(LT,TYPE_DOUBLE)
EXPECTED_OP_TYPE(LT,TYPE_L_DOUBLE)
EXPECTED_OP_TYPE(LT,TYPE_STRING)

EXPECTED_OP_TYPE(GT,TYPE_CHAR)
EXPECTED_OP_TYPE(GT,TYPE_U_CHAR)
EXPECTED_OP_TYPE(GT,TYPE_INT)
EXPECTED_OP_TYPE(GT,TYPE_U_INT)
EXPECTED_OP_TYPE(GT,TYPE_L_INT)
EXPECTED_OP_TYPE(GT,TYPE_U_L_INT)
EXPECTED_OP_TYPE(GT,TYPE_SIZE_T)
EXPECTED_OP_TYPE(GT,TYPE_FLOAT)
EXPECTED_OP_TYPE(GT,TYPE_DOUBLE)
EXPECTED_OP_TYPE(GT,TYPE_L_DOUBLE)
EXPECTED_OP_TYPE(GT,TYPE_STRING)

EXPECTED_OP_TYPE(LE,TYPE_CHAR)
EXPECTED_OP_TYPE(LE,TYPE_U_CHAR)
EXPECTED_OP_TYPE(LE,TYPE_INT)
EXPECTED_OP_TYPE(LE,TYPE_U_INT)
EXPECTED_OP_TYPE(LE,TYPE_L_INT)
EXPECTED_OP_TYPE(LE,TYPE_U_L_INT)
EXPECTED_OP_TYPE(LE,TYPE_SIZE_T)
EXPECTED_OP_TYPE(LE,TYPE_FLOAT)
EXPECTED_OP_TYPE(LE,TYPE_DOUBLE)
EXPECTED_OP_TYPE(LE,TYPE_L_DOUBLE)
EXPECTED_OP_TYPE(LE,TYPE_STRING)

EXPECTED_OP_TYPE(GE,TYPE_CHAR)
EXPECTED_OP_TYPE(GE,TYPE_U_CHAR)
EXPECTED_OP_TYPE(GE,TYPE_INT)
EXPECTED_OP_TYPE(GE,TYPE_U_INT)
EXPECTED_OP_TYPE(GE,TYPE_L_INT)
EXPECTED_OP_TYPE(GE,TYPE_U_L_INT)
EXPECTED_OP_TYPE(GE,TYPE_SIZE_T)
EXPECTED_OP_TYPE(GE,TYPE_FLOAT)
EXPECTED_OP_TYPE(GE,TYPE_DOUBLE)
EXPECTED_OP_TYPE(GE,TYPE_L_DOUBLE)
EXPECTED_OP_TYPE(GE,TYPE_STRING)

EXPECTED_OP_TYPE(NE,TYPE_CHAR)
EXPECTED_OP_TYPE(NE,TYPE_U_CHAR)
EXPECTED_OP_TYPE(NE,TYPE_INT)
EXPECTED_OP_TYPE(NE,TYPE_U_INT)
EXPECTED_OP_TYPE(NE,TYPE_L_INT)
EXPECTED_OP_TYPE(NE,TYPE_U_L_INT)
EXPECTED_OP_TYPE(NE,TYPE_SIZE_T)
EXPECTED_OP_TYPE(NE,TYPE_FLOAT)
EXPECTED_OP_TYPE(NE,TYPE_DOUBLE)
EXPECTED_OP_TYPE(NE,TYPE_L_DOUBLE)
EXPECTED_OP_TYPE(NE,TYPE_STRING)


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
    append_failed_list(&failed_l, fun_ame); 
    PRINT_HK_C(RED_K, HK_FL, " %lu tests failed from %s\n",count_fail_local,fun_ame);
    PRINT_TIMESTAMP_STAT(RED_K);
  }
  else 
  {
    INCREMENT(count_pass_global); /*++count_pass_global*/
    PRINT_TIMESTAMP_STAT(GREEN_K);
  }
}
/*
 * print on the top of test
 */ 
void head_run(size_t nbtest, struct timespec *start_t){
  clock_gettime(CLOCK_REALTIME, start_t);
  PRINT_HK_C(GREEN_K, HK_EQ," Running %lu tests.\n",nbtest);
}

/*
 * printing on the end of test
 */ 
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
    PRINT_HK_C("","","\n%ld FAILED TESTS \n",count_fail_global);
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

#if 0
/*
 * parallel run tests / div section
 */
void execute_div_test(struct func *fun, size_t num){
  size_t cur = 0;
  struct timespec start_t;
  struct func *tmp = fun;
  while(tmp){
    if(cur %PARALLEL == num){
      PRINT_DEBUG("thread [%ld], cur = %ld , cur mod PARA = %ld , funcname = %s \n", num,cur, cur%PARALLEL, tmp->name);
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
#endif
/*
 * begin end parallel tests
 */ 
/*
 * print on the top of all test  (parallel case)
 */
void head_all_parallel_run(struct timespec *start_t){
  clock_gettime(CLOCK_REALTIME, start_t);
  PRINT_HK_C(GREEN_K, HK_EQ," Running tests on %ld threads\n", parallel_nb);
}

/*
 * print on the top of test in parallel
 */ 
void head_parallel_run(struct timespec *start_t, size_t id_thrd){
  sprintf(log_name_file_thrd[id_thrd],"log_thread_%ld_id_%ld",id_thrd,pthread_self());
  f_ou_th[id_thrd] = fopen(log_name_file_thrd[id_thrd], "w+"); 
  clock_gettime(CLOCK_REALTIME, start_t);
  PRINT_HK_C(GREEN_K, HK_EQ," Running tests on thread[%ld] ========== ==threadID== %ld \n", id_thrd,pthread_self());
}

/*
 * printing stat of each thread tests
 */ 
void
stat_end_parallel_run(size_t ntst, struct timespec start_t, size_t id_thrd){
  struct timespec end_t; clock_gettime(CLOCK_REALTIME, &end_t);
  
  if(SECOND) PRINT_HK_C(GREEN_K, HK_EQ," %lu tests ran on thread[%ld]. (%lf s total) \n",ntst, id_thrd, diff_timespec_seconds(end_t, start_t));
  else if(NANOSECOND) PRINT_HK_C(GREEN_K, HK_EQ," %lu tests ran on thread[%ld]. (%ld ns total)\n",ntst, id_thrd, diff_timespec_nanoseconds(end_t, start_t));
  else PRINT_HK_C(GREEN_K, HK_EQ," %lu tests ran on thread[%ld]. (%lf ms total)\n",ntst, id_thrd, diff_timespec_milliseconds(end_t, start_t));
  
  PRINT_HK_C(GREEN_K, HK_PS," %lu tests passed on thread[%ld]\n", count_pass_thread[id_thrd], id_thrd);
  if(thread_test_failed_l[id_thrd] != NULL){
    PRINT_HK_C(RED_K, HK_FL," %lu tests failed on thread[%ld], listed below:\n",count_fail_thread[id_thrd],id_thrd); 
    list_failed_test(thread_test_failed_l[id_thrd]);
  }
}
/*
 * stat of all tests on all threads
 */ 

void
stat_end_all_parallel_run(size_t ntst, struct timespec start_t){
  struct timespec end_t; clock_gettime(CLOCK_REALTIME, &end_t);

  //PRINT_HK_C(DEFAULT_K, HK_EQ," %s: all parallel tests done\n\n",__FILE__);
  
  if(SECOND) PRINT_HK_C(GREEN_K, HK_EQ," %lu tests ran on %ld threads. (%lf s total) \n",ntst, parallel_nb, diff_timespec_seconds(end_t, start_t));
  else if(NANOSECOND) PRINT_HK_C(GREEN_K, HK_EQ," %lu tests ran on %ld threads. (%ld ns total)\n",ntst, parallel_nb, diff_timespec_nanoseconds(end_t, start_t));
  else PRINT_HK_C(GREEN_K, HK_EQ," %lu tests ran on %ld threads. (%lf ms total)\n",ntst, parallel_nb, diff_timespec_milliseconds(end_t, start_t));
  
  PRINT_HK_C(GREEN_K, HK_PS," %lu tests\n", count_pass_global);
  if(failed_l != NULL){
    PRINT_HK_C(RED_K, HK_FL," %lu tests, listed below:\n",count_fail_global); 
    list_failed_test(failed_l);
    PRINT_HK_C("","","\n%ld FAILED TESTS \n",count_fail_global);
  }
}



void begin_execute_func_parallel(char *fun_ame, struct timespec *start_t, size_t id_thrd){
  clock_gettime(CLOCK_REALTIME, start_t);
  PRINT_HK_C(GREEN_K,HK_RN," %s on thread[%ld]\n", fun_ame, id_thrd); 
}

#define PRINT_TIMESTAMP_STAT_PARALLEL(color)\
    if(SECOND) PRINT_HK_C(color,HK_DN," %lu tests passed from %s (%lf s), on thread[%ld]\n\n",count_pass_test[num_test],fun_ame, diff_timespec_seconds(end_t, start_t),id_thrd);\
    else if(NANOSECOND) PRINT_HK_C(color,HK_DN," %lu tests passed from %s (%ld ns), on thread[%ld]\n\n",count_pass_test[num_test],fun_ame, diff_timespec_nanoseconds(end_t, start_t),id_thrd);\
    else PRINT_HK_C(color,HK_DN," %lu tests passed from %s (%lf ms), on thread[%ld]\n\n",count_pass_test[num_test],fun_ame, diff_timespec_milliseconds(end_t, start_t),id_thrd);

void end_execute_func_parallel(char *fun_ame, struct timespec start_t, size_t id_thrd){
  struct timespec end_t; clock_gettime(CLOCK_REALTIME, &end_t);
  size_t num_test = extract_num__f(fun_ame);  
  PRINT_DEBUG(" ... thread[%ld], count_fail_test[%ld] = %ld ... %s\n", id_thrd, num_test, count_fail_test[num_test],fun_ame);
  if(count_fail_test[num_test]){
    INCREMENT(count_fail_global); /*++count_fail_global*/
    append_failed_list(&thread_test_failed_l[id_thrd], fun_ame); 
    ++(count_fail_thread[id_thrd]);
    LOCK(mut_global_list_fail);
    append_failed_list(&failed_l, fun_ame); 
    UNLOCK(mut_global_list_fail);
    PRINT_HK_C(RED_K, HK_FL, " %lu tests failed from %s on thread[%ld], %ld tests failed on thread[%ld]\n",count_fail_test[num_test],fun_ame, id_thrd,count_fail_thread[id_thrd],id_thrd);
    PRINT_TIMESTAMP_STAT_PARALLEL(RED_K);
  }
  else 
  {
    ++(count_pass_thread[id_thrd]);
    INCREMENT(count_pass_global); /*++count_pass_global*/
    PRINT_TIMESTAMP_STAT_PARALLEL(GREEN_K);
  }
}

#if 1
void progress_test_(int max_colon){
  struct func *tmp;
  size_t num_test=0;
  int  cur = 0, len;
  //get_cursor_position(&col, &row);

  char prgss[]="/ | --";
  len=strlen(prgss);
  do{
    LOCK(mut_current_test);
    tmp = current_fn;
    UNLOCK(mut_current_test);
    if(tmp)
      num_test = extract_num__f(tmp->name);
    gotoxy(13,0);
    for(int i=0; i<(num_test*max_colon/count_tests); ++i) printf("#");
    //printf("%c",prgss[cur]);
    printf("\33[2K\r");  /* remove current line and go to begin of the current line */
    if(cur<len-1) ++cur;
    else cur=0;
    //printf("\n");
    //sleep(1);
    //usleep(50000);
  }while(tmp);

}

void*
run_progress_tests(void *max_d)
{
   int max_col = 180; //*(int*)max_d;
   //progress_test_(max_col);
}

#endif

void execute_test_parallel(size_t id_thrd){
  
  struct timespec start_t;
  struct func *tmp;
 
  do{ 
    LOCK(mut_current_test);
    tmp = current_fn;
    if(tmp){
      current_fn = tmp->next;
      UNLOCK(mut_current_test);
      PRINT_DEBUG(" *** thread[%ld], func_name = %s *** \n", id_thrd, tmp->name);
      begin_execute_func_parallel(tmp->name, &start_t, id_thrd);
      tmp->run();
      end_execute_func_parallel(tmp->name, start_t, id_thrd);
    }
    else{
      UNLOCK(mut_current_test);
    }
  }while(tmp);
}

void*
run_parallel_tests(void *id)
{
   size_t id_th=*(size_t*)id;
   id_thread_self[id_th] = pthread_self();
   struct timespec start_t;
   head_parallel_run(&start_t, id_th);
   execute_test_parallel(id_th);
   stat_end_parallel_run(count_fail_thread[id_th]+count_pass_thread[id_th], start_t, id_th);
}

/*
 * initialisation
 */ 
void
init_parallel_test_()
{
  progress = PROGRESS;

  is_parallel_nb = 1;
  
  f_ou_th = malloc((parallel_nb + 1) *sizeof(FILE*));
  log_name_file_thrd = malloc((parallel_nb + 1) *sizeof(char*));
  for(size_t i=0; i<=parallel_nb; ++i){
    log_name_file_thrd[i] = malloc((256) *sizeof(char));
  }

  /*
   *  on thread principale
   */
  sprintf(log_name_file_thrd[parallel_nb],"log_principal_thread_%ld_id_%ld",parallel_nb,pthread_self());
  f_ou_th[parallel_nb] = fopen(log_name_file_thrd[parallel_nb], "w+");



  count_pass_test = malloc(count_tests * sizeof(size_t));
  count_fail_test = malloc(count_tests * sizeof(size_t));
  for(size_t i=0; i<count_tests; ++i){
    count_pass_test[i]=0;
    count_fail_test[i]=0;
  }

  thread_test_failed_l = malloc(parallel_nb * sizeof(struct failed_lists*));
  count_pass_thread = malloc(parallel_nb * sizeof(size_t));
  count_fail_thread = malloc(parallel_nb * sizeof(size_t));
  id_thread_self = malloc((parallel_nb + 1) * sizeof(size_t));
  
  id_thread_self[parallel_nb]=   pthread_self();// main thread 

  for(size_t i=0; i<parallel_nb; ++i){
    thread_test_failed_l[i] = NULL;
    count_pass_thread[i] = 0;
    count_fail_thread[i] = 0;
  }

  current_fn = f_beging;

  pthread_mutex_init(&mut_global_list_fail, NULL);
  pthread_mutex_init(&mut_current_test, NULL);
  pthread_mutex_init(&mut_count_pass_global, NULL);
  pthread_mutex_init(&mut_count_fail_global, NULL);
  pthread_mutex_init(&mut_count_pass_local, NULL);
  pthread_mutex_init(&mut_count_fail_local, NULL);
}
/*
 * finalisation, cleanup
 */ 
void
final_parallel_test_()
{
  free(count_pass_test);
  free(count_fail_test);
  free(count_pass_thread);
  free(count_fail_thread);

  free(thread_test_failed_l);

  
  pthread_mutex_destroy(&mut_global_list_fail);
  pthread_mutex_destroy(&mut_current_test);
  pthread_mutex_destroy(&mut_count_pass_global);
  pthread_mutex_destroy(&mut_count_fail_global);
  pthread_mutex_destroy(&mut_count_pass_local);
  pthread_mutex_destroy(&mut_count_fail_local);


  char reader[256]; 
  
  if(savelog){
    FILE *f_savelog;
    f_savelog=fopen(savelog, "w+");
    for(size_t id_thrd =0 ; id_thrd <= parallel_nb; ++id_thrd){
      rewind(f_ou_th[id_thrd]); // put the file pointer to the begin of file;
      while(fgets(reader, 255,f_ou_th[id_thrd] )){
        fprintf(F_OUT,"%s",reader);
        fprintf(f_savelog,"%s",reader);
      }
      fclose(f_ou_th[id_thrd]);
    }
    fclose(f_savelog);
  }else{
    for(size_t id_thrd =0 ; id_thrd <= parallel_nb; ++id_thrd){
      rewind(f_ou_th[id_thrd]); // put the file pointer to the begin of file;
      while(fgets(reader, 255,f_ou_th[id_thrd] ))
        fprintf(F_OUT,"%s",reader); 
      fclose(f_ou_th[id_thrd]);
    }
  }
  
  if(removelog){
    for(size_t i=0; i<=parallel_nb; ++i){
      remove(log_name_file_thrd[i]);
    }
  }


}

void run_all_tests_parallel(size_t parallel /*, int max_col*/)
{
  parallel_nb = parallel; /* need to be here to initialise parallel_nb for init_parallel_test_ */
  
  init_parallel_test_();
  
  struct timespec start_t;
  
  head_all_parallel_run(&start_t);

#if 1
  pthread_t thrd_progress;
if(progress)  pthread_create(&thrd_progress, NULL, run_progress_tests, NULL);
//if(progress)  pthread_create(&thrd_progress, NULL, run_progress_tests, (void*)&max_col);
#endif

  pthread_t *thrd = malloc(parallel_nb * sizeof(pthread_t));
  size_t *id_th = malloc( parallel_nb * sizeof(size_t));

  for(size_t i = 0; i < parallel_nb; ++i){
    id_th[i]=i;
    pthread_create(&thrd[i], NULL, run_parallel_tests, (void*)&id_th[i]);
  }

  for(size_t i=0; i<parallel_nb; ++i){
    pthread_join(thrd[i], NULL);
  }

if(progress)  pthread_join(thrd_progress, NULL);
  
  stat_end_all_parallel_run(count_tests, start_t );

  free(id_th);
  free(thrd);
  
  final_parallel_test_();
}


void run_all_tests_args(int argc, char **argv){
  
  parse_options(argc,argv);
  if(help) usage(argc,argv);
  if(is_parallel_nb) run_all_tests_parallel(parallel_nb);
  else run_all_tests();
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
__attribute__((destructor)) 
void
purge_tests()
{
  struct func *tmp = f_beging;
  clear_all_func(&tmp); 
  PRINT_DEBUG("%s\n","purge done"); 
}

