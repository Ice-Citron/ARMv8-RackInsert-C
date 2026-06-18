CC     =  gcc
VENV   ?= .venv
# Use venv Python if exitsts, otherwise fall back to system python3for rack_insert.xn
PYTHON ?= $(if $(wildcard $(VENV)/bin/python),$(VENV)/bin/python,python3)

# MuJoCo Library Dependency
MUJOCO_DIR ?= $(shell $(PYTHON) -c \
				'import mujoco, pathlib; \
			  	print(pathlib.Path(mujoco.__file__).resolve().parent)' \
			  	2>/dev/null)
MUJOCO_LIB ?= $(notdir $(firstword $(wildcard $(MUJOCO_DIR)/libmujoco.so*)))

CFLAGS  = -std=c17 -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE \
		  	-Wall -Wextra -pedantic -Wshadow -Wconversion -g \
			-Iinclude -I$(MUJOCO_DIR)/include \
			-DMUJOCO_PLUGIN_DIR=\"$(MUJOCO_DIR)/plugin\" # defines preprocessor macro of `MUJOCO_PLUGIN_DIR`
LDFLAGS	= -L$(MUJOCO_DIR) -Wl,-rpath,$(MUJOCO_DIR)
LDLIBS  = -l:$(MUJOCO_LIB) -lm

RACK_SRCS = src/main.c src/evaluator.c src/sim.c src/vec.c src/logging.c \
			src/trajectory.c src/scripted_policy.c

RACK_OBJS = $(RACK_SRCS:src/%.c=build/%.o)

EVALUATOR_TEST_OBJS 	  = build/test_evaluator.o build/evaluator.o build/vec.o 
ACTUATOR_TEST_OBJS   	  = build/sim.o build/test_actuators.o
SCRIPTED_POLICY_TEST_OBJS = build/evaluator.o build/scripted_policy.o \
							build/sim.o build/vec.o build/test_scripted_policy.o

.PHONY: all clean run test asan deps check-mujoco

all: rack_insert

# Safety Guards: Catch MuJoCo-dependency errors before compiler attempts to 
# 				 build the full Project.
check-mujoco:
	@test -n "$(MUJOCO_DIR)" || \
		{ echo "ERROR: MuJoCo not found. Run `make deps && \
		PYTHON=.venv/bin/python`"; exit 1; }
	@test -n "$(MUJOCO_LIB)" || \
		{ echo "ERROR: libmujoco.so not found in $(MUJOCO_DIR)."; exit 1; }

rack_insert: $(RACK_OBJS) | check-mujoco
	$(CC) $(CFLAGS) $(RACK_OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

test_evaluator: $(EVALUATOR_TEST_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

test_actuators: $(ACTUATOR_TEST_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

test_scripted_policy: $(SCRIPTED_POLICY_TEST_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

run: rack_insert
	./rack_insert

test: test_evaluator test_actuators test_scripted_policy
	./test_evaluator
	./test_actuators
	./test_scripted_policy

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

# If MuJoCo not previously installed on machine, run `make deps`
deps:
	$(PYTHON) -m venv $(VENV)
	$(VENV)/bin/python -m pip install --upgrade pip
	$(VENV)/bin/python -m pip install mujoco

clean:
	rm -rf build rack_insert test_evaluator test_actuators MUJOCO_LOG.TXT