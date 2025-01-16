#ifndef CPU_H_
#define CPU_H_

#include <common.h>
#include <instruction.h>
#include <orchestr.h>

typedef struct {
    union {struct {
            u8 f;
            u8 a;
        };u16 af;};
    union {struct {
            u8 c;
            u8 b;
        };u16 bc;};
    union {struct {
            u8 e; 
            u8 d; 
        };u16 de;};
    union { struct {
            u8 l; 
            u8 h; 
        };u16 hl;};
    u16 pc;
    u16 sp;
} cpu_registers;

typedef struct
{
    cpu_registers regi;
    u16 fetched_data;
    u16 mem_dest;
    u8 cur_opcode;

    u8 cycles;

    uint8_t memory[0x10000];

    bool halted;
    bool stepping;
    
    bool IME;
    bool IME_delay;
    u8 ie_register;    //(Interrupt Enable Register)
    u8 if_register;    //(Interrupt Flag Register)
} cpu_context;

bool cpu_step(cpu_context *ctx);
void cpu_init();


typedef void (*IN_PROC)(cpu_context *);


#define ZERO_FLAG BIT(cpu->regi.f, 7)
#define CARRY_FLAG BIT(cpu->regi.f, 4)

#define SET_ZERO_FLAG(cpu) (cpu->regi.f |= (1 << 7))
#define CLEAR_ZERO_FLAG(cpu) (cpu->regi.f &= ~(1 << 7))
#define SET_CARRY_FLAG(cpu) (cpu->regi.f |= (1 << 4))
#define CLEAR_CARRY_FLAG(cpu) (cpu->regi.f &= ~(1 << 4))
#define SET_HALF_CARRY_FLAG(cpu) (cpu->regi.f |= (1 << 5))
#define CLEAR_HALF_CARRY_FLAG(cpu) (cpu->regi.f &= ~(1 << 5))
#define SET_SUBTRACT_FLAG(cpu) (cpu->regi.f |= (1 << 6))
#define CLEAR_SUBTRACT_FLAG(cpu) (cpu->regi.f &= ~(1 << 6))


void execute_instruction(cpu_context *cpu);
u16 cpu_read_reg(reg_type rt, cpu_context *ctx);
void cpu_set_reg(reg_type rt, u16 val, cpu_context *ctx);

#endif /* !CPU_H_ */
