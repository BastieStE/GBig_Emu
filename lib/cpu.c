#include <cpu.h>
#include <orchestr.h>
#include <bus.h>


/*  
        Reactiver l'execution bios + MBC0
        Implémenter les IO du PPU
        Architecture générale du PPU
*/

void cpu_init(cpu_context *ctx, const char *bios_path) {

    // Set the program counter to the start of BIOS (0x0000)
    ctx->regi.pc = 0x0000;

    // Other initialization steps (set up registers, flags, etc.)
    ctx->cycles = 0;
    ctx->halted = false;
    ctx->stepping = false;
    ctx->IME = false;
    ctx->IME_delay = false;
    ctx->regi.ie_register = 0;
    ctx->regi.if_register = 0;
}

static void fetch_instruction(cpu_context *ctx) {
    ctx->cur_opcode = bus_read(ctx->regi.pc);
}

void handle_interrupt(cpu_context *ctx, uint8_t interrupts)
{
    NOT_YET
}

void fetch_data()
{
    NOT_YET
}

bool cpu_interrupt(cpu_context *ctx) 
{
    if (ctx->IME) {
        uint8_t interrupts = ctx->regi.ie_register & ctx->regi.if_register;
        if (interrupts) {
            handle_interrupt(ctx, interrupts);
            ctx->cycles += 20;
            return true;
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
        uint16_t pc = ctx->regi.pc;
        fetch_instruction(ctx);
        fetch_data(ctx);
        execute_instruction(ctx);
    }
    return true;
}

