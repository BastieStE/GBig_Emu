#include <cpu.h>
#include <orchestr.h>

void cpu_init(cpu_contex *ctx) {
    ctx->regi.pc = 0x100;
    ctx->regi.a = 0x01;
    ctx->IME = false;
}

static void fetch_instruction(cpu_contex *ctx) {
    ctx->cur_opcode = bus_read(ctx->regi.pc);
}

void fetch_data()
{
    NOT_YET
}

bool cpu_step(cpu_contex *ctx) {
    if (!ctx->halted) {
        u16 pc = ctx->regi.pc;

        fetch_instruction(ctx);
        fetch_data();

        execute_instruction(ctx);
    }

    return true;
}

u16 cpu_read_reg(reg_type rt, cpu_contex *ctx) {      // /!\  16 bit a implémenter
    switch(rt) {
        case RT_A: return ctx->regi.a;
        case RT_F: return ctx->regi.f;
        case RT_B: return ctx->regi.b;
        case RT_C: return ctx->regi.c;
        case RT_D: return ctx->regi.d;
        case RT_E: return ctx->regi.e;
        case RT_H: return ctx->regi.h;
        case RT_L: return ctx->regi.l;
        default: return 0;
    }
}

void cpu_set_reg(reg_type rt, u16 val, cpu_contex *ctx) {  // /!\ 16 bit a implémenter
    switch(rt) {
        case RT_A: ctx->regi.a = val & 0xFF; break;
        case RT_F: ctx->regi.f = val & 0xFF; break;
        case RT_B: ctx->regi.b = val & 0xFF; break;
        case RT_C: ctx->regi.c = val & 0xFF; break;
        case RT_D: ctx->regi.d = val & 0xFF; break;
        case RT_E: ctx->regi.e = val & 0xFF; break;
        case RT_H: ctx->regi.h = val & 0xFF; break;
        case RT_L: ctx->regi.l = val & 0xFF; break;
        }
}