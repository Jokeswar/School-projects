.phony: build run test clean

TESTSRC = $(wildcard Test*.c)
TEST = $(patsubst %.c,%,$(TESTSRC))

SRC = $(foreach src, $(wildcard *c), $(patsubst Test%,,$(src)))
EXEC = $(patsubst %.c,%,$(SRC))
EXECNAME = electricity
HEADERS = $(wildcard *.h)

CC = gcc
CC_FLAGS = -std=c9x -g -O0
LD_FLAGS = -lm

build: $(EXEC) $(TEST)

$(EXEC):%:%.c $(HEADERS)
	$(CC) $(CC_FLAGS) $(firstword $+) -o $(EXECNAME) $(LD_FLAGS)

$(TEST):%:%.c $(HEADERS)
	$(CC) $(CC_FLAGS)  $(firstword $+) -o $@ $(LD_FLAGS)

clean:
	rm -f $(EXECNAME) $(TEST)