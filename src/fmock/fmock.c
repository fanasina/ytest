#include "fmock/fmock.h"

/*
 * return the numbers of comas (,) +1 in the input string
 */


int parse_count_args_(char *input){
  int val=0;
  for(int i = 0; i< strlen(input); ++i){
    if(input[i]==',') ++val;
  }  
  return val+1;
}

struct func_mock_info_struct *f_mock_glist;

void append_fmock_to_listmock(struct func_mock_info_struct **f_mock_list,  struct func_mock_info_struct *f_mock){
  if(*f_mock_list){
    struct func_mock_info_struct *tmp_fmock_info = *f_mock_list;
    while(tmp_fmock_info->next) tmp_fmock_info = tmp_fmock_info->next;
    tmp_fmock_info->next = f_mock;
  }
  else{
    //*f_mock_global = malloc(sizeof(struct func_mock_info_struct));
    *f_mock_list = f_mock;
  }

}


