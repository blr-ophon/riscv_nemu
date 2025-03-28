CC = gcc
OPT = -O3
CFLAGS = -std=c2x -fPIC -g -Wall -Wextra -pedantic $(OPT)

INCLUDES= -I ./include

CFILES_DIR := ./src
BUILD_DIR := ./build

CFILES := $(wildcard ./src/*.c)
CFILES := $(filter-out src/test.c, $(CFILES))
OBJECTS := $(CFILES:$(CFILES_DIR)/%.c=$(BUILD_DIR)/%.o)

MERGED_OBJECT := ./riscv_nemu.o
EXEC := ./riscv_nemu

${EXEC}: ${MERGED_OBJECT}
	gcc ${CFLAGS} ${INCLUDES} $^ -o $@

${MERGED_OBJECT}: ${OBJECTS}
	ld -r -o $@ $^ -g

${BUILD_DIR}/%.o: ${CFILES_DIR}/%.c
	mkdir -p $(dir $@)
	$(CC) ${CFLAGS} ${INCLUDES} -c $< -o $@

clean:
	rm -rf ${OBJECTS}
	rm -rf ${EXEC}
	rm -rf ${MERGED_OBJECT}

debug: ${EXEC}
	cgdb --args ./$< 

