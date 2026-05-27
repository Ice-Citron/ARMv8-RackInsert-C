CC      = gcc
CFLAGS  = -std=c17 -D_POSIX_SOURCE=200809L -D_DEFAULT_SOURCE \
			-Wall -Wextra -pedantic -Wshadow -Wconversion -g
SRCS 	= $(wildcard *.c)
OBJS 	= $(SRCS:%.c=build/%.o)

.SUFFIXES: .c .o

.PHONY: all clean asan build

build: $(OBJS)
	mkdir -p build

build/%.o : %.c | build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build
