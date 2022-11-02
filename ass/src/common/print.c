#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include "headers/common.h"
#include "headers/cpu.h"
#include "headers/memory.h"

// wrapper of stdio printf
// controlled by the debug verbose bit set
uint64_t debug_printf(uint64_t open_set, const char *format, ...) {
	if ((open_set & DEBUG_VERBOSE_SET) == 0x0) {
		return 0x1;
	}

	va_list argptr;
	va_start(argptr, format);
	vfprintf(stderr, format, argptr);
	va_end(argptr);

	return 0x0;
}

// debug function
void print_register(core_t *cr) {
	if ((DEBUG_VERBOSE_SET & DEBUG_REGISTERS) == 0x0) {
        return;
    }

    reg_t reg = cr->reg;

    printf("rax = %16lx\nrbx = %16lx\nrcx = %16lx\nrdx = %16lx\n",
            reg.rax, reg.rbx, reg.rcx, reg.rdx);
    printf("rsi = %16lx\nrdi = %16lx\nrbp = %16lx\nrsp = %16lx\n",
            reg.rsi, reg.rdi, reg.rbp, reg.rsp);
    printf("rip = %16lx\n", cr->rip);
	printf("CF = %u\tZF = %u\tSF = %u\tOF = %u\n",
        cr->flags.CF, cr->flags.ZF, cr->flags.SF, cr->flags.OF);
}

void print_stack(core_t *cr) {
	if ((DEBUG_VERBOSE_SET & DEBUG_PRINTSTACK) == 0x0) {
		return;
	}

    int n = 10;
    uint64_t *high  = (uint64_t *)&pm[va2pa((cr->reg).rsp, cr)];
    
    high = &high[n];
    uint64_t stack_start = (cr->reg).rsp + n * 8;
    for (int i = 0; i < 2 * n; ++i) {
        uint64_t *ptr = (uint64_t *)(high - i);
        printf("0x%016lx : %16lx", stack_start, (uint64_t)*ptr);
        if ((cr->reg).rbp == stack_start) {
            printf(" < === rbp");
        }
        stack_start -= 8;
        if (i == n) {
            printf(" < === rsp");
        }

        printf("\n");
    }
}