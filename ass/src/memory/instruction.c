#include <stdio.h>

#include "instruction.h"
#include "cpu/mmu.h"
#include "cpu/register.h"
#include "memory/dram.h"

static uint64_t decode_od(od_t od) {
    uint64_t vaddr = 0;
    if (od.type == IMM) {
        return *((uint64_t *)&od.imm);
    } else if (od.type == REG) {
        return (uint64_t)od.reg1;
    } else {
        // mm 
        if (od.type == MM_IMM) {
            vaddr = od.imm;
        } else if (od.type == MM_REG) {
            vaddr = *(od.reg1);
        } else if (od.type == MM_IM_REG) {
            vaddr = od.imm + *(od.reg1);
        } else if (od.type == MM_REG1_REG2) {
            vaddr = *(od.reg1) + *(od.reg2);
        } else if (od.type == MM_IM_REG1_REG2) {
            vaddr = *(od.reg1) + *(od.reg2) + od.imm;
        } else if (od.type == MM_REG2_S) {
            vaddr = (*(od.reg2)) * od.scal;
        } else if (od.type == MM_IMM_REG2_S) {
            vaddr = (*(od.reg2)) * od.scal + od.imm;
        } else if (od.type == MM_REG1_REG2_S) {
            vaddr = *(od.reg1) + (*(od.reg2)) * od.scal;
        } else if (od.type == MM_IM_REG1_REG2_S) {
            vaddr = od.imm + *(od.reg1) + (*(od.reg2)) * od.scal;
        }
    }
    return vaddr;
}

void init_handler_table() {
    handler_table[mov_reg_reg] = &mov_reg_reg_handler;
    handler_table[mov_reg_mem] = &mov_reg_mem_handler;
    handler_table[mov_mem_reg] = &mov_mem_reg_handler;
    handler_table[add_reg_reg] = &add_reg_reg_handler;
    handler_table[call] = &call_handler;
    handler_table[push_reg] = &push_reg_handler;
    handler_table[pop_reg] = &pop_reg_handler;
}

void instruction_cycle() {
    // 取指
    inst_t *instr = (inst_t *)reg.rip;

    // 译码(寻址)
    uint64_t src = decode_od(instr->src);
    uint64_t dst = decode_od(instr->dst);

    // 执行
    handler_t handler = handler_table[instr->op];
    handler(src, dst);

    printf("\t%s\n", instr->code);
}

void mov_reg_reg_handler(uint64_t src, uint64_t dst) {
    // mov %rax %rbx (src = &rax, dst = &rbx)
    *(uint64_t *)dst = *(uint64_t *)src;
    reg.rip = reg.rip + sizeof(inst_t);
}

void mov_reg_mem_handler(uint64_t src, uint64_t dst) {
    write64bits_dram(
        va2pa(dst),
        *(uint64_t *)src
    );
    reg.rip = reg.rip + sizeof(inst_t);
}

void mov_mem_reg_handler(uint64_t src, uint64_t dst) {
    *(uint64_t *)dst = read64bits_dram(va2pa(src));
    reg.rip = reg.rip + sizeof(inst_t);
}

void push_reg_handler(uint64_t src, uint64_t dst) {
    reg.rsp = reg.rsp - 8;
    write64bits_dram(
        va2pa(reg.rsp),
        *(uint64_t *)src
    );
    reg.rip = reg.rip + sizeof(inst_t);
}

void pop_reg_handler(uint64_t src, uint64_t dst) {

}

void call_handler(uint64_t src, uint64_t dst) {
    // src: imm address of called function
    reg.rsp = reg.rsp - 8;
    // write return address to rsp memory
    write64bits_dram(
        va2pa(reg.rsp),
        reg.rip + sizeof(inst_t)
    );
    reg.rip = src;
}

void add_reg_reg_handler(uint64_t src, uint64_t dst) {
    // add %rax %rbx (src = &rax, dst = &rbx)
    *(uint64_t *)dst = *(uint64_t *)dst + *(uint64_t *)src;
    reg.rip = reg.rip + sizeof(inst_t);
}