CC     =  gcc
PYTHON ?= python3 
VENV   ?= .venv

# MuJoCo Library Dependency
MUJOCO_DIR ?= $(shell $(PYTHON) -c \
				'import mujoco, pathlib; \
			  	print(pathlib.Path(mujo.__file__).resolve().parent)' \
			  	2>/dev/null)
MUJOCO_LIB ?= $(notdir $(firstword $(wildcard $(MUJOCO_DIR)/libmujoco.so*)))

CFLAGS  = -std=c17 -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE \
		  	-Wall -Wextra -pedantic -Wshadow -Wconversion -g  \ 
			-Iinclude -I$(MUJOCO_DIR)/include
LDFLAGS	=
LDLIBS  = -lm

RACK_SRCS = src/main.c src/evaluator.c src/sim.c
RACK_OBJS = $(RACK_SRCS:src/%.c=build/%.o)

TEST_SRCS = tests/test_evaluator.c src/evaluator.c
TEST_OBJS = build/test_evaluator.o build/evaluator.o

.PHONY: all clean run test asan deps

all: rack_insert

rack_insert: $(RACK_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

test_evaluator: $(TEST_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

run: rack_insert
	./rack_insert

test: test_evaluator
	./test_evaluator

# For debugging with AddressSanitizer
asan: CFLAGS += -fsanitize=address,undefined -fno-omit-frame-pointer
asan: LDFLAGS += -fsanitize=address,undefined
asan: clean all test_evaluator

build:
	mkdir -p build

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/test_%.o: tests/test_%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build rack_insert test_evaluator