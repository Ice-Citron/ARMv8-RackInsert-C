CC      ?= gcc
CFLAGS  ?= -std=c17 -D_POSIX_SOURCE=200809L -D_DEFAULT_SOURCE \
		   -Wall -Wextra -Werror -pedantic -Wshadow -Wconversion -g

.SUFFIXES: .c .o

.PHONY: all clean asan build

build:
	mkdir -p build

build/%.o : %.c | build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o assemble emulate
	
