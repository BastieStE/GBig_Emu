#include <stdlib.h>
#include <stdio.h>
#include <orchestr.h>
#include "wram.h"
#include <cpu.h>
#include <cart.h>
#include <cpu.h>
#include <ppu.h>

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

    // Initialize the main context, CPU, WRAM, and PPU.
    //cart_load("loremIpsum")
    ctx_init(&ctx);
    cpu_init(&cpu_ctx);
    init_wram();
    ppu_init(&ppu); // Initialize your PPU state

//    load_bios("../gbc_bios.bin");

    while(ctx.running) {
        printf("%d ticks\n", ctx.ticks);
        if (ctx.paused) {
            printf("pause not implemented\n");
        }

        // Execute the next CPU instruction and obtain the number of cycles taken.
        int cycles = cpu_step(&cpu_ctx);
        if (cycles < 0) {
            printf("CPU Stopped\n");
            return -3;
        }

        // Update the PPU with the cycles executed by the CPU.
        updatePPU(&ppu, cycles);

        // You might also update other systems (timers, sound, etc.) here with the same cycle count.
        
        ctx.ticks++;
    }
    return 0;
}

int main(int argc, char **argv) {
    run(argc, argv);
    return 77;
}
