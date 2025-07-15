#include <debug.h>

bool debug_breakpoint = false;

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

void print_framebuffer(const ppu_context *ctx) {
    if (!ctx) return;

    printf("[Framebuffer] %dx%d\n", SCREEN_WIDTH, SCREEN_HEIGHT);

    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            uint32_t pixel = ctx->framebuffer[y][x];  // ← 2D access

            // Basic brightness estimation from RGB
            uint8_t r = (pixel >> 16) & 0xFF;
            uint8_t g = (pixel >> 8) & 0xFF;
            uint8_t b = pixel & 0xFF;
            uint8_t brightness = (r + g + b) / 3;

            // Convert brightness to ASCII character
            char shade = '.';
            if (brightness > 200) shade = '#';
            else if (brightness > 128) shade = '0';
            else if (brightness > 64) shade = '*';
            else if (brightness > 16) shade = '+';

            printf("%c", shade);
        }
        printf("\n");
    }
}



const char* get_ppu_mode_name(PPU_Mode mode) {
    switch (mode) {
        case 0: return "HBlank";
        case 1: return "VBlank";
        case 2: return "OAM Search";
        case 3: return "Drawing";
        default: return "Unknown";
    }
}

void print_ppu_data(const ppu_context *ctx)
{
    printf("[PPU] Mode: %s (%d), LY: %3u, SCX: %3u, SCY: %3u, WX: %3u, WY: %3u, LYC: %3u\n",
        get_ppu_mode_name(ctx->mode), ctx->mode, ctx->LY, ctx->SCX, ctx->SCY, ctx->WX, ctx->WY, ctx->LYC);

    printf("[PPU] LCDC: 0x%02X, STAT: 0x%02X, BGP: 0x%02X, OBP0: 0x%02X, OBP1: 0x%02X\n",
        ctx->LCDC, ctx->STAT, ctx->BGP, ctx->OBP0, ctx->OBP1);

    printf("[PPU] VRAM Bank: %u, VBK: 0x%02X, BGPI: 0x%02X, BGPD: 0x%02X, OBPI: 0x%02X, OBPD: 0x%02X\n",
        ctx->current_vram_bank, ctx->VBK, ctx->BGPI, ctx->BGPD, ctx->OBPI, ctx->OBPD);

    printf("[PPU] Cycle Counter: %d\n", ctx->cycleCounter);

    printf("[PPU] Palette: ");
    for (int i = 0; i < 4; i++) {
        printf("0x%08X ", ctx->palette[i]);
    }
    printf("\n");
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
    if (argc >= 3)
        if ((ctx.speed = atoi(argv[2])) == 0)
            ctx.speed = 1;
    ctx.step = 0;
    return ctx;
}


int debug_step(debug_ctx *debug, const IO_register_t *io_regs)
{
    if (debug->is_on == true) {
        debug->step++;
        if (debug->step < debug->speed && debug->is_super == false) {
            return 0;
        } else if (debug->is_super == true && debug_breakpoint == false) {
            if (debug->cpu->cycles <= 0)
                print_cpu_registers(debug->cpu);
            return 0;
        }
        // if (debug->is_super == true && debug->cpu->cur_opcode != 0xFF50) { return 0;} // the is super is a debug mode that allows to wait for a specific register to be modified to something expected
        char cmd[5];      // pause and get command
        char c;
        for (long unsigned int i = 0; (c = getchar()) != '\n' && c != EOF && i < sizeof(cmd) - 1; i++) {
            cmd[i] = c;
        }
        printf("the command was : %s\n", cmd);
        if (cmd[0] == 'g' && cmd[1] == 'o')  // exit debug
            debug->is_on = false;
        else if (cmd[0] == 'h' && cmd[1] == 'e' && cmd[2] == 'l' && cmd[3] == 'p') {
            printf("help ? \n");
            debug_step(debug, io_regs);
        } else if (cmd[0] == '+')
            debug->speed++; // incresse speed
        else if (cmd[0] == '-') {
            debug->speed--; // decresse speed
        } else if (cmd[0] == 'q') {
            return 1; // leave loop
        } else if (cmd[0] == 's') {
            if (debug->is_super == true){debug->is_super = false;puts("SUPER IS ENABLED");}
            else{debug->is_super = true; puts("SUPER IS ENABLED");}
        } else {
            printf("debug every : %d\n", debug->speed);
            if (debug->cpu->cycles <= 0){
                print_cpu_registers(debug->cpu);
                print_ppu_data(debug->ppu);
        }
            printf("\n\n\nthe IO registers : \n\tjoypad hard: %02X\n\tjoypad joyp: %02X\n", io_regs->joypad.hard, io_regs->joypad.joyp);
            printf("bios register : %02X\n", debug->bus->bios_enabled);
        }
        debug->step = 0;
        debug_breakpoint = false;
    }
    return 0;
}
