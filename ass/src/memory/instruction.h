#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdlib.h>
#include <stdint.h>

#define NUM_INSTRTYPE 30

// 操作
typedef enum OP
{
    mov_reg_reg,
    mov_reg_mem,
    mov_mem_reg,
    push_reg,
    pop_reg,
    call,
    ret,
    add_reg_reg
} op_t;

// 寻址方法
typedef enum OD_TYPE
{
    EMPTY,
    IMM,
    REG,
    MM_IMM,
    MM_REG,
    MM_IM_REG,
    MM_REG1_REG2,
    MM_IM_REG1_REG2,
    MM_REG2_S,
    MM_IMM_REG2_S,
    MM_REG1_REG2_S,
    MM_IM_REG1_REG2_S
} od_type_t;

// 操作数
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
    char code[100]; // str 
} inst_t;

typedef void (*handler_t)(uint64_t, uint64_t);

handler_t handler_table[NUM_INSTRTYPE];

// 初始化指令函数集
void init_handler_table();

// 指令周期函数
void instruction_cycle();

// 指令运算函数集
void add_reg_reg_handler(uint64_t src, uint64_t dst);
void mov_reg_reg_handler(uint64_t src, uint64_t dst);
void mov_reg_mem_handler(uint64_t src, uint64_t dst);
void mov_mem_reg_handler(uint64_t src, uint64_t dst);
void call_handler(uint64_t src, uint64_t dst);
void push_reg_handler(uint64_t src, uint64_t dst);
void pop_reg_handler(uint64_t src, uint64_t dst);

#endif // INSTRUCTION_H