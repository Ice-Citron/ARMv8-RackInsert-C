CC      ?= gcc

CFLAGS  ?= -std=c17 -g \
	-D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE \
	-Wall -Werror -pedantic \
	-Wextra -Wshadow -Wno-unused-parameter
CFLAGS += -I. -I assembler/ -I emulator/
LDFLAGS ?=

ASSEMBLE_SRCS = assembler/assemble.c \
	assembler/symbol_table.c \
	assembler/ass_helpers.c \
	assembler/ass_immreg.c \
	assembler/assemble_file.c \
	assembler/ass_branch.c \
	assembler/ass_single_data_transfer.c \
	assembler/dotint.c \
	assembler/assemble_dp_imm.c \
	assembler/assemble_dp_reg.c \
	assembler/assemble_dp_logical_reg.c \
	assembler/assemble_multiply.c \
	assembler/assemble_wide_move.c 
ASSEMBLE_OBJS = $(ASSEMBLE_SRCS:%.c=build/%.o)

.PHONY: all clean asan


EMULATE_SRCS = emulator/emulate.c \
	emulator/loader.c \
	emulator/dp_shared.c \
	emulator/dp_immediate.c \
	emulator/dp_register.c \
	emulator/data_transfer.c \
	emulator/branch.c
EMULATE_OBJS = $(EMULATE_SRCS:%.c=build/%.o)

.PHONY: all clean asan

all: assemble emulate


assemble: $(ASSEMBLE_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)


emulate: $(EMULATE_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

asan: CFLAGS  += -fsanitize=address,undefined -fno-omit-frame-pointer
asan: LDFLAGS += -fsanitize=address,undefined
asan: clean all


build:
	mkdir -p build


build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	$(RM) -r build assemble emulate