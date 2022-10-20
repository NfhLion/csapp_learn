#include "instruction.h"

static uint64_t decode_od(od_t od) {
    uint64_t res = 0;
    if (od.type == IMM) {
        return od.imm;
    } else if (od.type == REG) {
        return (uint64_t)od.reg1;
    } else {

    }
    return res;
}