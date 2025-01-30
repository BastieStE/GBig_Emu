#include "instruction.h"

void store_immediate(cpu_context *cpu) {
    uint8_t value = bus_read(cpu->regi.pc + 1);

    bus_write(cpu->regi.hl, value);

    cpu->regi.pc += 2;
    cpu->cycles += 12;
}

void store_direct_16(cpu_context *cpu, uint16_t *reg) {
    uint16_t address = bus_read16(cpu->regi.pc + 1);

    bus_write16(address, *reg); // Use bus_write16 to store both bytes

    cpu->regi.pc += 3;
    cpu->cycles += 20;
}

void ld_absolute(cpu_context *cpu) {
    uint16_t address = bus_read16(cpu->regi.pc + 1);

    cpu->regi.a = bus_read(address); // Use bus_read instead of direct memory access

    cpu->regi.pc += 3;
    cpu->cycles += 16;
}

void ld_absolute_store(cpu_context *cpu) {
    uint16_t address = bus_read16(cpu->regi.pc + 1);
    bus_write(address, cpu->regi.a);  // Store value from A into memory
    cpu->regi.pc += 3;
    cpu->cycles += 16;
}


void ld_sp_hl(cpu_context *cpu) {
    cpu->regi.hl = cpu->regi.sp;

    cpu->regi.pc += 1;
    cpu->cycles += 8;
}

void ld_io_offset_indirect(cpu_context *cpu) {
    uint16_t address = 0xFF00 + cpu->regi.c;

    cpu->regi.a = bus_read(address);

    cpu->regi.pc += 1;
    cpu->cycles += 8;
}

void ld_io_offset_indirect_store(cpu_context *cpu) {
    bus_write(0xFF00 + cpu->regi.c, cpu->regi.a);
    cpu->regi.pc += 1;
    cpu->cycles += 8;
}


void ld_io_offset(cpu_context *cpu) {
    uint16_t address = 0xFF00 + bus_read(cpu->regi.pc + 1);
    cpu->regi.a = bus_read(address);  // Load from memory into A
    cpu->regi.pc += 2;
    cpu->cycles += 12;
}

void ld_io_offset_store(cpu_context *cpu) {
    uint16_t address = 0xFF00 + bus_read(cpu->regi.pc + 1);
    bus_write(address, cpu->regi.a);  // Store A into memory
    cpu->regi.pc += 2;
    cpu->cycles += 12;
}


void ld_hl_sp_offset(cpu_context *cpu) {
    int8_t offset = (int8_t)bus_read(cpu->regi.pc + 1);
    uint16_t result = cpu->regi.sp + offset;

    CLEAR_ZERO_FLAG(cpu);
    CLEAR_SUBTRACT_FLAG(cpu);
    if (((cpu->regi.sp & 0xF) + (offset & 0xF)) > 0xF) SET_HALF_CARRY_FLAG(cpu);
    else CLEAR_HALF_CARRY_FLAG(cpu);
    if (((cpu->regi.sp & 0xFF) + (offset & 0xFF)) > 0xFF) SET_CARRY_FLAG(cpu);
    else CLEAR_CARRY_FLAG(cpu);

    cpu->regi.hl = result;

    cpu->regi.pc += 2;
    cpu->cycles += 12;
}


void add_sp_offset(cpu_context *cpu) {
    int8_t offset = (int8_t)bus_read(cpu->regi.pc + 1);
    uint16_t result = cpu->regi.sp + offset;

    CLEAR_ZERO_FLAG(cpu);
    CLEAR_SUBTRACT_FLAG(cpu);
    if (((cpu->regi.sp & 0xF) + (offset & 0xF)) > 0xF) SET_HALF_CARRY_FLAG(cpu);
    else CLEAR_HALF_CARRY_FLAG(cpu);
    if (((cpu->regi.sp & 0xFF) + (offset & 0xFF)) > 0xFF) SET_CARRY_FLAG(cpu);
    else CLEAR_CARRY_FLAG(cpu);

    cpu->regi.sp = result;

    cpu->regi.pc += 2;
    cpu->cycles += 16;
}

void add_16(cpu_context *cpu, uint16_t *reg) {
    uint32_t result = cpu->regi.hl + *reg;

    CLEAR_SUBTRACT_FLAG(cpu);
    if ((cpu->regi.hl & 0xFFF) + (*reg & 0xFFF) > 0xFFF) SET_HALF_CARRY_FLAG(cpu);
    else CLEAR_HALF_CARRY_FLAG(cpu);
    if (result > 0xFFFF) SET_CARRY_FLAG(cpu);
    else CLEAR_CARRY_FLAG(cpu);

    cpu->regi.hl = (uint16_t)result;

    cpu->regi.pc += 1;
    cpu->cycles += 8;
}
