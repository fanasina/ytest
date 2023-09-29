#ifndef __MOCK_C_H__
#define __MOCK_C_H__

#include "ftest/ftest.h"
#include "tools_t/tools_t.h"

#define INFINITY -8
#define INITSTATE -1
#define DONOTHING 0

struct func_mock_info_struct{
  long id;
  char *str_namefunc;
  char *str_conditions;
  char *str_caller;
  int expect_call;/* 1 if EXPECT_MOCK_CALL and 0 if WILL_MOCK_CALL */
  long call;/* increment when call (try to executed) and 0 if not : init value */
  long failed_call;/* increment when condition not fill and 0 if not : init value */
  long init_times_left;/* DONOTHING do nothing (pass to -> next), INFINITY every time; INITSTATE init; > 0 execute and decrement  */
  long times_left;/* DONOTHING do nothing (pass to -> next), INFINITY every time; INITSTATE init; > 0 execute and decrement  */
  struct func_mock_info_struct *next;
};

struct list_base_fmock{
  struct func_mock_info_struct *info_mock;
  struct list_base_fmock *next;
};


int parse_count_args_(char *input);
void append_fmock_to_listmock(struct func_mock_info_struct **f_mock_list,  struct func_mock_info_struct *f_mock);
void append_list_base_fmock(struct list_base_fmock **l_fmock, struct func_mock_info_struct *f_mock);

extern struct func_mock_info_struct *f_mock_glist;
extern struct list_base_fmock *g_list_base_fmock;

#if 0
    int expect_call; /* 1 if EXPECT_MOCK_CALL and 0 if WILL_MOCK_CALL */\
    long init_times_left; /* DONOTHING do nothing (pass to -> next), INFINITY every time; INITSTATE init; > 0 execute and decrement  */\
    long times_left; /* DONOTHING do nothing (pass to -> next), INFINITY every time; INITSTATE init; > 0 execute and decrement  */\

#endif


