# --------------------------------------------------
# Makefile for shell project
# --------------------------------------------------

# Declare all phony (non-file) targets
.PHONY: all build run test clean

# Compiler and flags
CC = gcc
CFLAGS = -g -Wall

# Source and target definitions
SHELL_SRCS = parser.c shell.c
SHELL_TARGET = shell.out

TEST_SRCS = parser.c test.c 
TEST_TARGET = test.out


all: build

build: $(SHELL_TARGET)


$(SHELL_TARGET): $(SHELL_SRCS)
	$(CC) $(CFLAGS) -o $@ $(SHELL_SRCS)


run: $(SHELL_TARGET)
	./$(SHELL_TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRCS)
	$(CC) $(CFLAGS) -o $@ $(TEST_SRCS)


clean:
	rm -f *.out
