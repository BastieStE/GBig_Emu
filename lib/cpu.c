#include <cpu.h>
#include <orchestr.h>


void cpu_init(cpu_context *ctx, const char *bios_path) {
    // Load the BIOS using the bus
    load_bios(bios_path);

    // Set the program counter to the start of BIOS (0x0000)
    ctx->regi.pc = 0x0000;

    // Other initialization steps (set up registers, flags, etc.)
    ctx->cycles = 0;
    ctx->halted = false;
    ctx->stepping = false;
    ctx->IME = false;
    ctx->IME_delay = false;
    ctx->ie_register = 0;
    ctx->if_register = 0;
}

static void fetch_instruction(cpu_context *ctx) {
    ctx->cur_opcode = bus_read(ctx->regi.pc);
}

void fetch_data()
{
    NOT_YET
}

bool cpu_interrupt(cpu_context *ctx) 
{
    if (ctx->IME) {
        uint8_t interrupts = ctx->ie_register & ctx->if_register;
        if (interrupts) {
            handle_interrupt(ctx, interrupts);
            ctx->cycles += 20;
            return true;
        }
    } else if (ctx->halted) {
        if (ctx->if_register & ctx->ie_register) {
            ctx->halted = false;
        }
        return true;
    }
    if (ctx->IME_delay) {
        ctx->IME_delay = false;
        ctx->IME = true;
    }
    return false;
}

bool cpu_step(cpu_context *ctx) 
{
    if (ctx->cycles > 0) {ctx->cycles--;return true;}

    if (!cpu_interrupt(ctx)) {
        uint16_t pc = ctx->regi.pc;
        fetch_instruction(ctx);
        fetch_data(ctx);
        execute_instruction(ctx);
    }
    return true;
}


u16 cpu_read_reg(reg_type rt, cpu_context *ctx) {
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

void cpu_set_reg(reg_type rt, u16 val, cpu_context *ctx) {
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