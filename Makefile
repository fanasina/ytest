CC=nvcc
LDFLAGS=-lgtest -lpthread
ROOT_DIR=$(shell pwd)
INCLUDE_DIR=$(ROOT_DIR)
CFLAGS=-I$(INCLUDE_DIR)
SRC_DIR=$(ROOT_DIR)/src
SRC=$(wildcard src/*/*.c*) $(wildcard src/*/*/*.c*)
OBJPP=$(SRC:.cpp=.o)
OBJS=$(OBJPP:.cu=.o)
#HEADS=$(OBJS:.o=.h)
TEST_DIR=$(ROOT_DIR)/test
EXEC=$(TEST_DIR)/isgood
PERMSRC=$(wildcard src/*/*perm*.cpp)
PERMSRC_O=$(PERMSRC:.cpp=.o)
DIMSRC=$(wildcard src/*/*dim*.cpp)
DIMSRC_O=$(DIMSRC:.cpp=.o)
TENSRCPP=$(wildcard src/*/*/tens*.cpp)
TENSRCPP_O=$(TENSRCPP:.cpp=.o)
TENSRCU=$(wildcard src/*/*/tens*.cu)
TENSRCU_O=$(TENSRCU:.cu=.o)
DTENSRCU=$(wildcard src/*/*/d_tens*.cu)
DTENSRCU_O=$(DTENSRCU:.cu=.o)


TENSRC=$(TENSRCPP) $(TENSRCU)
all: $(EXEC)

$(EXEC):	$(EXEC).cu $(OBJS)
	$(CC) -o $@ $^ -I$(INCLUDE_DIR) $(LDFLAGS)

$(DIMSRC_O): $(DIMSRC) $(PERMSRC_O)
	$(CC) -o $@ -c $< $(CFLAGS)

$(TENSRCPP_O): $(TENSRCPP) $(DIMSRC_O)
	$(CC) -o $@ -c $< $(CFLAGS)

$(TENSRCU_O): $(TENSRCU) $(DTENSRCU_O) $(DIMSRC_O)
	$(CC) -o $@ -c $< $(CFLAGS)

$(PERMSRC_O): $(PERMSRC) 
	$(CC) -o $@ -c $< $(CFLAGS)

$(DTENSRCU_O) : $(DTENSRCU)
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	rm -f $(OBJS)

mrproper: clean
	rm -f $(EXEC)

