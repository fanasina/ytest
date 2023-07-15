#!/bin/bash

make "$@"
compute-sanitizer --tool memcheck ./test/isgood
