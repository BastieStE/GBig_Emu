#include <stdlib.h>
#include <stdio.h>
#include <orchestr.h>
#include <wram.h>
#include <cpu.h>
#include <cart.h>
#include <cpu.h>
#include <ppu.h>

static void init_debug(int argc, char **argv, debug_ctx *ctx)
{
    if (argc < 2) { ctx->is_on = false; return;}

    if (argv[1][0] == '-' && argv[1][1] == 'd') {
        printf("debug mode activated\n");
        ctx->is_on = true;
    }
    if (argc > 3)
        if(ctx->speed = atoi(argv[2]) == 0)
            ctx->speed = 1;
    ctx->step = 0;
}


static int debug_step(debug_ctx *debug, IO_register_t *io_regs)
{
    if (debug->is_on == true) {
        puts("debug :");
        debug->step++;
        if (debug->step >= debug->speed) {
            
            char cmd[5];      // pause and get command
            char c;
            for (int i = 0; (c = getchar()) != '\n' && c != EOF && i < sizeof(cmd) - 1; i++) {
                cmd[i] = c;
            }
            poll_sdl_input(io_regs->joypad);
            
            if (cmd[0] == 's')  // exit debug
                debug->is_on = false;
            else if (cmd[0] == '+')
                debug->speed++; // incresse speed
            else if (cmd[0] == '-')
                debug->speed--; // decresse speed
            else if (cmd[0] == 'q')
                return 1; // leave loop
            debug->step = 0;

            print_cpu_registers();
            printf("the IO registers : \n\tjoypad hard : %02X\n\tjoypad joyp:%02X\n", io_regs->joypad.hard, io_regs->joypad.joyp);
        }
    }
    return 0;
}

static int ctx_init(main_context *ctx)
{
    ctx->paused = false;
    ctx->running = true;
    ctx->ticks = 0;
}

int run(int argc, char **argv) {
    main_context ctx;
    cpu_context ;
    ppu_context ppu;  // Create an instance of your PPU
    debug_ctx debug;
    IO_register_t io_regs;

    // Initialize the main context, CPU, WRAM, and PPU.
    cart_load("../Tetris.gb");
    init_debug(argc, argv, &debug);
    ctx_init(&ctx);
    cpu_init();
    bus_init(&io_regs);
    ppu_init(&ppu); // Initialize your PPU state
    printf("end of init\n");

//    load_bios("../gbc_bios.bin");

    while(ctx.running) {
        printf("%d ticks\n", ctx.ticks);
        if (ctx.paused) {
            printf("pause not implemented\n");
        }

        // Execute the next CPU instruction and obtain the number of cycles taken.
        int cycles = cpu_step();
        printf("step\n");

        if (cycles < 0) {
            printf("CPU Stopped\n");
            return -3;
        }

        // Update the PPU with the cycles executed by the CPU.
        updatePPU(&ppu, cycles);

        if (debug.is_on == false)
            poll_sdl_input(&io_regs.joypad);
        // update other systems (timers, sound, etc.) here with the same cycle count.
        
        ctx.ticks++;
        if (debug_step(&debug, &io_regs)) break;
    }
    return 0;
}

int main(int argc, char **argv) {
    run(argc, argv);
    return 77;
}
