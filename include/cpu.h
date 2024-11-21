#ifndef CPU_H_
#define CPU_H_

#include <common.h>
#include <instruction.h>
#include <orchestr.h>

typedef struct {
    u8 a;
    u8 f;
    u8 b;
    u8 c;
    u8 d;
    u8 e;
    u8 h;
    u8 l;
    u16 pc;
    u16 sp;
} cpu_registers;

///  faire des unions 

typedef struct
{
    cpu_registers regi;
    u16 fetched_data;
    u16 mem_dest;
    u8 cur_op_code;
    u8 cur_opcode;
    instruction *cur_inst;

    bool halted;
    bool stepping;

} cpu_contex;

bool cpu_step(cpu_contex *ctx);
void cpu_init();


typedef void (*IN_PROC)(cpu_contex *);

#define CPU_FLAG_Z BIT(ctx->regs.f, 7)
#define CPU_FLAG_C BIT(ctx->regs.f, 4)

u16 cpu_read_reg(reg_type rt, cpu_contex *ctx);
void cpu_set_reg(reg_type rt, u16 val, cpu_contex *ctx);

#endif /* !CPU_H_ */
