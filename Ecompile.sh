#!/bin/bash
gcc test/is_good.c src/ftest/ftest.c src/fmock/fmock.c \
  src/tools_t/tools_t.c src/bar_progress/bar_progress.c \
  src/permutation_t/permutation_t.c src/set_theoric_t/set_theoric_t.c \
  -I./src $1 -E > eEcomp 
