CC      ?= gcc

CFLAGS  ?= -std=c17 -g \
	-D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE \
	-Wall -Werror -pedantic \
	-Wextra -Wshadow -Wno-unused-parameter
LDFLAGS ?=


ASSEMBLE_SRCS = assemble.c \
	assemble_file.c \
	symbol_table.c \
	parse_reg.c \
	ass_branch.c \
	ass_single_data_transfer.c \
	dotint.c \
	assemble_dp_imm.c \
	assemble_dp_reg.c \
	assemble_dp_logical_reg.c \
	assemble_multiply.c \
	assemble_wide_move.c 
ASSEMBLE_OBJS = $(ASSEMBLE_SRCS:%.c=build/%.o)

.PHONY: all clean asan

all: assemble


assemble: $(ASSEMBLE_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)


asan: CFLAGS  += -fsanitize=address,undefined -fno-omit-frame-pointer
asan: LDFLAGS += -fsanitize=address,undefined
asan: clean all


build:
	mkdir -p build


build/%.o: %.c | build
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	$(RM) -r build assemble
	
