#ifndef __TEST_C_H__
#define __TEST_C_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
//#include <time.h>
#include <pthread.h>
//#include <unistd.h>
#include <sys/ioctl.h> /* to have size of screen, for progress bar */

#include "tools_t/tools_t.h"
#include "bar_progress/bar_progress.h"

#define DEFAULT_K "\033[0m" //Resets the text to default color
#define GREEN_K "\033[0;32m" 
#define RED_K "\033[0;31m"
#define YELLOW_K "\033[0;33m"
#define BLUE_K "\033[0;34m"


#define COLOR_SZ 6

#define Dknothing COLOR_SZ - 1

#define SZ_TAB_HK 8


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
#endif /* HK */

/*
 * compare symbol
 */
#define EQ ==
#define LT <
#define GT >
#define LE <=
#define GE >=
#define NE !=


extern FILE **f_ou_th;
extern bool unicolour;
extern bool ordered;
extern bool log_parallel;

extern char *colors_f[];
extern char *tab_hk_f[];

extern int kdefault, kgreen, kred, kyellow, kblue, knothing;
extern int hk_EQ, hk_TR, hk_RN, hk_DN, hk_OK, hk_FL, hk_PS, hk_SK;

#ifndef SAVE_LOG
#define SAVE_LOG 0
#else
#ifndef ORDER_LOG
  #define ORDER_LOG 1
#endif
#endif

#define BUF_SIZE 256

#define INIT_STREAM_(stream,buf,len)\
  FILE *stream;\
  char *buf;\
  size_t len;\
  stream = open_memstream (&buf, &len);\
  if (stream == NULL) { fprintf(stderr," error open_memstream %s:%d:%s \n",__FILE__,__LINE__,__func__); exit(0); }
 

#define CLOSE_STREAM_(stream, buf)\
  fclose (stream);\
  free (buf);

#define directory_in_memory "/dev/shm"

#define BUILD_PATH_ID_FILE(dir,id) STRFY(dir/id)

#define INIT_STREAM_MEM(stream, buf)\
  char *buf = malloc(BUF_SIZE);\
  char *filename=malloc(strlen(directory_in_memory) + strlen("tmp_")+32) ;\
  sprintf(filename,"%s/tmp_%ld",directory_in_memory,pthread_self());\
  FILE *stream = fopen(filename,"w+");\
  if (stream == NULL) { fprintf(stderr," error open stream on \'tmp\' %s:%d:%s \n",__FILE__,__LINE__,__func__); exit(0); }
  
#define BEGIN_CPY_STREAM_MEM(stream, buf)\
  rewind(stream);\
  while(fgets(buf, BUF_SIZE, stream)){
    
  
#define END_CPY_STREAM_MEM(stream, buf)\
  }\
  fclose(stream);\
  free(buf);\
  remove(filename);


#if 1
/*
 *  to execute once in print functions in the case of log_parallel (printing on screen and recording in file), we have to copy to string before copy it, 
 *  I've tried open_memstream but it have some bugs. 
 *  so I use normal fopen a file a memory location '/dev/shm', it is remove after use!
 */
#define PRINT_HK_C(color,hk,...)\
   do{ \
      if(is_parallel_nb){\
        size_t id_thread=id_of_thread_executed();\
        if(log_parallel){\
          INIT_STREAM_MEM(stream, msg);\
          if(!unicolour) fprintf(stream, color hk DEFAULT_K  __VA_ARGS__); \
          else fprintf(stream, hk  __VA_ARGS__); \
          BEGIN_CPY_STREAM_MEM (stream, msg)\
            fprintf(F_OUT,"%s",msg);\
            if(id_thread >= 0){\
              fprintf(f_ou_th[id_thread],"%s",msg); \
            }\
          END_CPY_STREAM_MEM (stream, msg);\
        }\
        else{\
          if(id_thread < 0){\
            if(!unicolour) fprintf(F_OUT, color hk DEFAULT_K  __VA_ARGS__); \
            else fprintf(F_OUT, hk  __VA_ARGS__); \
          }\
          else{\
            if(!unicolour) fprintf(f_ou_th[id_thread], color hk DEFAULT_K  __VA_ARGS__); \
            else fprintf(f_ou_th[id_thread], hk  __VA_ARGS__); \
          }\
        }\
      } \
    else{\
       if(!unicolour) fprintf(F_OUT, color hk DEFAULT_K  __VA_ARGS__); \
       else fprintf(F_OUT, hk  __VA_ARGS__);  \
    }\
  }while(0) 

   

