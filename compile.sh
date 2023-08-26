#!/bin/bash
gcc is_good.c src/test_t/test_t.c src/tools_t/tools_t.c \
  src/permutation_t/permutation_t.c src/set_theoric_t/set_theoric_t.c \
  -I./src -o lunch_is_good_c && ./lunch_is_good_c -h -p
