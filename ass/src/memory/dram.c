#include <stdio.h>

#include "memory/dram.h"
#include "cpu/register.h"
#include "cpu/mmu.h"

#define SRAM_CACHE_SETTING 0    // 是否在访问内存前先访问cache

// flighting
uint64_t read64bits_dram(uint64_t paddr) {
    if (SRAM_CACHE_SETTING) {
        return 0x00;
    }
    
    uint64_t val = 0x0;

    val += (((uint64_t)mm[paddr + 0]) << 0);
    val += (((uint64_t)mm[paddr + 1]) << 8);
    val += (((uint64_t)mm[paddr + 2]) << 16);
    val += (((uint64_t)mm[paddr + 3]) << 24);
    val += (((uint64_t)mm[paddr + 4]) << 32);
    val += (((uint64_t)mm[paddr + 5]) << 40);
    val += (((uint64_t)mm[paddr + 6]) << 48);
    val += (((uint64_t)mm[paddr + 7]) << 56);
    return val;
}

void write64bits_dram(uint64_t paddr, uint64_t data) {
    if (SRAM_CACHE_SETTING) {
        return;
    }

    mm[paddr + 0] = (data >> 0 ) & 0xff;
    mm[paddr + 1] = (data >> 8 ) & 0xff;
    mm[paddr + 2] = (data >> 16) & 0xff;
    mm[paddr + 3] = (data >> 24) & 0xff;
    mm[paddr + 4] = (data >> 32) & 0xff;
    mm[paddr + 5] = (data >> 40) & 0xff;
    mm[paddr + 6] = (data >> 48) & 0xff;
    mm[paddr + 7] = (data >> 56) & 0xff;
}

// debug function
void print_register() {
    printf("rax = %16lx\nrbx = %16lx\nrcx = %16lx\nrdx = %16lx\n",
            reg.rax, reg.rbx, reg.rcx, reg.rdx);
    printf("rsi = %16lx\nrdi = %16lx\nrbp = %16lx\nrsp = %16lx\n",
            reg.rsi, reg.rdi, reg.rbp, reg.rsp);
    printf("rip = %16lx\n", reg.rip);
}

void print_stack() {
    int n = 10;
    uint64_t *high  = (uint64_t *)&mm[va2pa(reg.rsp)];
    
    high = &high[n];
    uint64_t stack_start = reg.rsp + n * 8;
    for (int i = 0; i < 2 * n; ++i) {
        uint64_t *ptr = (uint64_t *)(high - i);
        printf("0x%016lx : %16lx", stack_start, (uint64_t)*ptr);
        stack_start -= 8;
        if (i == n) {
            printf(" < === rsp");
        }
        if (reg.rbp == stack_start) {
            printf(" < === rbp");
        }
        printf("\n");
    }
}