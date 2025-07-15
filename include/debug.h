#ifndef DEBUG_H_
#define DEBUG_H_

#include <common.h>
#include <cpu.h>
#include <ppu.h>
#include <bus.h>

typedef struct {
    int speed;
    int step;
    bool is_on;
    bool is_display_register;
    bool is_super;
    const bus_context * const bus;
    const cpu_context * const cpu;
    const ppu_context * const ppu;
} debug_ctx;

extern bool debug_breakpoint;

void print_cpu_registers();
debug_ctx init_debug(int argc, char **argv, bus_context *bus_ctx, ppu_context *ppu_ctx, cpu_context *cpu_ctx);
int debug_step(debug_ctx *debug, const IO_register_t *io_regs);

#endif /* !DEBUG_H_ */