#include <stdio.h>

#include "cpu.h"

int main(int argc, char *argv[]) {
    cpu_t cpu;
    cpu.rax = 0xabcdef0012345678;
    printf("rax = %0llx\n", cpu.rax);
    printf("eax = %0x\n", cpu.eax);
    printf("ax = %0x\n", cpu.ax);
    printf("al = %0x\n", cpu.al);
    printf("ah = %0x\n", cpu.ah);
    return 0;
}