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
  int expect_call;/* 1 if EXPECT_MOCK_CALL and 0 if WILL_MOCK_CALL */
  long init_times_left;/* DONOTHING do nothing (pass to -> next), INFINITY every time; INITSTATE init; > 0 execute and decrement  */
  long times_left;/* DONOTHING do nothing (pass to -> next), INFINITY every time; INITSTATE init; > 0 execute and decrement  */
  struct func_mock_info_struct *next;
};

int parse_count_args_(char *input);
void append_fmock_to_listmock(struct func_mock_info_struct **f_mock_list,  struct func_mock_info_struct *f_mock);

extern struct func_mock_info_struct *f_mock_glist;

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
    struct list_mock_return_ ## namefunction *tmp_mock = &list_mo_ ## namefunction;\
    while(tmp_mock->next && (tmp_mock->info_mock)->times_left == 0) tmp_mock = tmp_mock->next ;\
      /*LOG("condition_func:%d\n", tmp_mock->call_mock_condition args_call_with_parenthesis);*/ /*LOG("%s\n","failure condition");*/\
      /*EXPECT_EQ_TYPE_INT(1, tmp_mock->call_mock_condition args_call_with_parenthesis);*/ /*LOG("%s\n","failure condition");*/\
      if(0 == tmp_mock->call_mock_condition args_call_with_parenthesis){\
        PRINT_LOC("Failure, arguments not expected\ncondition ( %s ) not verified\n\n", (tmp_mock->info_mock)->str_conditions);\
        PRINT_HK_C(RED_K,HK_TR," 1 argument check failed from %s \n",__func__);                                 \
      }\
      if (((tmp_mock->info_mock)->times_left <= INFINITY) || ((tmp_mock->info_mock)->times_left > 0)){\
        --((tmp_mock->info_mock)->times_left);\
        if(1 == tmp_mock->call_mock_condition args_call_with_parenthesis){\
          return tmp_mock->run args_call_with_parenthesis;\
        }\
        else return (returntype)0;/* default return */\
      }\
  }

#if 0
      (tmp_mock->info_mock)->expect_call = f_expect_call;\
      (tmp_mock->info_mock)->init_times_left = repeat;\
      (tmp_mock->info_mock)->times_left = repeat;\
      (tmp_mock->info_mock)->next = NULL;\
      tmp_mock->call_mock_condition = CONCAT(namefunction ## _cond_, id);\
      tmp_mock->run = CONCAT(run_ ## namefunction ## _ID_, id);\
 


      (tmp_mock->next)->run = CONCAT(run_ ## namefunction ## _ID_, id);\
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


#define FILL_MOCK_INFO(tmp_new_mock, namefunction, condition_on_args_expression , repeat, f_expect_call, id) \
      (tmp_new_mock)->run = CONCAT(run_ ## namefunction ## _ID_, id);\
      (tmp_new_mock)->call_mock_condition = CONCAT(namefunction ## _cond_, id);\
      /*(tmp_new_mock)->info_mock  = malloc(sizeof(struct func_mock_info_struct));*/\
      ((tmp_new_mock)->info_mock)->expect_call = f_expect_call;\
      ((tmp_new_mock)->info_mock)->init_times_left = repeat;\
      ((tmp_new_mock)->info_mock)->times_left = repeat;\
      ((tmp_new_mock)->info_mock)->str_namefunc = malloc(strlen(#namefunction));\
      strcpy(((tmp_new_mock)->info_mock)->str_namefunc, #namefunction);\
      ((tmp_new_mock)->info_mock)->str_conditions = malloc(strlen(#condition_on_args_expression));\
      strcpy(((tmp_new_mock)->info_mock)->str_conditions, #condition_on_args_expression);\
      ((tmp_new_mock)->info_mock)->next = NULL;\
      (tmp_new_mock)->next = NULL;\
      append_fmock_to_listmock(&f_mock_glist, (tmp_new_mock)->info_mock);


#define ADD_RESPONSE(returntype, namefunction, args_prototype_with_parenthesis, condition_on_args_expression , repeat, f_expect_call, id)\
  /*FUNC_type_ ## namefunction CONCAT(run_ ## namefunction ## _ID_ , id);*/\
  returntype CONCAT(run_ ## namefunction ## _ID_ , id) args_prototype_with_parenthesis; \
  int CONCAT(namefunction ## _cond_ , id) args_prototype_with_parenthesis {/*LOG("cond:%d\n",condition_on_args_expression);*/ return condition_on_args_expression;}\
  __attribute__((constructor)) void CONCAT(append_list_ ## namefunction , id)(void){\
    struct list_mock_return_ ## namefunction *tmp_mock = &list_mo_ ## namefunction;\
    if((tmp_mock->info_mock)->times_left == INITSTATE){/* init state */\
      FILL_MOCK_INFO(tmp_mock, namefunction, condition_on_args_expression , repeat, f_expect_call, id);\
    }\
    else{\
      while(tmp_mock->next) tmp_mock = tmp_mock->next;\
      tmp_mock->next = malloc(sizeof(list_mo_ ## namefunction));\
      (tmp_mock->next)->info_mock  = malloc(sizeof(struct func_mock_info_struct));\
      FILL_MOCK_INFO(tmp_mock->next, namefunction, condition_on_args_expression , repeat, f_expect_call, id);\
    }\
  }\
  returntype CONCAT(run_ ## namefunction ## _ID_, id) args_prototype_with_parenthesis




#define EXPECT_MOCK_CALL(returntype, namefunction, args_prototype_with_parenthesis, condition_on_args_expression ,repeat) \
  ADD_RESPONSE(returntype,namefunction, args_prototype_with_parenthesis, condition_on_args_expression, repeat, 1, __LINE__)


#define WILL_MOCK_CALL(returntype, namefunction, args_prototype_with_parenthesis, condition_on_args_expression ,repeat) \
  ADD_RESPONSE(returntype,namefunction, args_prototype_with_parenthesis, condition_on_args_expression, repeat, 0, __LINE__)
  


#endif /* __MOCK_C_H__ */
