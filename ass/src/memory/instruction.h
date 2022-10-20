#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdlib.h>
#include <stdint.h>

// 操作
typedef enum OP
{
    MOV,
    PUSH,
    CALL
} op_t;

// 寻址方法
typedef enum OD_TYPE
{
    IMM,
    REG,
    MM_IMM,
    MM_IM_REG,
    MM_REG1_REG2,
    MM_IM_REG1_REG2,
    MM_REG2_S,
    MM_IMM_REG2_S,
    MM_REG1_REG2_S,
    MM_IM_REG1_REG2_S
} od_type_t;

typedef struct OD
{
    /* data */
    od_type_t type;
    int64_t imm;  // 立即数
    int64_t scal; // 乘数
    uint64_t *reg1; // 1 寄存器
    uint64_t *reg2; // 2 寄存器
} od_t;

// 指令
typedef struct INSTRUCT_STRUCT
{
    /* data */
    op_t op;  // mov push call
    od_t src; // source
    od_t dst; // target
} inst_t;

static uint64_t decode_od(od_t od);

#endif // INSTRUCTION_H