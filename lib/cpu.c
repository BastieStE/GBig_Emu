#include <cpu.h>
#include <orchestr.h>
#include <bus.h>


/*
Terminer le membus avec gestion cartouche/bios, CPU, registres du CPU.
                done (I hope)

Terminer le MBC0 (no MBC).
        done (and even more)

Création d'un header .h pour chaque composant fonctionnel exposant les fonctions interfaces.
Cacher les fonctions privées.
*/

void cpu_init(cpu_context *ctx, const char *bios_path) 
{
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

static void handle_interrupt(cpu_context *ctx, uint8_t interrupts)
{
    NOT_YET
}

static void fetch_data()
{
    NOT_YET
}

static bool cpu_interrupt(cpu_context *ctx) 
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

