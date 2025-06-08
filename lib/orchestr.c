#include <stdlib.h>
#include <stdio.h>
#include <orchestr.h>
#include "wram.h"
#include <cpu.h>
#include <cart.h>
#include <cpu.h>
#include <ppu.h>

void init_debug(int argc, char **argv, debug_ctx *ctx)
{
    if (argc < 2) { ctx->is_on = false; return;}

    if (argv[1][0] == '-' && argv[1][1] == 'd') {
        printf("debug mode activated\n");
        ctx->is_on = true;
    }
    ctx->speed = 1;
    ctx->step = 0;
}


void print_cpu_registers(const cpu_registers regs) {
    printf("=== CPU Registers ===\n");

    printf("AF: 0x%04X (A: 0x%02X, F: 0x%02X)\n", regs.af, regs.a, regs.f);
    printf("BC: 0x%04X (B: 0x%02X, C: 0x%02X)\n", regs.bc, regs.b, regs.c);
    printf("DE: 0x%04X (D: 0x%02X, E: 0x%02X)\n", regs.de, regs.d, regs.e);
    printf("HL: 0x%04X (H: 0x%02X, L: 0x%02X)\n", regs.hl, regs.h, regs.l);

    printf("PC: 0x%04X\n", regs.pc);
    printf("SP: 0x%04X\n", regs.sp);

    printf("IE Register:  0x%02X\n", regs.ie_register);
    printf("IF Register:  0x%02X\n", regs.if_register);

    printf("======================\n");
}

int debug_step(debug_ctx *debug, cpu_context cpu_ctx)
{
    if (debug->is_on == true) {
        puts("debug :");
        debug->step++;
        if (debug->step >= debug->speed) {
            
            char cmd[5];
            char c;
            for (int i = 0; (c = getchar()) != '\n' && c != EOF && i < sizeof(cmd) - 1; i++) {
                cmd[i] = c;
            }
            
            if (cmd[0] == 's')
                debug->is_on = false;
            else if (cmd[0] == '+')
                debug->speed++;
            else if (cmd[0] == '-')
                debug->speed--;
            else if (cmd[0] == 'q')
                return 1;
            debug->step = 0;

            print_cpu_registers(cpu_ctx.regi);
        }
    }
    return 0;
}

int ctx_init(main_context *ctx)
{
    ctx->paused = false;
    ctx->running = true;
    ctx->ticks = 0;
}

int run(int argc, char **argv) {
    main_context ctx;
    cpu_context cpu_ctx;
    ppu_context ppu;  // Create an instance of your PPU
    debug_ctx debug;

    // Initialize the main context, CPU, WRAM, and PPU.
    cart_load("../Tetris.gb");
    init_debug(argc, argv, &debug);
    ctx_init(&ctx);
    cpu_init(&cpu_ctx);
    init_wram();
    ppu_init(&ppu); // Initialize your PPU state
    printf("end of init\n");

//    load_bios("../gbc_bios.bin");

    while(ctx.running) {
        printf("%d ticks\n", ctx.ticks);
        if (ctx.paused) {
            printf("pause not implemented\n");
        }

        // Execute the next CPU instruction and obtain the number of cycles taken.
        int cycles = cpu_step(&cpu_ctx);
        printf("step\n");

        if (cycles < 0) {
            printf("CPU Stopped\n");
            return -3;
        }

        // Update the PPU with the cycles executed by the CPU.
        updatePPU(&ppu, cycles);

        // You might also update other systems (timers, sound, etc.) here with the same cycle count.
        
        ctx.ticks++;
        if (debug_step(&debug, cpu_ctx)) break;
    }
    return 0;
}

int main(int argc, char **argv) {
    run(argc, argv);
    return 77;
}
