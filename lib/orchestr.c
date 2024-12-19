#include <stdlib.h>
#include <stdio.h>
#include <orchestr.h>
#include <cpu.h>
#include <cart.h>
#include <cpu.h>
#include <wram.h>

int ctx_init(main_context *ctx)
{
    ctx->paused = false;
    ctx->running = true;
    ctx->ticks = 0;
}

int run(int argc, char **argv) {
    main_context ctx;
    cpu_context cpu_ctx;

    ctx_init(&ctx);
    cpu_init();
    init_wram();

    while(ctx.running) {
        printf("%d ticks\n", ctx.ticks);
        if (ctx.paused) {
            printf("pause not implemented\n");
        }
        if (!cpu_step(&cpu_ctx)) {
            printf("CPU Stopped\n");
            return -3;
        }
        ctx.ticks++;
    }
    return 0;
}

int main(int argc, char **argv) {
    run(argc, argv);
    return 77;
}
