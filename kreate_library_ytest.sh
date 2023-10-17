#!/bin/bash

for yfile in y*
do
  cp -r "$yfile/include" include_ytest/
done

if [ "$#" -le 0 ] ; then
  echo "Usage: $0" >&2
  echo "we can add more option for example '-D DEBUG=1' to have debug print,  or '-g' to gbd" >&2
  echo "for example: $0 \"-D DEBUG=1 -g\""
fi


gcc -c -Wall -Werror -fpic  yftest/src/ftest/ftest.c yfmock/src/fmock/fmock.c \
  ytools_t/src/tools_t/tools_t.c ybar_progress/src/bar_progress/bar_progress.c \
  -I./include_ytest/include $1 -lpthread 

gcc -shared -o libytest.so *.o 

rm *.o
