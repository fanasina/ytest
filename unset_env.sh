#!/bin/bash
file_env=tempFILE
if [ -e  "$file_env" ] ; then
  rm "$file_env"
fi
if [ "$#" -le 0 ] ; then
  echo "Usage: $0 file_name" >&2
  echo "for example: $0 ~/.bashrc" >&2
  echo "or remove these lines in bash profile (for example: ~/.bashrc) th file you have set whith set_env.sh"
  echo "export CPATH=$PWD/include_ytest/include:\$CPATH" >> $file_env
  echo "export LD_LIBRARY_PATH=$PWD:\$LD_LIBRARY_PATH" >> $file_env

  cat $file_env
  rm $file_env
fi


if [ "$#" -ge 1 ] ; then
  sed -i "\@export CPATH=$PWD/include_ytest/include:\$CPATH@d" "$1"
  sed -i "\@export LD_LIBRARY_PATH=$PWD:\$LD_LIBRARY_PATH@d"  "$1"
  
fi
