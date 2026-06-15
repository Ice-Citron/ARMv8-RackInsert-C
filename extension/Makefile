CC      = gcc
CFLAGS  = -std=c17 -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE \
			-Wall -Wextra -pedantic -Wshadow -Wconversion -g
LDFLAGS	=
LDLIBS  = -lm

RACK_SRCS = src/main.c src/evaluator.c
RACK_OBJS = $(RACK_SRCS:src/%.c=build/%.o)

TEST_SRCS = tests/test_evaluator.c src/evaluator.c
TEST_OBJS = build/test_evaluator.o build/evaluator.o

.PHONY: all clean run tests asan

all: rack_insert

rack_insert: $(RACK_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

test_evaluator: $(TEST_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)