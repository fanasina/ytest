
NAME_TEST=is_good
CC=gcc
LDFLAGS=-lpthread -D DEBUG=1
ROOT_DIR=$(shell pwd)
INCLUDE_DIR=$(ROOT_DIR)/src
CFLAGS=-I$(INCLUDE_DIR)
SRC_DIR=$(ROOT_DIR)/src
SRC=$(wildcard */*/*.c) 
OBJ=$(SRC:.c=.o)
#HEADS=$(OBJS:.o=.h)
TEST_DIR=$(ROOT_DIR)/test
EXECSRC=$(TEST_DIR)/$(NAME_TEST).c
EXEC=$(ROOT_DIR)/launch_$(NAME_TEST)_m
PERMSRC=src/permutation_t/permutation_t.c
PERMSRC_O=$(PERMSRC:.c=.o)
SETTSRC=src/set_theoric_t/set_theoric_t.c
SETTSRC_O=$(SETTSRC:.c=.o)
TOOLSRC=src/tools_t/tools_t.c
TOOLSRC_O=$(TOOLSRC:.c=.o)
FTESTSRC=src/ftest/ftest.c
FTESTSRC_O=$(FTESTSRC:.c=.o)
FMOCKSRC=src/fmock/fmock.c
FMOCKSRC_O=$(FMOCKSRC:.c=.o)


all: $(EXEC)

$(EXEC):	$(EXECSRC) $(OBJ)
	$(CC) -o $@ $^ -I$(INCLUDE_DIR) $(LDFLAGS)

$(FMOCKSRC_O): $(FMOCKSRC) $(FTESTSRC_O) 
	$(CC) -o $@ -c $< $(CFLAGS)

$(FTESTSRC_O): $(FTESTSRC) $(TOOLSRC_O) 
	$(CC) -o $@ -c $< $(CFLAGS)

$(PERMSRC_O): $(PERMSRC) $(SETTSRC_O)
	$(CC) -o $@ -c $< $(CFLAGS)

$(SETTSRC_O) : $(SETTSRC) $(TOOLSRC_O)
	$(CC) -o $@ -c $< $(CFLAGS)

$(TOOLSRC_O): $(TOOLSRC) 
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	rm -f $(OBJS)

mrproper: clean
	rm -f $(EXEC)

run: $(EXEC)
	$(EXEC) -p
