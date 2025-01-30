#include "instruction.h"

void stop(cpu_context *cpu) {
    cpu->halted = true;
    cpu->regi.pc += 1;
}

void decimal_adjust(cpu_context *cpu) {
    if (!(cpu->regi.f & SUBTRACT_FLAG)) {
        if ((cpu->regi.f & CARRY_FLAG) || cpu->regi.a > 0x99) {
            cpu->regi.a += 0x60;
            SET_CARRY_FLAG(cpu);
        }
        if ((cpu->regi.f & HALF_CARRY_FLAG) || (cpu->regi.a & 0x0F) > 0x09) {
            cpu->regi.a += 0x06;
        }
    } else {
        if (cpu->regi.f & CARRY_FLAG) {
            cpu->regi.a -= 0x60;
        }
        if (cpu->regi.f & HALF_CARRY_FLAG) {
            cpu->regi.a -= 0x06;
        }
    }
    if (cpu->regi.a == 0) SET_ZERO_FLAG(cpu);
    else CLEAR_ZERO_FLAG(cpu);
    CLEAR_HALF_CARRY_FLAG(cpu);
    cpu->regi.pc += 1;
    cpu->cycles += 4;
}

void complement(cpu_context *cpu) {
    cpu->regi.a = ~cpu->regi.a;
    SET_SUBTRACT_FLAG(cpu);
    SET_HALF_CARRY_FLAG(cpu);
    cpu->regi.pc += 1;
    cpu->cycles += 4;
}

void set_carry(cpu_context *cpu) {
    SET_CARRY_FLAG(cpu);
    CLEAR_SUBTRACT_FLAG(cpu);
    CLEAR_HALF_CARRY_FLAG(cpu);
    cpu->regi.pc += 1;
    cpu->cycles += 4;
}

void complement_carry(cpu_context *cpu) {
    if (cpu->regi.f & CARRY_FLAG) {
        CLEAR_CARRY_FLAG(cpu);
    } else {
        SET_CARRY_FLAG(cpu);
    }
    CLEAR_SUBTRACT_FLAG(cpu);
    CLEAR_HALF_CARRY_FLAG(cpu);
    cpu->regi.pc += 1;
    cpu->cycles += 4;
}

void halt(cpu_context *cpu) {
    cpu->halted = true;
    cpu->regi.pc += 1;
}

void ret_cond(cpu_context *cpu, bool condition) {
    if (condition) {
        cpu->regi.pc = bus_read16(cpu->regi.sp);
        cpu->regi.sp += 2;
        cpu->cycles += 20;
    } else {
        cpu->regi.pc += 1;
        cpu->cycles += 8;
    }
}

void pop_register_pair(cpu_context *cpu, uint16_t *reg) {
    *reg = bus_read16(cpu->regi.sp);
    cpu->regi.sp += 2;
    cpu->regi.pc += 1;
    cpu->cycles += 12;
}

void call_cond(cpu_context *cpu, bool condition) {
    if (condition) {
        uint16_t address = bus_read16(cpu->regi.pc + 1);
        cpu->regi.sp -= 2;
        bus_write16(cpu->regi.sp, cpu->regi.pc + 3);
        cpu->regi.pc = address;
        cpu->cycles += 24;
    } else {
        cpu->regi.pc += 3;
        cpu->cycles += 12;
    }
}

void push_register_pair(cpu_context *cpu, uint16_t reg) {
    cpu->regi.sp -= 2;
    bus_write16(cpu->regi.sp, reg);
    cpu->regi.pc += 1;
    cpu->cycles += 16;
}

void restart(cpu_context *cpu, uint8_t address) {
    cpu->regi.sp -= 2;
    bus_write16(cpu->regi.sp, cpu->regi.pc + 1);
    cpu->regi.pc = address;
    cpu->cycles += 16;
}

void ret(cpu_context *cpu) {
    cpu->regi.pc = bus_read16(cpu->regi.sp);
    cpu->regi.sp += 2;
    cpu->cycles += 16;
}

void execute_cb_instruction(cpu_context *cpu) {
    uint8_t opcode = bus_read(cpu->regi.pc + 1);
    // Implement CB instruction decoding and execution
    cpu->regi.pc += 2;
    cpu->cycles += 8;
}

void call(cpu_context *cpu) {
    uint16_t address = bus_read16(cpu->regi.pc + 1);
    cpu->regi.sp -= 2;
    bus_write16(cpu->regi.sp, cpu->regi.pc + 3);
    cpu->regi.pc = address;
    cpu->cycles += 24;
}

void invalid_opcode(cpu_context *cpu) {
    // Handle an invalid opcode gracefully (could be a no-op or crash handling)
    cpu->regi.pc += 1;
}

void reti(cpu_context *cpu) {
    cpu->regi.pc = bus_read16(cpu->regi.sp);
    cpu->regi.sp += 2;
    cpu->IME = true;
    cpu->cycles += 16;
}

void or_immediate(cpu_context *cpu) {
    cpu->regi.a |= bus_read(cpu->regi.pc + 1);
    if (cpu->regi.a == 0) SET_ZERO_FLAG(cpu);
    else CLEAR_ZERO_FLAG(cpu);
    CLEAR_CARRY_FLAG(cpu);
    CLEAR_HALF_CARRY_FLAG(cpu);
    CLEAR_SUBTRACT_FLAG(cpu);
    cpu->regi.pc += 2;
    cpu->cycles += 8;
}

void enable_interrupts(cpu_context *cpu) {
    cpu->IME = true;
    cpu->regi.pc += 1;
    cpu->cycles += 4;
}

void disable_interrupts(cpu_context *cpu) {
    cpu->IME = false;
    cpu->regi.pc += 1;
    cpu->cycles += 4;
}
