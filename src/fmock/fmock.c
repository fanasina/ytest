#include "fmock/fmock.h"

/*
 * global variables
 */

//char *colors_f[]={DEFAULT_K, RED_K, GREEN_K, YELLOW_K, BLUE_K, ""};

size_t count_f_mock_wished=0;
pthread_mutex_t mut_count_f_mock_wished;

size_t count_f_mock=0;
pthread_mutex_t mut_count_f_mock;

size_t count_expect_mock=0;
pthread_mutex_t mut_count_expect_mock;

struct list_base_fmock *g_list_base_fmock=NULL;

#define LOCK(mutex_var)  pthread_mutex_lock(&mutex_var);
#define UNLOCK(mutex_var) pthread_mutex_unlock(&mutex_var);


#define INCREMENT_(variable)\
  do{\
      LOCK(mut_##variable); \
      ++variable;\
      UNLOCK(mut_##variable);\
  }while(0);


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

/*
 * the begin only !
 */
void append_list_base_fmock(struct list_base_fmock **l_fmock, struct func_mock_info_struct *f_mock){
  INCREMENT_(count_f_mock);
  if(*l_fmock){
    struct list_base_fmock *tmp_l_n = *l_fmock;
    while(tmp_l_n->next) tmp_l_n = tmp_l_n->next;
    (tmp_l_n->next) = malloc(sizeof(struct list_base_fmock));
    (tmp_l_n->next)->info_mock = f_mock;
  }
  else{
    *l_fmock = malloc(sizeof(struct list_base_fmock)); 
    (*l_fmock)->info_mock = f_mock;
  }
}

struct func_mock_info_struct *f_mock_glist;
pthread_mutex_t mut_f_mock_glist;

void append_fmock_to_listmock(struct func_mock_info_struct **f_mock_list,  struct func_mock_info_struct *f_mock){
  INCREMENT_(count_f_mock_wished);
  if(f_mock->expect_call) {
    INCREMENT_(count_expect_mock);
  }
  LOCK(mut_f_mock_glist);
  if(*f_mock_list){
    struct func_mock_info_struct *tmp_fmock_info = *f_mock_list;
    while(tmp_fmock_info->next) tmp_fmock_info = tmp_fmock_info->next;
    tmp_fmock_info->next = f_mock;
  }
  else{
    *f_mock_list = f_mock;
  }
  UNLOCK(mut_f_mock_glist);

}

char* extract_name_func_mock(char *input){
  char * ret=malloc(strlen(input));
  strcpy(ret,input);
  for(size_t i=0; i<strlen(ret)-6; ++i){
    if(strncmp(ret+i,"_line_",6) == 0){
      ret[i]='\0';
      break;
    }
  }
  return ret;
}

__attribute__((constructor))
  void
  init_mock_c(){
    
      pthread_mutex_init(&mut_f_mock_glist, NULL);
      pthread_mutex_init(&mut_count_f_mock_wished, NULL);
      pthread_mutex_init(&mut_count_f_mock, NULL);
      pthread_mutex_init(&mut_count_expect_mock, NULL);
  }

extern bool is_parallel_nb;

char * number_call_translate(long nb){
  char *ret=malloc(250);
  if(nb>0) sprintf(ret,"%ld",nb);
  else if(nb == 0 ) sprintf(ret,"zero");
  else if(nb==INFINITY) sprintf(ret,"INFINITY");
  else sprintf(ret,"negative");

  return ret;
}



__attribute__((destructor))
  void
  check_mock_excpected(){

    //int kdefault=0, kgreen=1, kred=2, kyellow=3, kblue=4, knothing=5;
    if(unicolour) {
      kgreen=5; kred=5; kyellow=5; kblue=5;
    }

    struct winsize w;
    ioctl(1, TIOCGWINSZ, &w);
    
    char *reader=malloc(w.ws_col);
    strcpy(reader,"STAT OF MOCK FUNCTIONS");

    fprintf(F_OUT,"%s\n\n%0*d\n %*s \n%0*d %s\n\n", colors_f[kyellow] ,w.ws_col,0, (int)(w.ws_col+strlen(reader))/2, reader,w.ws_col,0, DEFAULT_K );


    is_parallel_nb = 0; /* no longer parallel here */
    struct func_mock_info_struct *tmock = f_mock_glist, *tfree;

    /* global order of fmock , order of expect and will */
    while(tmock){
      PRINT_DEBUG("check mock function: %s\n",tmock->str_namefunc);
      tfree=tmock;
 PRINT_DEBUG("**** STAT mock function:%s, conditions:%s t_left:%ld, init_left:%ld, failed_call:%ld\n",tmock->str_namefunc, tmock->str_conditions, tmock->times_left,tmock->init_times_left, tmock->failed_call);
      if((tmock->expect_call) && (tmock->init_times_left == tmock->times_left) || (tmock->failed_call)){
        PRINTF("%s%s %s %s expect to be called with condition %s, %s times, it was called %ld times and failed %ld times \n",colors_f[kred],HK_TR,tmock->str_namefunc,DEFAULT_K, tmock->str_conditions,
            number_call_translate(tmock->init_times_left),
            tmock->call/*tmock->failed_call + (tmock->init_times_left - tmock->times_left)*/, 
            tmock->failed_call );
      }
      tmock=tmock->next;
      //free(tfree);
    }
    
    PRINTF("\n%s%s STAT MOCK : there  are %ld mock functions, %ld wished response, %ld expected response, which are:\n\n",colors_f[kgreen],HK_EQ,count_f_mock, count_f_mock_wished, count_expect_mock);
    struct list_base_fmock *tmp_list_fm = g_list_base_fmock;
    struct func_mock_info_struct *tmp_inf_mock;
    /* list each fmock an each calls */
    while(tmp_list_fm){
      tmp_inf_mock = tmp_list_fm->info_mock;
      memset(reader,'=',w.ws_col-1); 
      char *nameff=extract_name_func_mock(tmp_inf_mock->str_namefunc);
      size_t len_nameff=strlen(nameff);
      size_t bg_rd=(w.ws_col - len_nameff)/2;
      for(size_t i=0; i<len_nameff; ++i) 
        reader[bg_rd+i]=nameff[i];

      PRINTF("%s%s%s\n\n",colors_f[kblue],reader,DEFAULT_K );
      while(tmp_inf_mock){
        if(0==strncmp(tmp_inf_mock->str_namefunc,nameff, len_nameff)){
          if(tmp_inf_mock->expect_call){
            if(tmp_inf_mock->init_times_left == tmp_inf_mock->times_left || tmp_inf_mock->failed_call){
              PRINTF("%s%s%s %s\t expect be call %s times,\t called %ld times and failed %ld times,\t with condition: %s,\t call by: %s \n" , 
                colors_f[kred],HK_EQ,DEFAULT_K,tmp_inf_mock->str_namefunc, number_call_translate(tmp_inf_mock->init_times_left), tmp_inf_mock->call, 
                tmp_inf_mock->failed_call, tmp_inf_mock->str_conditions , tmp_inf_mock->str_caller);
            }else{
              PRINTF("%s%s %s\t expect be call %s times,\t called %ld times and failed %ld times,\t with condition: %s,\t call by: %s %s\n" , 
                colors_f[kgreen],HK_TR,tmp_inf_mock->str_namefunc, number_call_translate(tmp_inf_mock->init_times_left), tmp_inf_mock->call, 
                tmp_inf_mock->failed_call, tmp_inf_mock->str_conditions , tmp_inf_mock->str_caller, DEFAULT_K);
            }
          }else{
            if(tmp_inf_mock->failed_call){
              PRINTF("%s%s%s %s\t   will be call %s times,\t called %ld times and failed %ld times,\t with condition: %s,\t call by: %s \n" , 
                colors_f[kred],HK_EQ,DEFAULT_K,tmp_inf_mock->str_namefunc, number_call_translate(tmp_inf_mock->init_times_left), tmp_inf_mock->call, 
                tmp_inf_mock->failed_call, tmp_inf_mock->str_conditions , tmp_inf_mock->str_caller);
            }else{  
              PRINTF("%s%s %s\t   will be call %s times,\t called %ld times and failed %ld times,\t with condition: %s,\t call by: %s %s\n" , 
                colors_f[kyellow],HK_TR,tmp_inf_mock->str_namefunc, number_call_translate(tmp_inf_mock->init_times_left), tmp_inf_mock->call, 
                tmp_inf_mock->failed_call, tmp_inf_mock->str_conditions , tmp_inf_mock->str_caller, DEFAULT_K);
            }
          }
        }
        tmp_inf_mock = tmp_inf_mock->next;
      }
      tmp_list_fm = tmp_list_fm->next;
    }

    PRINT_DEBUG("%s\n","check mock done!");
    
    pthread_mutex_destroy(&mut_f_mock_glist);
    pthread_mutex_destroy(&mut_count_f_mock_wished);
    pthread_mutex_destroy(&mut_count_f_mock);
    pthread_mutex_destroy(&mut_count_expect_mock);
  }