#define MOCK_FUNC(returntype, namefunction, args_prototype_with_parenthesis, args_call_with_parenthesis)\
  /*typedef returntype FUNC_type_ ## namefunction args_prototype_with_parenthesis ;*/\
  /*typedef args_prototype_with_parenthesis args_ ## namefunction;*/\
  struct list_mock_return_ ## namefunction{\
    returntype (*run) args_prototype_with_parenthesis;\
    int (*call_mock_condition) args_prototype_with_parenthesis ;/* to store condition */\
    /*int arg_count;*/\
    struct func_mock_info_struct *info_mock;\
    struct list_mock_return_ ## namefunction *next;\
  } list_mo_ ## namefunction;\
  __attribute__((constructor)) void init_list_m_ ## namefunction(void){\
    list_mo_ ## namefunction.info_mock = malloc(sizeof(struct func_mock_info_struct));\
    (list_mo_ ## namefunction.info_mock)->times_left = INITSTATE;\
    (list_mo_ ## namefunction.info_mock)->init_times_left = INITSTATE;\
    /*list_mo_ ## namefunction.arg_count = parse_count_args_(#args_prototype_with_parenthesis);\
    list_mo_ ## namefunction.call_mock_condition = malloc(list_mo_ ## namefunction.arg_count * sizeof( int (*)(void*))) ;*/\
    list_mo_ ## namefunction.next = NULL;\
  }\
  returntype namefunction args_prototype_with_parenthesis {\
    /*static size_t count_call_f=0;\
    PRINT_DEBUG(">>>>>>count call of %s: %ld\n",STRFY(namefunction),++count_call_f);*/\
    struct list_mock_return_ ## namefunction *tmp_mock = &list_mo_ ## namefunction;\
    if( (tmp_mock->info_mock)->times_left == INITSTATE ){\
        PRINT_HK_C(YELLOW_K,HK_TR," WARNING, no EXPECT or WILL CALL defined for the mock function %s.\n",#namefunction);\
        PRINT_HK_C(YELLOW_K,HK_TR," Can be defined by EXPECT_MOCK_CALL(%s,%s,%s,true,1) if call once and accept all args, the same args with WILL_MOCK_CALL \n",STRFY (returntype), STRFY(namefunction),STRFY (args_prototype_with_parenthesis) ); \
        return (returntype)0; \
    }\
    while(tmp_mock->next && (tmp_mock->info_mock)->times_left == 0) tmp_mock = tmp_mock->next ;\
    ++((tmp_mock->info_mock)->call);\
    /*LOG("condition_func:%d\n", tmp_mock->call_mock_condition args_call_with_parenthesis);*/ /*LOG("%s\n","failure condition");*/\
    /*EXPECT_EQ_TYPE_INT(1, tmp_mock->call_mock_condition args_call_with_parenthesis);*/ /*LOG("%s\n","failure condition");*/\
    if ((tmp_mock->info_mock)->times_left == 0) /*no longer response, default return */ \
      return (returntype)0;/* default return */\
    if( (tmp_mock->info_mock)->str_caller == NULL){ \
      PRINT_HK_C(YELLOW_K,HK_TR," WARNING, MOCK need to be initialized in TEST env call by INIT_CALLER_MOCK(%s) if need to have stats\n",(tmp_mock->info_mock)->str_namefunc); \
      /*return (returntype)0;*/ \
    }\
    else{\
      size_t len0 = strlen((tmp_mock->info_mock)->str_conditions);\
      size_t len1 = strlen("call   check condition: aa");\
      char *msg_call=malloc(len0 + len1);\
      sprintf(msg_call,"call check condition: %s",(tmp_mock->info_mock)->str_conditions);\
      HANDLE_OP_EXPECT_NAME(EQ,TYPE_INT,1, tmp_mock->call_mock_condition args_call_with_parenthesis, (tmp_mock->info_mock)->str_caller, msg_call); /*LOG("%s\n","failure condition");*/\
      free(msg_call);\
    }\
    /*if(0 == tmp_mock->call_mock_condition args_call_with_parenthesis){\
      PRINT_LOC("Failure, arguments not expected\ncondition ( %s ) not verified\n\n", (tmp_mock->info_mock)->str_conditions);\
      PRINT_HK_C(RED_K,HK_TR," 1 argument check failed from %s \n",__func__);                                 \
    }*/\
    PRINT_DEBUG(" %*c VALUES: mock function:%s, conditions:%s t_left:%ld, init_left:%ld| args:%s\n",8,'^',(tmp_mock->info_mock)->str_namefunc, (tmp_mock->info_mock)->str_conditions, (tmp_mock->info_mock)->times_left,(tmp_mock->info_mock)->init_times_left, STRFY(args_call_with_parenthesis));\
    if (((tmp_mock->info_mock)->times_left <= INFINITY) || ((tmp_mock->info_mock)->times_left > 0)){\
      --((tmp_mock->info_mock)->times_left);\
    PRINT_DEBUG(" %*c VALUES: mock function:%s, conditions:%s t_left:%ld, init_left:%ld| args:%s\n",8,'v',(tmp_mock->info_mock)->str_namefunc, (tmp_mock->info_mock)->str_conditions, (tmp_mock->info_mock)->times_left,(tmp_mock->info_mock)->init_times_left, STRFY(args_call_with_parenthesis));\
      if(1 == tmp_mock->call_mock_condition args_call_with_parenthesis){\
        return tmp_mock->run args_call_with_parenthesis;\
      }\
      else ++((tmp_mock->info_mock)->failed_call);\
    }\
    return (returntype)0;/* default return */\
  }


char* extract_name_func_mock(char *input);


#define EXPECT_EQ_IN_MOCKF(var1,var2, name_f_mocked)\
  do{ HANDLE_OP_EXPECT_NAME(EQ,TYPE_INT,var1,var2,(list_mo_ ## name_f_mocked.info_mock)->str_caller,"mock test")}while(0)
    
#if 0
      (tmp_mock->info_mock)->expect_call = f_expect_call;\
      (tmp_mock->info_mock)->init_times_left = repeat;\
      (tmp_mock->info_mock)->times_left = repeat;\
      (tmp_mock->info_mock)->next = NULL;\
      tmp_mock->call_mock_condition = CONCAT(namefunction ## _cond_, id);\
      tmp_mock->run = CONCAT(run_ ## namefunction ## _line_, id);\
 


      (tmp_mock->next)->run = CONCAT(run_ ## namefunction ## _line_, id);\
      (tmp_mock->next)->call_mock_condition = CONCAT(namefunction ## _cond_, id);\
      (tmp_mock->next)->info_mock  = malloc(sizeof(struct func_mock_info_struct));\
      ((tmp_mock->next)->info_mock)->expect_call = f_expect_call;\
      ((tmp_mock->next)->info_mock)->init_times_left = repeat;\
      ((tmp_mock->next)->info_mock)->times_left = repeat;\
      ((tmp_mock->next)->info_mock)->str_namefunc = malloc(strlen(#namefunction));\
      strcpy(((tmp_mock->next)->info_mock)->str_namefunc, #namefunction);\
      ((tmp_mock->next)->info_mock)->str_conditions = malloc(strlen(#condition_on_args_expression));\
      strcpy(((tmp_mock->next)->info_mock)->str_conditions, #condition_on_args_expression;\
      ((tmp_mock->next)->info_mock)->next = NULL;\
      (tmp_mock->next)->next = NULL;\

#endif

      /*
       *  to inject the name TEST in the mock attribute info
       */
#define INIT_CALLER_MOCK(namefunction)/* */\
  do{\
      struct list_mock_return_ ## namefunction *tmp_mock = &list_mo_ ## namefunction;\
      while(tmp_mock){\
        (tmp_mock->info_mock)->str_caller=malloc(strlen(__func__));\
        strcpy((tmp_mock->info_mock)->str_caller,__func__);\
        tmp_mock = tmp_mock->next;\
      }\
 }while(0);

#define FILL_MOCK_INFO(tmp_new_mock, namefunction, condition_on_args_expression , repeat, f_expect_call, id) \
      (tmp_new_mock)->run = CONCAT(run_ ## namefunction ## _line_, id);\
      (tmp_new_mock)->call_mock_condition = CONCAT(namefunction ## _cond_, id);\
      /*(tmp_new_mock)->info_mock  = malloc(sizeof(struct func_mock_info_struct));*/\
      ((tmp_new_mock)->info_mock)->expect_call = f_expect_call;\
      ((tmp_new_mock)->info_mock)->call = 0;\
      ((tmp_new_mock)->info_mock)->failed_call = 0;\
      ((tmp_new_mock)->info_mock)->init_times_left = repeat;\
      ((tmp_new_mock)->info_mock)->times_left = repeat;\
      ((tmp_new_mock)->info_mock)->str_namefunc = malloc(strlen(#namefunction) + 32 + strlen("_line_"));\
      sprintf(((tmp_new_mock)->info_mock)->str_namefunc,"%s_line_%d",#namefunction,id);\
      ((tmp_new_mock)->info_mock)->str_conditions = malloc(strlen(#condition_on_args_expression));\
      strcpy(((tmp_new_mock)->info_mock)->str_conditions, #condition_on_args_expression);\
      ((tmp_new_mock)->info_mock)->str_caller = NULL;\
      ((tmp_new_mock)->info_mock)->next = NULL;\
      (tmp_new_mock)->next = NULL;\
      append_fmock_to_listmock(&f_mock_glist, (tmp_new_mock)->info_mock);


#define ADD_RESPONSE(returntype, namefunction, args_prototype_with_parenthesis, condition_on_args_expression , repeat, f_expect_call, id)\
  /*FUNC_type_ ## namefunction CONCAT(run_ ## namefunction ## _line_ , id);*/\
  returntype CONCAT(run_ ## namefunction ## _line_ , id) args_prototype_with_parenthesis; \
  int CONCAT(namefunction ## _cond_ , id) args_prototype_with_parenthesis {/*LOG("cond:%d\n",condition_on_args_expression);*/ return condition_on_args_expression;}\
  __attribute__((constructor)) void CONCAT(append_list_ ## namefunction , id)(void){\
    struct list_mock_return_ ## namefunction *tmp_mock = &list_mo_ ## namefunction;\
    if((tmp_mock->info_mock)->times_left == INITSTATE){/* init state */\
      FILL_MOCK_INFO(tmp_mock, namefunction, condition_on_args_expression , repeat, f_expect_call, id);\
      append_list_base_fmock( &g_list_base_fmock ,(tmp_mock->info_mock));\
    }\
    else{\
      while(tmp_mock->next) tmp_mock = tmp_mock->next;\
      tmp_mock->next = malloc(sizeof(list_mo_ ## namefunction));\
      (tmp_mock->next)->info_mock  = malloc(sizeof(struct func_mock_info_struct));\
      FILL_MOCK_INFO(tmp_mock->next, namefunction, condition_on_args_expression , repeat, f_expect_call, id);\
      /*(tmp_mock->info_mock)->next  = (tmp_mock->next)->info_mock ;*/\
    }\
  }\
  returntype CONCAT(run_ ## namefunction ## _line_, id) args_prototype_with_parenthesis




#define EXPECT_MOCK_CALL(returntype, namefunction, args_prototype_with_parenthesis, condition_on_args_expression ,repeat) \
  ADD_RESPONSE(returntype,namefunction, args_prototype_with_parenthesis, condition_on_args_expression, repeat, 1, __LINE__)


#define WILL_MOCK_CALL(returntype, namefunction, args_prototype_with_parenthesis, condition_on_args_expression ,repeat) \
  ADD_RESPONSE(returntype,namefunction, args_prototype_with_parenthesis, condition_on_args_expression, repeat, 0, __LINE__)
  


#endif /* __MOCK_C_H__ */
