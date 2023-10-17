# lib: -lytest
PROJECT_LIB=libytest.so
CC=gcc
#LDFLAGS=
INCLUDE_DIRS=$(PWD) 
#$(wildcard $(PWD)/**/include)
INCLUDE=-I$(PWD)/yftest/include  -I$(PWD)/yfmock/include -I$(PWD)/ytools_t/include -I$(PWD)/ybar_progress/include
CFLAGS=-lpthread -Wall -Werror -fpic $(INCLUDE) 

TOPTARGETS := all clean #update_headers

#SRC=$(wildcard y*/src/**/**/*.c) 
SRC=$(wildcard y*/*/*/*.c) 
OBJ=$(SRC:.c=.o)

SUBDIRS :=$(wildcard y*)

export


$(TOPTARGETS): $(SUBDIRS)

all: $(PROJECT_LIB) 


$(PROJECT_LIB): $(OBJ)
	echo $(OBJ)
	#$(CC) -shared -o $@ $^ $(INCLUDE) $(LDFLAGS)
	$(CC) -shared -o $@ $^ $(LDFLAGS)


$(SUBDIRS):
		$(MAKE) -C $@ $(MAKECMDGOALS)

update_headers: $(PROJECT_LIB)
	for file_h in $(SUBDIRS); do cp -r "$$file_h/include" include_ytest/;	done	

.PHONY: $(TOPTARGETS) $(SUBDIRS)

clean:
	rm -f $(PROJECT_LIB)

remove_headers:
	rm -r include_ytest/*

#SRC_test=test/is_good.c

#compile: $(SRC_test) $(PROJECT_LIB) 
#	$(CC) -o launch_is_good_m $< -L. test/src/permutation_t/permutation_t.o test/src/set_theoric_t/set_theoric_t.o -lytest -I./test/src -I./include_ytest
