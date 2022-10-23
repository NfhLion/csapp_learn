#ifndef DRAM_H
#define DRAM_H

#include <stdlib.h>
#include <stdint.h>

#define MM_LEN 8192

uint8_t mm[MM_LEN];     // physical memory

uint64_t read64bits_dram(uint64_t paddr);
void write64bits_dram(uint64_t paddr, uint64_t data);

void print_register();
void print_stack();

#endif // DRAM_H