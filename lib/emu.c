#include <stdlib.h>
#include <stdio.h>
#include <emu.h>
#include <cpu.h>
#include <cart.h>
#include <cpu.h>

int ctx_init(emu_context *ctx)
{
    ctx->paused = false;
    ctx->running = true;
    ctx->ticks = 0;
}

int emu_run(int argc, char **argv) {
    emu_context ctx;
    ctx_init(&ctx);
    cpu_init();
    
    ctx.running = true;
    ctx.paused = false;
    ctx.ticks = 0;

    while(ctx.running) {
        printf("%d ticks\n", ctx.ticks);
        if (ctx.paused) {
            printf("pause not implemented\n");
        }

        if (!cpu_step(ctx)) {
            printf("CPU Stopped\n");
            return -3;
        }
        ctx.ticks++;
    }

    return 0;
}

int main(int argc, char **argv) {
    emu_run(argc, argv);
    return 77;
}
