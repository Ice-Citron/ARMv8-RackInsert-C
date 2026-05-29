CC      = gcc
CFLAGS  = -std=c17 -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE \
			-Wall -Wextra -pedantic -Wshadow -Wconversion -g
LDFLAGS	=

ASSEMBLE_SRCS	= assemble.c
ASSEMBLE_OBJS 	= $(ASSEMBLE_SRCS:%.c=build/%.o)
EMULATE_SRCS 	= emulate.c dp_shared.c dp_register.c loader.c dp_immediate.c \
				  data_transfer.c
EMULATE_OBJS 	= $(EMULATE_SRCS:%.c=build/%.o)

.PHONY: all clean asan

all: assemble emulate

assemble: $(ASSEMBLE_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

emulate: $(EMULATE_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# For debugging (Address Sanitizers)
asan: CFLAGS 	+= -fsanitize=address,undefined -fno-omit-frame-pointer
asan: LDFLAGS	+= -fsanitize=address,undefined
asan: clean all

build:
	mkdir -p build

build/%.o : %.c | build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build assemble emulate
