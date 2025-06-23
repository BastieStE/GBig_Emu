#include <cpu.h>
#include <orchestr.h>
#include <bus.h>

static cpu_context ctx;

/*
Implémenter les IO de DMG (sauf Audio/wave pattern, serial transfer) :

https://gbdev.io/pandocs/Memory_Map.html#io-ranges

Implémenter les interrupts dans le CPU.
*/

void print_cpu_registers() {
    printf("=== CPU Registers ===\n");

    printf("AF: 0x%04X (A: 0x%02X, F: 0x%02X)\n", ctx.regi.af, ctx.regi.a, ctx.regi.f);
    printf("BC: 0x%04X (B: 0x%02X, C: 0x%02X)\n", ctx.regi.bc, ctx.regi.b, ctx.regi.c);
    printf("DE: 0x%04X (D: 0x%02X, E: 0x%02X)\n", ctx.regi.de, ctx.regi.d, ctx.regi.e);
    printf("HL: 0x%04X (H: 0x%02X, L: 0x%02X)\n", ctx.regi.hl, ctx.regi.h, ctx.regi.l);

    printf("PC: 0x%04X\n", ctx.regi.pc);
    printf("SP: 0x%04X\n", ctx.regi.sp);

    printf("IE Register:  0x%02X\n", ctx.regi.ie_register);
    printf("IF Register:  0x%02X\n", ctx.regi.if_register);

    printf("======================\n");
}

void cpu_init() 
{
    ctx.regi.pc = 0x0000;

    // Other initialization steps (set up registers, flags, etc.)
    
    ctx.cycles = 0;
    ctx.halted = false;
    ctx.stepping = false;
    ctx.IME = false;
    ctx.IME_delay = false;
    ctx.regi.ie_register = 0;
    ctx.regi.if_register = 0;
}

static void fetch_instruction() {
    ctx.cur_opcode = bus_read(ctx.regi.pc);
}

static void handle_interrupt(uint8_t interrupts)
{
    NOT_YET
}

static void fetch_data()
{
    NOT_YET
}

static bool cpu_interrupt() 
{
     uint8_t interrupt_flags = ctx.regi.if_register;
    uint8_t interrupt_enable = ctx.regi.ie_register;

    uint8_t pending = interrupt_flags & interrupt_enable;

    if (ctx.IME && pending) {
        // Find the highest priority interrupt
        for (int i = 0; i < 5; i++) {
            if (pending & (1 << i)) {
                // Clear IF bit
                ctx.regi.if_register &= ~(1 << i);

                // Push PC to stack
                ctx.regi.sp -= 2;
                bus_write(ctx.regi.sp, ctx.regi.pc & 0xFF);
                bus_write(ctx.regi.sp + 1, (ctx.regi.pc >> 8) & 0xFF);

                // Jump to interrupt vector
                switch (i) {
                    case 0: ctx.regi.pc = 0x40; break; // V-Blank
                    case 1: ctx.regi.pc = 0x48; break; // LCD STAT
                    case 2: ctx.regi.pc = 0x50; break; // Timer
                    case 3: ctx.regi.pc = 0x58; break; // Serial
                    case 4: ctx.regi.pc = 0x60; break; // Joypad
                }

                ctx.IME = false;        // Disable further interrupts
                ctx.halted = false;     // Exit HALT state if needed
                ctx.cycles += 20;       // Interrupt handling cycle cost

                return true;
            }
        }
    } else if (ctx.halted) {
        if (ctx.regi.if_register & ctx.regi.ie_register) {
            ctx.halted = false;
        }
        return true;
    }
    if (ctx.IME_delay) {
        ctx.IME_delay = false;
        ctx.IME = true;
    }
    return false;
}

bool cpu_step() 
{


    if (ctx.cycles > 0) {ctx.cycles--;return true;}

    if (!cpu_interrupt(ctx)) {

        uint16_t pc = ctx.regi.pc;
        fetch_instruction(ctx);



    fetch_data(ctx);

    execute_instruction(&ctx);

    }
    return true;
}



u8 interrupt_flag_read()
{
    return ctx.regi.if_register;
}

u8 cpu_get_ie_register()
{
    return ctx.regi.ie_register;
}

void interrupt_flag_write(u8 value)
{
    ctx.regi.if_register = value;
}

void cpu_set_ie_register(value)
{
    ctx.regi.ie_register = value;
}
