CC = /usr/bin/gcc
CFLAGS = -Wall -g -O2 -Werror -std=gnu99 -Wno-unused-function

BIN_HARDWARE = ./bin/test_hardware

SRC_DIR = ./src

HEADERS = ./src

# debug
COMMON = $(SRC_DIR)/common/print.c $(SRC_DIR)/common/convert.c

# hardware
CPU =$(SRC_DIR)/hardware/cpu/mmu.c $(SRC_DIR)/hardware/cpu/isa.c
MEMORY = $(SRC_DIR)/hardware/memory/dram.c

# main
TEST_HARDWARE = $(SRC_DIR)/tests/test_hardware.c

.PHONY:hardware run clean

hardware:
	$(CC) $(CFLAGS) -I$(HEADERS) $(COMMON) $(CPU) $(MEMORY) $(TEST_HARDWARE) -o $(BIN_HARDWARE)

run:
	./$(BIN_HARDWARE)

clean:
	rm -f *.o *~ $(BIN_HARDWARE)
