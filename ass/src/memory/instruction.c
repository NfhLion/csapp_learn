#include "instruction.h"
#include "cpu/mmu.h"
#include "cpu/register.h"

static uint64_t decode_od(od_t od) {
    uint64_t vaddr = 0;
    if (od.type == IMM) {
        return od.imm;
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
    return va2pa(vaddr);
}

void instruction_cycle() {
    inst_t *instr = (inst_t *)reg.rip;
}