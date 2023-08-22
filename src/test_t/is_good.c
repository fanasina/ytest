#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// for sleep !
#ifdef __linux__ 
  #include <unistd.h>
#elif _WIN32 
  #include <windows.h>
#endif

#include "src/test_t/test_t.h"

//#include "src/permutation_t/permutation_t.h"
TEST(size_permutation2){
  printf("another size_permutation2 again\n");
  ASSERT_TRUE(false);
}

TEST(size_permutation)
{
/*
  PERMUTATION_TYPE_CHAR *p = CREATE_PERMUTATION_TYPE_CHAR(3);
  
  printf(" size = %u \n",p->size);
  if(p->size == 3) print_OK_with_msg_endl(" FF yeah GOOD test size passed ");
  else  print_KO_with_msg_endl("NOT GOOD test size not passed ");
*/
  printf("test size_permutation2\n");
}
TEST(size_permutation2){
  printf("another size_permutation2 again false\n");
  bool val_bool = false;
  ASSERT_TRUE(val_bool);
}
TEST(size_permutation2)
{
  printf("test size_permutation2\n");
  bool val_bool = true;
  ASSERT_FALSE(val_bool);
/*
  PERMUTATION_TYPE_CHAR *p = CREATE_PERMUTATION_TYPE_CHAR(3);
  
  printf(" size = %u \n",p->size);
  if(p->size == 3) print_OK_with_msg_endl(" FF yeah GOOD test size passed ");
  else  print_KO_with_msg_endl("NOT GOOD test size not passed ");
*/
}
TEST(float_equal){
  printf("another size_permutation2 float\n");
  ASSERT_TRUE(true);
  float a = 1.00001f;
  float b = 1.00001f;
  ASSERT_EQ_TYPE_FLOAT(a,b);
  b=1.0000101f;
  ASSERT_EQ_TYPE_FLOAT(a,b);
  ASSERT_EQ_TYPE_FLOAT(1.0000102f,b);
}
TEST(double_equal){
  printf("another size_permutation2 double\n");
  ASSERT_TRUE(true);
  double a = 1.00000001;
  double b = 1.00000001;
  ASSERT_EQ_TYPE_DOUBLE(a,b);
  b=1.00000001000000001;
  ASSERT_EQ_TYPE_DOUBLE(a,b);
  ASSERT_EQ_TYPE_DOUBLE(1.0000000100000002,b);
}

TEST(){
  unsigned char c = 'a';

  debug_print("another size_permutation2, a = %c\n",c);
  ASSERT_FALSE(true);
  ASSERT_TRUE(true);
  ASSERT_TRUE(true);
}

TEST(){
  sleep(3);
  int a = 5;
  long b = 5;
  ASSERT_EQ(a,b);
  a=4;
  ASSERT_EQ(a,b);

}

TEST(expect){
  sleep(2);
  int a = 5;
  int b = 6;
  EXPECT_EQ(a,b);
  //SKIP();
  SKIP("on skip eq string\n");
  EXPECT_EQ_TYPE_STRING("hello","hello");
  float f1 = 1.00019999, f2=1.00019999;
  EXPECT_EQ_TYPE_FLOAT(f1,f2);

}

TEST(){
  printf("no test, only print\n");
}

TEST(){
  printf("no test, only print\n");
}

TEST(){
  printf("no test, only print\n");
}


TEST(){
  printf("no test, only print\n");
}

TEST(lessThan){
  long int a=1,b=2;
  EXPECT_LT(a,b);
  EXPECT_LT(b,a);

}

int main(){
  
  //run_all_tests();
  run_all_tests_parallel(4);

  //purge_tests();
  //run_some_tests(8, 1, 2, 2, 3, 3, 0, 4, 1);
  //run_some_tests(8, 5, 7, 1, 1, 1, 1, 1, 1);
  //run_some_tests_one_by_one(3, 1, 2, 2);
  //run_all_tests_exept(2, 1, 3);
  return 0;
}
