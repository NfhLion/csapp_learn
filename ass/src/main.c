#include <stdint.h>

#include "disk/elf.h"
#include "cpu/register.h"
#include "cpu/mmu.h"
#include "memory/instruction.h"
#include "memory/dram.h"

int main(int argc, char *argv[]) {

    init_handler_table();
    
    // init register
    reg.rax = 0x12340000;
    reg.rbx = 0x0;
    reg.rcx = 0x8000660;
    reg.rdx = 0xabcd;
    reg.rsi = 0x7ffffffee2f8;
    reg.rdi = 0x1;
    reg.rbp = 0x7ffffffee210;
    reg.rsp = 0x7ffffffee1f0;
    
    reg.rip = *(uint64_t *)&(program[11]);

    // init memory
    mm[va2pa(0x7ffffffee210)] = 0x08000660;         // rbp
    mm[va2pa(0x7ffffffee208)] = 0x0;
    mm[va2pa(0x7ffffffee200)] = 0xabcd;
    mm[va2pa(0x7ffffffee1f8)] = 0x12340000;
    mm[va2pa(0x7ffffffee1f0)] = 0x08000660;         // rsp

    // run inst
    

    // match out

    return 0;
}