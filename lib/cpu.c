#include <cpu.h>
#include <orchestr.h>
#include <instruction.h>
#include <bus.h>

/*
Implémenter les IO de DMG (sauf Audio/wave pattern, serial transfer) :

https://gbdev.io/pandocs/Memory_Map.html#io-ranges

Implémenter les interrupts dans le CPU.
*/

cpu_context cpu_init() 
{
    cpu_context ctx;
    ctx.regi.pc = 0x0000;

    // Other initialization steps (set up registers, flags, etc.)
    
    ctx.cycles = 0;
    ctx.halted = false;
    ctx.stepping = false;
    ctx.IME = false;
    ctx.IME_delay = false;
    ctx.regi.ie_register = 0;
    ctx.regi.if_register = 0;
    return ctx;
}

static void fetch_instruction(cpu_context *ctx) {
    ctx->cur_opcode = bus_read(ctx->regi.pc);
}

static bool cpu_interrupt(cpu_context *ctx) 
{
    uint8_t interrupt_flags = ctx->regi.if_register;
    uint8_t interrupt_enable = ctx->regi.ie_register;

    uint8_t pending = interrupt_flags & interrupt_enable;

    if (ctx->IME && pending) {
        // Find the highest priority interrupt
        for (int i = 0; i < 5; i++) {
            if (pending & (1 << i)) {
                // Clear IF bit
                ctx->regi.if_register &= ~(1 << i);

                // Push PC to stack
                ctx->regi.sp -= 2;
                bus_write(ctx->regi.sp, ctx->regi.pc & 0xFF);
                bus_write(ctx->regi.sp + 1, (ctx->regi.pc >> 8) & 0xFF);

                // Jump to interrupt vector
                switch (i) {
                    case 0: ctx->regi.pc = 0x40; break; // V-Blank
                    case 1: ctx->regi.pc = 0x48; break; // LCD STAT
                    case 2: ctx->regi.pc = 0x50; break; // Timer
                    case 3: ctx->regi.pc = 0x58; break; // Serial
                    case 4: ctx->regi.pc = 0x60; break; // Joypad
                }

                ctx->IME = false;        // Disable further interrupts
                ctx->halted = false;     // Exit HALT state if needed
                ctx->cycles += 20;       // Interrupt handling cycle cost

                return true;
            }
        }
    } else if (ctx->halted) {
        if (ctx->regi.if_register & ctx->regi.ie_register) {
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

        fetch_instruction(ctx);
        execute_instruction(ctx);
    }
    return true;
}



u8 interrupt_flag_read(cpu_context *ctx)
{
    return ctx->regi.if_register;
}

u8 cpu_get_ie_register(cpu_context *ctx)
{
    return ctx->regi.ie_register;
}

void interrupt_flag_write(cpu_context *ctx, u8 value)
{
    ctx->regi.if_register = value;
}

void cpu_set_ie_register(cpu_context *ctx, u8 value)
{
    ctx->regi.ie_register = value;
}
