#ifndef CPU_H_
#define CPU_H_

#include <common.h>

typedef struct {
    __extension__ union {struct {
            u8 f;
            u8 a;
        };u16 af;};
    __extension__ union {struct {
            u8 c;
            u8 b;
        };u16 bc;};
    __extension__ union {struct {
            u8 e; 
            u8 d; 
        };u16 de;};
    __extension__ union { struct {
            u8 l; 
            u8 h; 
        };u16 hl;};
    u16 pc;
    u16 sp;
    u8 ie_register;    //(Interrupt Enable Register)
    u8 if_register;    //(Interrupt Flag Register)
} cpu_registers;

typedef struct
{
    cpu_registers regi;
    u16 fetched_data;
    u16 mem_dest;
    u8 cur_opcode;

    u8 cycles;

    bool halted;
    bool stepping;
    
    bool IME;
    bool IME_delay;
} cpu_context;


typedef void (*IN_PROC)(cpu_context *);

#define INCR_CYCLE(x) (cpu->cycles += (x)) 

#define ZERO_FLAG BIT(cpu->regi.f, 7)
#define CARRY_FLAG BIT(cpu->regi.f, 4)
#define SUBTRACT_FLAG (1 << 6)
#define HALF_CARRY_FLAG (1 << 5)


#define SET_ZERO_FLAG(cpu) (cpu->regi.f |= (1 << 7))
#define CLEAR_ZERO_FLAG(cpu) (cpu->regi.f &= ~(1 << 7))
#define SET_CARRY_FLAG(cpu) (cpu->regi.f |= (1 << 4))
#define CLEAR_CARRY_FLAG(cpu) (cpu->regi.f &= ~(1 << 4))
#define SET_HALF_CARRY_FLAG(cpu) (cpu->regi.f |= (1 << 5))
#define CLEAR_HALF_CARRY_FLAG(cpu) (cpu->regi.f &= ~(1 << 5))
#define SET_SUBTRACT_FLAG(cpu) (cpu->regi.f |= (1 << 6))
#define CLEAR_SUBTRACT_FLAG(cpu) (cpu->regi.f &= ~(1 << 6))



int cpu_step(cpu_context *ctx);
cpu_context cpu_init();

// bus register funtions

u8 interrupt_flag_read(cpu_context *ctx);
u8 cpu_get_ie_register(cpu_context *ctx);
void interrupt_flag_write(cpu_context *ctx, u8 value);
void cpu_set_ie_register(cpu_context *ctx, u8 value);

#endif /* !CPU_H_ */
