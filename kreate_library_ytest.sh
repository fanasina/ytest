#!/bin/bash

for yfile in y*
do
  cp -r "$yfile/include" include_ytest/
done


gcc -c -Wall -Werror -fpic  yftest/src/ftest/ftest.c yfmock/src/fmock/fmock.c \
  ytools_t/src/tools_t/tools_t.c ybar_progress/src/bar_progress/bar_progress.c \
  -I./include_ytest/include $1 -lpthread 

gcc -shared -o libytest.so *.o 

rm *.o