#define PRINT_LOC(fmt, ...) \
  do{ \
    /*if(ordered){*/\
      if(is_parallel_nb){\
        size_t id_thread=id_of_thread_executed();\
        if(log_parallel){\
          INIT_STREAM_MEM (stream, msg);\
          fprintf(stream, "%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, __VA_ARGS__);\
          BEGIN_CPY_STREAM_MEM (stream, msg);\
          fprintf(F_OUT, "%s",msg);\
          if(id_thread >= 0){\
            fprintf(f_ou_th[id_thread], "%s",msg);\
          }\
          END_CPY_STREAM_MEM (stream, msg);\
        }\
        else{\
          if(id_thread < 0){\
            fprintf(F_OUT, "%s:%d:%s(): " fmt, __FILE__, \
            __LINE__, __func__, __VA_ARGS__);\
          }\
          else{\
            fprintf(f_ou_th[id_thread], "%s:%d:%s(): " fmt, __FILE__, \
            __LINE__, __func__, __VA_ARGS__);\
          }\
        }\
      } \
     else{\
      fprintf(F_OUT, "%s:%d:%s(): " fmt, __FILE__, \
      __LINE__, __func__, __VA_ARGS__);\
    }\
  }while(0)

#define PRINTF( ...) \
  do{ \
    if(is_parallel_nb){\
      size_t id_thread=id_of_thread_executed();\
      if(log_parallel){\
        INIT_STREAM_MEM (stream, msg);\
        fprintf(stream,  __VA_ARGS__);\
        BEGIN_CPY_STREAM_MEM (stream, msg);\
        fprintf(F_OUT,"%s",msg);\
        if(id_thread >= 0){\
          fprintf(f_ou_th[id_thread],"%s",msg);\
        }\
        END_CPY_STREAM_MEM(stream, msg);\
      }\
      else{\
        if(id_thread < 0){\
          fprintf(F_OUT,__VA_ARGS__);\
        }\
        else{\
          fprintf(f_ou_th[id_thread], __VA_ARGS__);\
        }\
      }\
    } \
    else{\
      fprintf(F_OUT, __VA_ARGS__);\
    }\
  }while(0)

#define LOG(...) PRINTF(__VA_ARGS__)

//#endif
#else  /* below alternative of above solution, but it execute twice functions called in print functions when log_parallel == 1 */
//#if 1

#define PRINT_LOC(fmt, ...) \
  do{ \
    /*if(ordered){*/\
      if(is_parallel_nb){\
        size_t id_thread=id_of_thread_executed();\
        if(id_thread < 0){\
          fprintf(F_OUT, "%s:%d:%s(): " fmt, __FILE__, \
          __LINE__, __func__, __VA_ARGS__);\
        }\
        else{\
          fprintf(f_ou_th[id_thread], "%s:%d:%s(): " fmt, __FILE__, \
          __LINE__, __func__, __VA_ARGS__);\
        }\
      } \
      /*else{\
        fprintf(F_OUT, "%s:%d:%s(): " fmt, __FILE__, \
          __LINE__, __func__, __VA_ARGS__);\
      }\
    }\
    else{*/if(log_parallel || !is_parallel_nb){\
      fprintf(F_OUT, "%s:%d:%s(): " fmt, __FILE__, \
      __LINE__, __func__, __VA_ARGS__);\
    }\
  }while(0)

#define PRINTF( ...) \
  do{ \
    printf("\n\n sizeof VARGS:%ld \n\n",sizeof(__VA_ARGS__));\
    /*if(ordered){*/\
      if(is_parallel_nb){\
        size_t id_thread=id_of_thread_executed();\
        if(id_thread < 0){\
          fprintf(F_OUT,__VA_ARGS__);\
        }\
        else{\
          fprintf(f_ou_th[id_thread], __VA_ARGS__);\
        }\
      } \
      /*else{\
         fprintf(F_OUT, __VA_ARGS__);\
      }\
    }\
    else{*/if(log_parallel || !is_parallel_nb){\
      fprintf(F_OUT, __VA_ARGS__);\
    }\
  }while(0)

#define LOG(...) PRINTF(__VA_ARGS__)

#define PRINT_HK_C(color,hk,...)\
  do{ \
    /*if(ordered){*/\
      if(is_parallel_nb){\
        size_t id_thread=id_of_thread_executed();\
        if(id_thread < 0){\
          if(!unicolour) fprintf(F_OUT, color hk DEFAULT_K  __VA_ARGS__); \
          else fprintf(F_OUT, hk  __VA_ARGS__); \
        }\
        else{\
          if(!unicolour) fprintf(f_ou_th[id_thread], color hk DEFAULT_K  __VA_ARGS__); \
          else fprintf(f_ou_th[id_thread], hk  __VA_ARGS__); \
        }\
      } \
      /*else{\
        if(!unicolour) fprintf(F_OUT, color hk DEFAULT_K  __VA_ARGS__); \
        else fprintf(F_OUT, hk  __VA_ARGS__); \
      }*/\
    /*}*/\
    /*else{*/if(log_parallel || !is_parallel_nb){\
       if(!unicolour) fprintf(F_OUT, color hk DEFAULT_K  __VA_ARGS__); \
       else fprintf(F_OUT, hk  __VA_ARGS__);  \
    }\
  }while(0) 

#endif    


#if 0

/*
 * print [ HK_NAME ] with color
 */
#define PRINT_HK_C(color,hk,format,...)\
  do{ if(!unicolour) fprintf(F_OUT, color hk DEFAULT_K format, __VA_ARGS__); \
      else fprintf(F_OUT, hk format, __VA_ARGS__); } while(0) \

#endif /* 0 */
      
/*
 * to skip the bloc test function
 */
#define SKIP(...)\
  PRINT_HK_C(GREEN_K, HK_SK __VA_ARGS__);\
  PRINT_LOC("%s\n\n" DEFAULT_K," Skiped "); return;



struct func {
  char *name;
  void (*run)(void);
  struct func *next;
};


extern bool is_parallel_nb;

long int id_of_thread_executed(void);

void parse_options(int argc, char **argv);

void run_all_tests();
void execute_all(struct func *fun);
void append_func(void (*run)(void), char *name);

char* extract_func_edited_TEST_from_exec_func_name(char* func_name); /* TEST_funcname___NUM -> TEST(funcname) */

/*
void run_some_tests(size_t cnt, ... );
void run_all_tests_exept(size_t cnt, ... );
void run_some_tests_ordered(size_t cnt, ... );
*/

void run_all_tests_parallel(size_t parallel /*, int max_col*/);

/* 
 * to launch test with different parameters without re-compile it  
 * it can print help if need!
 * */
void run_all_tests_args(int argc, char **argv);

bool expected_true_f(bool val);
bool expected_false_f(bool val);

bool expected_true_f_name(bool val, const char *name);
bool expected_false_f_name(bool val, const char *name);

#define GEN_EXPECTED_OP_TYPE_FUNC(OP,type)\
  bool expected_##OP##_##type(type var1, type var2);\
  bool expected_##OP##_name_##type(type var1, type var2, const char *name); 


/*
 * ***** generate signature of expected functions EQ ***********
 */ 
GEN_EXPECTED_OP_TYPE_FUNC(EQ, TYPE_CHAR)
GEN_EXPECTED_OP_TYPE_FUNC(EQ, TYPE_U_CHAR)
GEN_EXPECTED_OP_TYPE_FUNC(EQ, TYPE_INT)
GEN_EXPECTED_OP_TYPE_FUNC(EQ, TYPE_U_INT)
GEN_EXPECTED_OP_TYPE_FUNC(EQ, TYPE_L_INT)
GEN_EXPECTED_OP_TYPE_FUNC(EQ, TYPE_U_L_INT)
GEN_EXPECTED_OP_TYPE_FUNC(EQ, TYPE_SIZE_T)
GEN_EXPECTED_OP_TYPE_FUNC(EQ, TYPE_FLOAT)
GEN_EXPECTED_OP_TYPE_FUNC(EQ, TYPE_DOUBLE)
GEN_EXPECTED_OP_TYPE_FUNC(EQ, TYPE_L_DOUBLE)
GEN_EXPECTED_OP_TYPE_FUNC(EQ, TYPE_STRING)
/* 
 * ********************  end EQ generation ************************
 */ 


/*
 * ***** generate signature of expected functions LT ***********
 */ 
GEN_EXPECTED_OP_TYPE_FUNC(LT, TYPE_CHAR)
GEN_EXPECTED_OP_TYPE_FUNC(LT, TYPE_U_CHAR)
GEN_EXPECTED_OP_TYPE_FUNC(LT, TYPE_INT)
GEN_EXPECTED_OP_TYPE_FUNC(LT, TYPE_U_INT)
GEN_EXPECTED_OP_TYPE_FUNC(LT, TYPE_L_INT)
GEN_EXPECTED_OP_TYPE_FUNC(LT, TYPE_U_L_INT)
GEN_EXPECTED_OP_TYPE_FUNC(LT, TYPE_SIZE_T)
GEN_EXPECTED_OP_TYPE_FUNC(LT, TYPE_FLOAT)
GEN_EXPECTED_OP_TYPE_FUNC(LT, TYPE_DOUBLE)
GEN_EXPECTED_OP_TYPE_FUNC(LT, TYPE_L_DOUBLE)
GEN_EXPECTED_OP_TYPE_FUNC(LT, TYPE_STRING)
/* 
 * ********************  end LT generation ************************
 */ 


/*
 * ***** generate signature of expected functions GT ***********
 */ 
GEN_EXPECTED_OP_TYPE_FUNC(GT, TYPE_CHAR)
GEN_EXPECTED_OP_TYPE_FUNC(GT, TYPE_U_CHAR)
GEN_EXPECTED_OP_TYPE_FUNC(GT, TYPE_INT)
GEN_EXPECTED_OP_TYPE_FUNC(GT, TYPE_U_INT)
GEN_EXPECTED_OP_TYPE_FUNC(GT, TYPE_L_INT)
GEN_EXPECTED_OP_TYPE_FUNC(GT, TYPE_U_L_INT)
GEN_EXPECTED_OP_TYPE_FUNC(GT, TYPE_SIZE_T)
GEN_EXPECTED_OP_TYPE_FUNC(GT, TYPE_FLOAT)
GEN_EXPECTED_OP_TYPE_FUNC(GT, TYPE_DOUBLE)
GEN_EXPECTED_OP_TYPE_FUNC(GT, TYPE_L_DOUBLE)
GEN_EXPECTED_OP_TYPE_FUNC(GT, TYPE_STRING)
/* 
 * ********************  end GT generation ************************
 */ 


/*
 * ***** generate signature of expected functions LE ***********
 */ 
GEN_EXPECTED_OP_TYPE_FUNC(LE, TYPE_CHAR)
GEN_EXPECTED_OP_TYPE_FUNC(LE, TYPE_U_CHAR)
GEN_EXPECTED_OP_TYPE_FUNC(LE, TYPE_INT)
GEN_EXPECTED_OP_TYPE_FUNC(LE, TYPE_U_INT)
GEN_EXPECTED_OP_TYPE_FUNC(LE, TYPE_L_INT)
GEN_EXPECTED_OP_TYPE_FUNC(LE, TYPE_U_L_INT)
GEN_EXPECTED_OP_TYPE_FUNC(LE, TYPE_SIZE_T)
GEN_EXPECTED_OP_TYPE_FUNC(LE, TYPE_FLOAT)
GEN_EXPECTED_OP_TYPE_FUNC(LE, TYPE_DOUBLE)
GEN_EXPECTED_OP_TYPE_FUNC(LE, TYPE_L_DOUBLE)
GEN_EXPECTED_OP_TYPE_FUNC(LE, TYPE_STRING)
/* 
 * ********************  end LE generation ************************
 */ 


/*
 * ***** generate signature of expected functions GE ***********
 */ 
GEN_EXPECTED_OP_TYPE_FUNC(GE, TYPE_CHAR)
GEN_EXPECTED_OP_TYPE_FUNC(GE, TYPE_U_CHAR)
GEN_EXPECTED_OP_TYPE_FUNC(GE, TYPE_INT)
GEN_EXPECTED_OP_TYPE_FUNC(GE, TYPE_U_INT)
GEN_EXPECTED_OP_TYPE_FUNC(GE, TYPE_L_INT)
GEN_EXPECTED_OP_TYPE_FUNC(GE, TYPE_U_L_INT)
GEN_EXPECTED_OP_TYPE_FUNC(GE, TYPE_SIZE_T)
GEN_EXPECTED_OP_TYPE_FUNC(GE, TYPE_FLOAT)
GEN_EXPECTED_OP_TYPE_FUNC(GE, TYPE_DOUBLE)
GEN_EXPECTED_OP_TYPE_FUNC(GE, TYPE_L_DOUBLE)
GEN_EXPECTED_OP_TYPE_FUNC(GE, TYPE_STRING)
/* 
 * ********************  end GE generation ************************
 */ 


/*
 * ***** generate signature of expected functions NE ***********
 */ 
GEN_EXPECTED_OP_TYPE_FUNC(NE, TYPE_CHAR)
GEN_EXPECTED_OP_TYPE_FUNC(NE, TYPE_U_CHAR)
GEN_EXPECTED_OP_TYPE_FUNC(NE, TYPE_INT)
GEN_EXPECTED_OP_TYPE_FUNC(NE, TYPE_U_INT)
GEN_EXPECTED_OP_TYPE_FUNC(NE, TYPE_L_INT)
GEN_EXPECTED_OP_TYPE_FUNC(NE, TYPE_U_L_INT)
GEN_EXPECTED_OP_TYPE_FUNC(NE, TYPE_SIZE_T)
GEN_EXPECTED_OP_TYPE_FUNC(NE, TYPE_FLOAT)
GEN_EXPECTED_OP_TYPE_FUNC(NE, TYPE_DOUBLE)
GEN_EXPECTED_OP_TYPE_FUNC(NE, TYPE_L_DOUBLE)
GEN_EXPECTED_OP_TYPE_FUNC(NE, TYPE_STRING)
/* 
 * ********************  end NE generation ************************
 */

/*
 * only expect
 */
#define HANDLE_OP_EXPECT_NAME(OP,type,var1,var2,name_f,msg_call)                                                          \
do{      \
   if(is_parallel_nb == 0){\
      if(expected_##OP##_##type(var1, var2)){                                                                       \
        PRINT_HK_C(GREEN_K,HK_TR," 1 %s passed from %s \n\n",name_f,msg_call);                                       \
      }                                                                                                         \
      else{                                                                                                     \
        /*PRINT_LOC("Failure\nExpected %s of these values:\n   %s\n\tWhich is:  %s\n %s\n\tWhich is: %s\n\n"\
          ,DESCRIPTION_##OP,#var1, type##_TO_STR(var1),  #var2, type##_TO_STR(var2));  */                                          \
        PRINT_LOC("Failure\nExpected: (%s) %s (%s) :\n Value of %s: %s \n Value of %s: %s\n\n"\
          ,#var1,STRFY(OP),#var2,#var1, type##_TO_STR(var1),  #var2, type##_TO_STR(var2));                                            \
        PRINT_HK_C(RED_K,HK_TR," 1 %s failed from %s \n",name_f,msg_call);                                           \
      }                                                                                                         \
   }else {                                                                                                         \
      if(expected_##OP##_name_##type(var1, var2, name_f)){                                                                       \
        PRINT_HK_C(GREEN_K,HK_TR," 1 %s passed from %s \n\n",name_f,msg_call);                                       \
        /*PRINT_HK_C(GREEN_K,HK_TR," 1 test passed from %s \n\n",name_f);*/                                       \
      }                                                                                                         \
      else{                                                                                                     \
        /*PRINT_LOC("Failure\nExpected %s of these values:\n   %s\n\tWhich is:  %s\n %s\n\tWhich is: %s\n\n"\
         ,DESCRIPTION_##OP ,#var1, type##_TO_STR(var1),  #var2, type##_TO_STR(var2));*/                                            \
        PRINT_LOC("Failure\nExpected: (%s) %s (%s) :\n Value of %s: %s \n Value of %s: %s\n\n"\
          ,#var1,STRFY(OP),#var2,#var1, type##_TO_STR(var1),  #var2, type##_TO_STR(var2));                                            \
        PRINT_HK_C(RED_K,HK_TR," 1 %s failed from %s \n",name_f,msg_call);                                           \
      }                                                                                                         \
    }\
}while(0);

//#define EXPECT_OP_(OP,type,var1,var2)  HANDLE_OP_EXPECT_(OP,type,var1,var2)
  
  /**
   * is_assert : 0 for EXPECT and 1 for ASSERT 
   */
#define HANDLE_OP_EXPECT_ASSERT(OP,type,var1,var2,is_assert)                                                          \
do{      \
   if(is_parallel_nb == 0){\
      if(expected_##OP##_##type(var1, var2)){                                                                       \
        PRINT_HK_C(GREEN_K,HK_TR," 1 test passed from %s \n\n",__func__);                                       \
      }                                                                                                         \
      else{                                                                                                     \
        /*PRINT_LOC("Failure\nExpected %s of these values:\n   %s\n\tWhich is:  %s\n %s\n\tWhich is: %s\n\n"\
          ,DESCRIPTION_##OP,#var1, type##_TO_STR(var1),  #var2, type##_TO_STR(var2));  */                                          \
        PRINT_LOC("Failure\nExpected: (%s) %s (%s) :\n Value of %s: %s \n Value of %s: %s\n\n"\
          ,#var1,STRFY(OP),#var2,#var1, type##_TO_STR(var1),  #var2, type##_TO_STR(var2));                                            \
        PRINT_HK_C(RED_K,HK_TR," 1 test failed from %s \n",__func__);                                           \
        if(is_assert) return;                                                                                   \
      }                                                                                                         \
   }else {                                                                                                         \
      if(expected_##OP##_name_##type(var1, var2, __func__)){                                                                       \
        PRINT_HK_C(GREEN_K,HK_TR," 1 test passed from %s \n\n",__func__);                                       \
      }                                                                                                         \
      else{                                                                                                     \
        /*PRINT_LOC("Failure\nExpected %s of these values:\n   %s\n\tWhich is:  %s\n %s\n\tWhich is: %s\n\n"\
         ,DESCRIPTION_##OP ,#var1, type##_TO_STR(var1),  #var2, type##_TO_STR(var2));*/                                            \
        PRINT_LOC("Failure\nExpected: (%s) %s (%s) :\n Value of %s: %s \n Value of %s: %s\n\n"\
          ,#var1,STRFY(OP),#var2,#var1, type##_TO_STR(var1),  #var2, type##_TO_STR(var2));                                            \
        PRINT_HK_C(RED_K,HK_TR," 1 test failed from %s \n",__func__);                                           \
        if(is_assert) return;                                                                                   \
      }                                                                                                         \
    }\
}while(0);

// *********************** begin EQ  ************************ 
// ============== EXPECT ==============================

#define EXPECT_EQ_TYPE_CHAR(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(EQ, TYPE_CHAR,var1, var2, 0)
#define EXPECT_EQ_TYPE_U_CHAR(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(EQ, TYPE_U_CHAR,var1, var2, 0)
#define EXPECT_EQ_TYPE_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(EQ, TYPE_INT,var1, var2, 0)
#define EXPECT_EQ_TYPE_U_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(EQ, TYPE_U_INT,var1, var2, 0)
#define EXPECT_EQ_TYPE_L_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(EQ, TYPE_L_INT,var1, var2, 0)
#define EXPECT_EQ_TYPE_U_L_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(EQ, TYPE_U_L_INT,var1, var2, 0)
#define EXPECT_EQ_TYPE_SIZE_T(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(EQ, TYPE_SIZE_T,var1, var2, 0)
#define EXPECT_EQ_TYPE_FLOAT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(EQ, TYPE_FLOAT,var1, var2, 0)
#define EXPECT_EQ_TYPE_DOUBLE(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(EQ, TYPE_DOUBLE,var1, var2, 0)
#define EXPECT_EQ_TYPE_L_DOUBLE(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(EQ, TYPE_L_DOUBLE,var1, var2, 0)
#define EXPECT_EQ_TYPE_STRING(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(EQ, TYPE_STRING,var1, var2, 0)

#define EXPECT_EQ(var1, var2) HANDLE_OP_EXPECT_ASSERT(EQ, TYPE_L_INT,var1, var2, 0)



// ============== ASERT =====================

#define ASSERT_EQ_TYPE_CHAR(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(EQ, TYPE_CHAR,var1, var2, 1)
#define ASSERT_EQ_TYPE_U_CHAR(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(EQ, TYPE_U_CHAR,var1, var2, 1)
#define ASSERT_EQ_TYPE_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(EQ, TYPE_INT,var1, var2, 1)
#define ASSERT_EQ_TYPE_U_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(EQ, TYPE_U_INT,var1, var2, 1)
#define ASSERT_EQ_TYPE_L_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(EQ, TYPE_L_INT,var1, var2, 1)
#define ASSERT_EQ_TYPE_U_L_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(EQ, TYPE_U_L_INT,var1, var2, 1)
#define ASSERT_EQ_TYPE_SIZE_T(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(EQ, TYPE_SIZE_T,var1, var2, 1)
#define ASSERT_EQ_TYPE_FLOAT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(EQ, TYPE_FLOAT,var1, var2, 1)
#define ASSERT_EQ_TYPE_DOUBLE(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(EQ, TYPE_DOUBLE,var1, var2, 1)
#define ASSERT_EQ_TYPE_L_DOUBLE(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(EQ, TYPE_L_DOUBLE,var1, var2, 1)
#define ASSERT_EQ_TYPE_STRING(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(EQ, TYPE_STRING,var1, var2, 1)

#define ASSERT_EQ(var1, var2)  HANDLE_OP_EXPECT_ASSERT(EQ, TYPE_L_INT,var1, var2, 1)

// ************************ end EQ  **********************


// *********************** begin LT  ************************ 
// ============== EXPECT ==============================

#define EXPECT_LT_TYPE_CHAR(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LT, TYPE_CHAR,var1, var2, 0)
#define EXPECT_LT_TYPE_U_CHAR(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LT, TYPE_U_CHAR,var1, var2, 0)
#define EXPECT_LT_TYPE_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LT, TYPE_INT,var1, var2, 0)
#define EXPECT_LT_TYPE_U_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LT, TYPE_U_INT,var1, var2, 0)
#define EXPECT_LT_TYPE_L_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LT, TYPE_L_INT,var1, var2, 0)
#define EXPECT_LT_TYPE_U_L_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LT, TYPE_U_L_INT,var1, var2, 0)
#define EXPECT_LT_TYPE_SIZE_T(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LT, TYPE_SIZE_T,var1, var2, 0)
#define EXPECT_LT_TYPE_FLOAT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LT, TYPE_FLOAT,var1, var2, 0)
#define EXPECT_LT_TYPE_DOUBLE(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LT, TYPE_DOUBLE,var1, var2, 0)
#define EXPECT_LT_TYPE_L_DOUBLE(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LT, TYPE_L_DOUBLE,var1, var2, 0)
#define EXPECT_LT_TYPE_STRING(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LT, TYPE_STRING,var1, var2, 0)

#define EXPECT_LT(var1, var2) HANDLE_OP_EXPECT_ASSERT(LT, TYPE_L_INT,var1, var2, 0)


// ============== ASERT =====================

#define ASSERT_LT_TYPE_CHAR(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LT, TYPE_CHAR,var1, var2, 1)
#define ASSERT_LT_TYPE_U_CHAR(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LT, TYPE_U_CHAR,var1, var2, 1)
#define ASSERT_LT_TYPE_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LT, TYPE_INT,var1, var2, 1)
#define ASSERT_LT_TYPE_U_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LT, TYPE_U_INT,var1, var2, 1)
#define ASSERT_LT_TYPE_L_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LT, TYPE_L_INT,var1, var2, 1)
#define ASSERT_LT_TYPE_U_L_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LT, TYPE_U_L_INT,var1, var2, 1)
#define ASSERT_LT_TYPE_SIZE_T(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LT, TYPE_SIZE_T,var1, var2, 1)
#define ASSERT_LT_TYPE_FLOAT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LT, TYPE_FLOAT,var1, var2, 1)
#define ASSERT_LT_TYPE_DOUBLE(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LT, TYPE_DOUBLE,var1, var2, 1)
#define ASSERT_LT_TYPE_L_DOUBLE(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LT, TYPE_L_DOUBLE,var1, var2, 1)
#define ASSERT_LT_TYPE_STRING(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LT, TYPE_STRING,var1, var2, 1)

#define ASSERT_LT(var1, var2)  HANDLE_OP_EXPECT_ASSERT(LT, TYPE_L_INT,var1, var2, 1)

// ************************ end LT  **********************


// *********************** begin GT  ************************ 
// ============== EXPECT ==============================

#define EXPECT_GT_TYPE_CHAR(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GT, TYPE_CHAR,var1, var2, 0)
#define EXPECT_GT_TYPE_U_CHAR(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GT, TYPE_U_CHAR,var1, var2, 0)
#define EXPECT_GT_TYPE_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GT, TYPE_INT,var1, var2, 0)
#define EXPECT_GT_TYPE_U_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GT, TYPE_U_INT,var1, var2, 0)
#define EXPECT_GT_TYPE_L_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GT, TYPE_L_INT,var1, var2, 0)
#define EXPECT_GT_TYPE_U_L_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GT, TYPE_U_L_INT,var1, var2, 0)
#define EXPECT_GT_TYPE_SIZE_T(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GT, TYPE_SIZE_T,var1, var2, 0)
#define EXPECT_GT_TYPE_FLOAT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GT, TYPE_FLOAT,var1, var2, 0)
#define EXPECT_GT_TYPE_DOUBLE(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GT, TYPE_DOUBLE,var1, var2, 0)
#define EXPECT_GT_TYPE_L_DOUBLE(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GT, TYPE_L_DOUBLE,var1, var2, 0)
#define EXPECT_GT_TYPE_STRING(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GT, TYPE_STRING,var1, var2, 0)

#define EXPECT_GT(var1, var2) HANDLE_OP_EXPECT_ASSERT(GT, TYPE_L_INT,var1, var2, 0)



// ============== ASERT =====================

#define ASSERT_GT_TYPE_CHAR(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GT, TYPE_CHAR,var1, var2, 1)
#define ASSERT_GT_TYPE_U_CHAR(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GT, TYPE_U_CHAR,var1, var2, 1)
#define ASSERT_GT_TYPE_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GT, TYPE_INT,var1, var2, 1)
#define ASSERT_GT_TYPE_U_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GT, TYPE_U_INT,var1, var2, 1)
#define ASSERT_GT_TYPE_L_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GT, TYPE_L_INT,var1, var2, 1)
#define ASSERT_GT_TYPE_U_L_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GT, TYPE_U_L_INT,var1, var2, 1)
#define ASSERT_GT_TYPE_SIZE_T(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GT, TYPE_SIZE_T,var1, var2, 1)
#define ASSERT_GT_TYPE_FLOAT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GT, TYPE_FLOAT,var1, var2, 1)
#define ASSERT_GT_TYPE_DOUBLE(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GT, TYPE_DOUBLE,var1, var2, 1)
#define ASSERT_GT_TYPE_L_DOUBLE(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GT, TYPE_L_DOUBLE,var1, var2, 1)
#define ASSERT_GT_TYPE_STRING(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GT, TYPE_STRING,var1, var2, 1)

#define ASSERT_GT(var1, var2)  HANDLE_OP_EXPECT_ASSERT(GT, TYPE_L_INT,var1, var2, 1)

// ************************ end GT  **********************


// *********************** begin LE  ************************ 
// ============== EXPECT ==============================

#define EXPECT_LE_TYPE_CHAR(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LE, TYPE_CHAR,var1, var2, 0)
#define EXPECT_LE_TYPE_U_CHAR(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LE, TYPE_U_CHAR,var1, var2, 0)
#define EXPECT_LE_TYPE_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LE, TYPE_INT,var1, var2, 0)
#define EXPECT_LE_TYPE_U_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LE, TYPE_U_INT,var1, var2, 0)
#define EXPECT_LE_TYPE_L_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LE, TYPE_L_INT,var1, var2, 0)
#define EXPECT_LE_TYPE_U_L_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LE, TYPE_U_L_INT,var1, var2, 0)
#define EXPECT_LE_TYPE_SIZE_T(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LE, TYPE_SIZE_T,var1, var2, 0)
#define EXPECT_LE_TYPE_FLOAT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LE, TYPE_FLOAT,var1, var2, 0)
#define EXPECT_LE_TYPE_DOUBLE(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LE, TYPE_DOUBLE,var1, var2, 0)
#define EXPECT_LE_TYPE_L_DOUBLE(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LE, TYPE_L_DOUBLE,var1, var2, 0)
#define EXPECT_LE_TYPE_STRING(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LE, TYPE_STRING,var1, var2, 0)

#define EXPECT_LE(var1, var2) HANDLE_OP_EXPECT_ASSERT(LE, TYPE_L_INT,var1, var2, 0)



// ============== ASERT =====================

#define ASSERT_LE_TYPE_CHAR(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LE, TYPE_CHAR,var1, var2, 1)
#define ASSERT_LE_TYPE_U_CHAR(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LE, TYPE_U_CHAR,var1, var2, 1)
#define ASSERT_LE_TYPE_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LE, TYPE_INT,var1, var2, 1)
#define ASSERT_LE_TYPE_U_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LE, TYPE_U_INT,var1, var2, 1)
#define ASSERT_LE_TYPE_L_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LE, TYPE_L_INT,var1, var2, 1)
#define ASSERT_LE_TYPE_U_L_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LE, TYPE_U_L_INT,var1, var2, 1)
#define ASSERT_LE_TYPE_SIZE_T(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LE, TYPE_SIZE_T,var1, var2, 1)
#define ASSERT_LE_TYPE_FLOAT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LE, TYPE_FLOAT,var1, var2, 1)
#define ASSERT_LE_TYPE_DOUBLE(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LE, TYPE_DOUBLE,var1, var2, 1)
#define ASSERT_LE_TYPE_L_DOUBLE(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LE, TYPE_L_DOUBLE,var1, var2, 1)
#define ASSERT_LE_TYPE_STRING(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(LE, TYPE_STRING,var1, var2, 1)

#define ASSERT_LE(var1, var2)  HANDLE_OP_EXPECT_ASSERT(LE, TYPE_L_INT,var1, var2, 1)

// ************************ end LE  **********************


// *********************** begin GE  ************************ 
// ============== EXPECT ==============================

#define EXPECT_GE_TYPE_CHAR(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GE, TYPE_CHAR,var1, var2, 0)
#define EXPECT_GE_TYPE_U_CHAR(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GE, TYPE_U_CHAR,var1, var2, 0)
#define EXPECT_GE_TYPE_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GE, TYPE_INT,var1, var2, 0)
#define EXPECT_GE_TYPE_U_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GE, TYPE_U_INT,var1, var2, 0)
#define EXPECT_GE_TYPE_L_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GE, TYPE_L_INT,var1, var2, 0)
#define EXPECT_GE_TYPE_U_L_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GE, TYPE_U_L_INT,var1, var2, 0)
#define EXPECT_GE_TYPE_SIZE_T(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GE, TYPE_SIZE_T,var1, var2, 0)
#define EXPECT_GE_TYPE_FLOAT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GE, TYPE_FLOAT,var1, var2, 0)
#define EXPECT_GE_TYPE_DOUBLE(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GE, TYPE_DOUBLE,var1, var2, 0)
#define EXPECT_GE_TYPE_L_DOUBLE(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GE, TYPE_L_DOUBLE,var1, var2, 0)
#define EXPECT_GE_TYPE_STRING(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GE, TYPE_STRING,var1, var2, 0)

#define EXPECT_GE(var1, var2) HANDLE_OP_EXPECT_ASSERT(GE, TYPE_L_INT,var1, var2, 0)



// ============== ASERT =====================

#define ASSERT_GE_TYPE_CHAR(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GE, TYPE_CHAR,var1, var2, 1)
#define ASSERT_GE_TYPE_U_CHAR(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GE, TYPE_U_CHAR,var1, var2, 1)
#define ASSERT_GE_TYPE_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GE, TYPE_INT,var1, var2, 1)
#define ASSERT_GE_TYPE_U_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GE, TYPE_U_INT,var1, var2, 1)
#define ASSERT_GE_TYPE_L_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GE, TYPE_L_INT,var1, var2, 1)
#define ASSERT_GE_TYPE_U_L_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GE, TYPE_U_L_INT,var1, var2, 1)
#define ASSERT_GE_TYPE_SIZE_T(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GE, TYPE_SIZE_T,var1, var2, 1)
#define ASSERT_GE_TYPE_FLOAT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GE, TYPE_FLOAT,var1, var2, 1)
#define ASSERT_GE_TYPE_DOUBLE(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GE, TYPE_DOUBLE,var1, var2, 1)
#define ASSERT_GE_TYPE_L_DOUBLE(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GE, TYPE_L_DOUBLE,var1, var2, 1)
#define ASSERT_GE_TYPE_STRING(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(GE, TYPE_STRING,var1, var2, 1)

#define ASSERT_GE(var1, var2)  HANDLE_OP_EXPECT_ASSERT(GE, TYPE_L_INT,var1, var2, 1)

// ************************ end GE  **********************


// *********************** begin NE  ************************ 
// ============== EXPECT ==============================

#define EXPECT_NE_TYPE_CHAR(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(NE, TYPE_CHAR,var1, var2, 0)
#define EXPECT_NE_TYPE_U_CHAR(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(NE, TYPE_U_CHAR,var1, var2, 0)
#define EXPECT_NE_TYPE_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(NE, TYPE_INT,var1, var2, 0)
#define EXPECT_NE_TYPE_U_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(NE, TYPE_U_INT,var1, var2, 0)
#define EXPECT_NE_TYPE_L_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(NE, TYPE_L_INT,var1, var2, 0)
#define EXPECT_NE_TYPE_U_L_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(NE, TYPE_U_L_INT,var1, var2, 0)
#define EXPECT_NE_TYPE_SIZE_T(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(NE, TYPE_SIZE_T,var1, var2, 0)
#define EXPECT_NE_TYPE_FLOAT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(NE, TYPE_FLOAT,var1, var2, 0)
#define EXPECT_NE_TYPE_DOUBLE(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(NE, TYPE_DOUBLE,var1, var2, 0)
#define EXPECT_NE_TYPE_L_DOUBLE(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(NE, TYPE_L_DOUBLE,var1, var2, 0)
#define EXPECT_NE_TYPE_STRING(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(NE, TYPE_STRING,var1, var2, 0)

#define EXPECT_NE(var1, var2) HANDLE_OP_EXPECT_ASSERT(NE, TYPE_L_INT,var1, var2, 0)



// ============== ASERT =====================

#define ASSERT_NE_TYPE_CHAR(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(NE, TYPE_CHAR,var1, var2, 1)
#define ASSERT_NE_TYPE_U_CHAR(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(NE, TYPE_U_CHAR,var1, var2, 1)
#define ASSERT_NE_TYPE_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(NE, TYPE_INT,var1, var2, 1)
#define ASSERT_NE_TYPE_U_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(NE, TYPE_U_INT,var1, var2, 1)
#define ASSERT_NE_TYPE_L_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(NE, TYPE_L_INT,var1, var2, 1)
#define ASSERT_NE_TYPE_U_L_INT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(NE, TYPE_U_L_INT,var1, var2, 1)
#define ASSERT_NE_TYPE_SIZE_T(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(NE, TYPE_SIZE_T,var1, var2, 1)
#define ASSERT_NE_TYPE_FLOAT(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(NE, TYPE_FLOAT,var1, var2, 1)
#define ASSERT_NE_TYPE_DOUBLE(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(NE, TYPE_DOUBLE,var1, var2, 1)
#define ASSERT_NE_TYPE_L_DOUBLE(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(NE, TYPE_L_DOUBLE,var1, var2, 1)
#define ASSERT_NE_TYPE_STRING(var1, var2) 	HANDLE_OP_EXPECT_ASSERT(NE, TYPE_STRING,var1, var2, 1)

#define ASSERT_NE(var1, var2)  HANDLE_OP_EXPECT_ASSERT(NE, TYPE_L_INT,var1, var2, 1)

// ************************ end NE  **********************

/*
 * ==============  bool ===================
 *
 */

#define HANDLE_EXPECT_NOT_EXPECT_ASSERT(expect,not_expect,var1,is_assert)                             \
do{                                                                                               \
   if(is_parallel_nb==0){\
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

//#define test_label test

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
    append_func(CONCAT(TEST_##name_f##____,count),STRFY (TEST(name_f): test N° count| ) );          \
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
