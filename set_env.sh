#!/bin/bash
file_env=tempFILE
if [ -e  "$file_env" ] ; then
  rm "$file_env"
fi
if [ "$#" -le 0 ] ; then
  echo "Usage: $0 file_name" >&2
  echo "for example: $0 ~/.bashrc" >&2
  echo "or add these lines in bash profile for example: ~/.bashrc"
else
  file_env=$1
fi
echo "export CPATH=$PWD/include_ytest/include:\$CPATH" >> $file_env
echo "export LD_LIBRARY_PATH=$PWD:\$LD_LIBRARY_PATH" >> $file_env

if [ "$#" -le 0 ] ; then
  cat $file_env
  rm $file_env
fi

