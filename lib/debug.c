#include <debug.h>

void print_cpu_registers(const cpu_context *ctx) {
    printf("=== CPU Registers ===\n");

    printf("AF: 0x%04X (A: 0x%02X, F: 0x%02X)\n", ctx->regi.af, ctx->regi.a, ctx->regi.f);
    printf("BC: 0x%04X (B: 0x%02X, C: 0x%02X)\n", ctx->regi.bc, ctx->regi.b, ctx->regi.c);
    printf("DE: 0x%04X (D: 0x%02X, E: 0x%02X)\n", ctx->regi.de, ctx->regi.d, ctx->regi.e);
    printf("HL: 0x%04X (H: 0x%02X, L: 0x%02X)\n", ctx->regi.hl, ctx->regi.h, ctx->regi.l);

    printf("PC: 0x%04X\n", ctx->regi.pc);
    printf("SP: 0x%04X\n", ctx->regi.sp);

    printf("IE Register:  0x%02X\n", ctx->regi.ie_register);
    printf("IF Register:  0x%02X\n", ctx->regi.if_register);

    printf("======================\n");
}


debug_ctx init_debug(int argc, char **argv, bus_context *bus_ctx, ppu_context *ppu_ctx, cpu_context *cpu_ctx)
{
    debug_ctx ctx = {
        .bus = bus_ctx,
        .ppu = ppu_ctx,
        .cpu = cpu_ctx
    };

    if (argc < 2) { ctx.is_on = false; return ctx;}

    if (argv[1][0] == '-' && argv[1][1] == 'd' && argv[1][2] == 's') {
        printf("debug mode activated\n");
        ctx.is_on = true;
        ctx.is_super = true;
    } else if (argv[1][0] == '-' && argv[1][1] == 'd') {
        printf("debug mode activated\n");
        ctx.is_on = true;
    }
    if (argc > 3)
        if ((ctx.speed = atoi(argv[2])) == 0)
            ctx.speed = 1;
    ctx.step = 0;
    return ctx;
}


int debug_step(debug_ctx *debug, const IO_register_t *io_regs)
{
    puts("debug : \n");
    if (debug->is_on == true) {
        debug->step++;
        if (debug->step >= debug->speed) {
            
            char cmd[5];      // pause and get command
            char c;
            for (long unsigned int i = 0; (c = getchar()) != '\n' && c != EOF && i < sizeof(cmd) - 1; i++) {
                cmd[i] = c;
            }
            
            if (cmd[0] == 's')  // exit debug
                debug->is_on = false;
            else if (cmd[0] == '+')
                debug->speed++; // incresse speed
            else if (cmd[0] == '-')
                debug->speed--; // decresse speed
            else if (cmd[0] == 'q')
                return 1; // leave loop
            debug->step = 0;

            print_cpu_registers(debug->cpu);
            printf("\n\n\nthe IO registers : \n\tjoypad hard: %02X\n\tjoypad joyp: %02X\n", io_regs->joypad.hard, io_regs->joypad.joyp);
            printf("bios register : %02X\n", debug->bus->bios_enabled);
        }
    }
    return 0;
}
