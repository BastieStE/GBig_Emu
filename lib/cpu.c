#include <cpu.h>
#include <emu.h>

void cpu_init()
{
    return;
}

bool cpu_step(emu_context ctx)
{
    if (ctx.ticks > 5) {
        printf("need to stop %d\n", ctx.ticks);
        return false;
    }
    return true;
}