#ifndef __MOCK_C_H__
#define __MOCK_C_H__

#include "ftest/ftest.h"
#include "tools_t/tools_t.h"

#define INFINITY -8
#define INITSTATE -1
#define DONOTHING 0

int parse_count_args_(char *input);

struct func_mock_info_struct{
  long id;
  void *struct_mock;/* contain returntype, namefunction, args... */
  int expect_call;/* 1 if EXPECT_MOCK_CALL and 0 if WILL_MOCK_CALL */
  long init_times_left;/* DONOTHING do nothing (pass to -> next), INFINITY every time; INITSTATE init; > 0 execute and decrement  */
  long times_left;
  struct func_mock_struct *next;
};

#define MOCK_FUNC(returntype, namefunction, args_prototype_with_parenthesis, args_call_with_parenthesis)\
  /*typedef returntype FUNC_type_ ## namefunction args_prototype_with_parenthesis ;*/\
  /*typedef args_prototype_with_parenthesis args_ ## namefunction;*/\
  struct list_mock_return_ ## namefunction{\
    returntype (*run) args_prototype_with_parenthesis;\
    int expect_call;/* 1 if EXPECT_MOCK_CALL and 0 if WILL_MOCK_CALL */\
    int (*call_mock_condition) args_prototype_with_parenthesis ;/* to store condition */\
    /*int arg_count;*/\
    long init_times_left;/* DONOTHING do nothing (pass to -> next), INFINITY every time; INITSTATE init; > 0 execute and decrement  */\
    long times_left;/* DONOTHING do nothing (pass to -> next), INFINITY every time; INITSTATE init; > 0 execute and decrement  */\
    struct list_mock_return_ ## namefunction *next;\
  } list_mo_ ## namefunction;\
  __attribute__((constructor)) void init_list_m_ ## namefunction(void){\
    list_mo_ ## namefunction.times_left = INITSTATE;\
    /*list_mo_ ## namefunction.arg_count = parse_count_args_(#args_prototype_with_parenthesis);\
    list_mo_ ## namefunction.call_mock_condition = malloc(list_mo_ ## namefunction.arg_count * sizeof( int (*)(void*))) ;*/\
    list_mo_ ## namefunction.next = NULL;\
  }\
  returntype namefunction args_prototype_with_parenthesis {\
    struct list_mock_return_ ## namefunction *tmp_mock = &list_mo_ ## namefunction;\
    while(tmp_mock->next && tmp_mock->times_left == 0) tmp_mock = tmp_mock->next ;\
      /*LOG("condition_func:%d\n", tmp_mock->call_mock_condition args_call_with_parenthesis);*/ /*LOG("%s\n","failure condition");*/\
      EXPECT_EQ_TYPE_INT(1, tmp_mock->call_mock_condition args_call_with_parenthesis); /*LOG("%s\n","failure condition");*/\
      if ((tmp_mock->times_left <= INFINITY) || (tmp_mock->times_left > 0)){\
        --(tmp_mock->times_left);\
        if(1 == tmp_mock->call_mock_condition args_call_with_parenthesis){\
          return tmp_mock->run args_call_with_parenthesis;\
        }\
        else return (returntype)0;/* default return */\
      }\
  }


#define ADD_RESPONSE(returntype, namefunction, args_prototype_with_parenthesis, condition_on_args_expression , repeat, f_expect_call, id)\
  /*FUNC_type_ ## namefunction CONCAT(run_ ## namefunction ## _ID_ , id);*/\
  returntype CONCAT(run_ ## namefunction ## _ID_ , id) args_prototype_with_parenthesis; \
  int CONCAT(namefunction ## _cond_ , id) args_prototype_with_parenthesis {LOG("cond:%d\n",condition_on_args_expression); return condition_on_args_expression;}\
  __attribute__((constructor)) void CONCAT(append_list_ ## namefunction , id)(void){\
    struct list_mock_return_ ## namefunction *tmp_mock = &list_mo_ ## namefunction;\
    if(tmp_mock->times_left == INITSTATE){/* init state */\
      tmp_mock->expect_call = f_expect_call;\
      tmp_mock->init_times_left = repeat;\
      tmp_mock->times_left = repeat;\
      tmp_mock->call_mock_condition = CONCAT(namefunction ## _cond_, id);\
      tmp_mock->run = CONCAT(run_ ## namefunction ## _ID_, id);\
    }\
    else{\
      while(tmp_mock->next) tmp_mock = tmp_mock->next;\
      tmp_mock->next = malloc(sizeof(list_mo_ ## namefunction));\
      (tmp_mock->next)->run = CONCAT(run_ ## namefunction ## _ID_, id);\
      (tmp_mock->next)->call_mock_condition = CONCAT(namefunction ## _cond_, id);\
      (tmp_mock->next)->expect_call = f_expect_call;\
      (tmp_mock->next)->init_times_left = repeat;\
      (tmp_mock->next)->times_left = repeat;\
      (tmp_mock->next)->next = NULL;\
    }\
  }\
  returntype CONCAT(run_ ## namefunction ## _ID_, id) args_prototype_with_parenthesis




#define EXPECT_MOCK_CALL(returntype, namefunction, args_prototype_with_parenthesis, condition_on_args_expression ,repeat) \
  ADD_RESPONSE(returntype,namefunction, args_prototype_with_parenthesis, condition_on_args_expression, repeat, 1, __LINE__)


#define WILL_MOCK_CALL(returntype, namefunction, args_prototype_with_parenthesis, condition_on_args_expression ,repeat) \
  ADD_RESPONSE(returntype,namefunction, args_prototype_with_parenthesis, condition_on_args_expression, repeat, 0, __LINE__)
  


#endif /* __MOCK_C_H__ */
