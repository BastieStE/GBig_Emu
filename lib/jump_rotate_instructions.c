#include "instruction.h"

void jump_indirect(cpu_context *cpu) {
    cpu->regi.pc = cpu->regi.hl;  // Jump to the address stored in HL
    cpu->cycles += 4;              // JP (HL) takes 4 cycles
}

void jump_cond(cpu_context *cpu, bool condition) {
    uint16_t address = bus_read(cpu->regi.pc + 1) | (bus_read(cpu->regi.pc + 2) << 8);
    
    if (condition) {
        cpu->regi.pc = address;
        cpu->cycles += 16;  // JP takes 16 cycles if taken
    } else {
        cpu->regi.pc += 3;  // Skip operand bytes if not taken
        cpu->cycles += 12;  // JP takes 12 cycles if not taken
    }
}

void jump_relative_cond(cpu_context *cpu, bool condition) {
    int8_t offset = (int8_t)bus_read(cpu->regi.pc + 1);

    if (condition) {
        cpu->regi.pc += offset + 2; // Offset is relative to next instruction
        cpu->cycles += 12;  // JR takes 12 cycles if taken
    } else {
        cpu->regi.pc += 2;  // Skip offset byte if not taken
        cpu->cycles += 8;   // JR takes 8 cycles if not taken
    }
}

void jump_relative(cpu_context *cpu) {
    int8_t offset = (int8_t)bus_read(cpu->regi.pc + 1);
    
    cpu->regi.pc += offset + 2;  // Offset is relative to the next instruction
    cpu->cycles += 12;           // JR takes 12 cycles
}

void jump(cpu_context *cpu) {
    uint16_t address = bus_read(cpu->regi.pc + 1) | (bus_read(cpu->regi.pc + 2) << 8);
    
    cpu->regi.pc = address;  // Jump to the address
    cpu->cycles += 16;       // JP nn takes 16 cycles
}

void rotate_left_carry(cpu_context *cpu, uint8_t *reg) {
    uint8_t carry_out = (*reg & 0x80) ? 1 : 0;  // Get highest bit (bit 7)

    *reg = (*reg << 1) | carry_out;  // Shift left and wrap bit 7 to bit 0

    if (*reg == 0) { SET_ZERO_FLAG(cpu); } else { CLEAR_ZERO_FLAG(cpu); }
    if (carry_out) { SET_CARRY_FLAG(cpu); } else { CLEAR_CARRY_FLAG(cpu); }
    CLEAR_SUBTRACT_FLAG(cpu);
    CLEAR_HALF_CARRY_FLAG(cpu);

    cpu->regi.pc += 1;
    cpu->cycles += 8;
}

void rotate_right_carry(cpu_context *cpu, uint8_t *reg) {
    uint8_t carry_out = (*reg & 0x01) ? 1 : 0;  // Get lowest bit (bit 0)

    *reg = (*reg >> 1) | (carry_out << 7);  // Shift right and wrap bit 0 to bit 7

    if (*reg == 0) { SET_ZERO_FLAG(cpu); } else { CLEAR_ZERO_FLAG(cpu); }
    if (carry_out) { SET_CARRY_FLAG(cpu); } else { CLEAR_CARRY_FLAG(cpu); }
    CLEAR_SUBTRACT_FLAG(cpu);
    CLEAR_HALF_CARRY_FLAG(cpu);

    cpu->regi.pc += 1;
    cpu->cycles += 8;
}

void rotate_left(cpu_context *cpu, uint8_t *reg) {
    uint8_t carry_in = (cpu->regi.f & (1 << 4)) ? 1 : 0;  // Get carry flag
    uint8_t carry_out = (*reg & 0x80) ? 1 : 0;  // Get highest bit (bit 7)

    *reg = (*reg << 1) | carry_in;  // Shift left and insert carry flag into bit 0

    if (*reg == 0) { SET_ZERO_FLAG(cpu); } else { CLEAR_ZERO_FLAG(cpu); }
    if (carry_out) { SET_CARRY_FLAG(cpu); } else { CLEAR_CARRY_FLAG(cpu); }
    CLEAR_SUBTRACT_FLAG(cpu);
    CLEAR_HALF_CARRY_FLAG(cpu);

    cpu->regi.pc += 1;
    cpu->cycles += 8;
}

void rotate_right(cpu_context *cpu, uint8_t *reg) {
    uint8_t carry_in = (cpu->regi.f & (1 << 4)) ? 1 : 0;  // Get carry flag
    uint8_t carry_out = (*reg & 0x01) ? 1 : 0;  // Get lowest bit (bit 0)

    *reg = (*reg >> 1) | (carry_in << 7);  // Shift right and insert carry flag into bit 7

    if (*reg == 0) { SET_ZERO_FLAG(cpu); } else { CLEAR_ZERO_FLAG(cpu); }
    if (carry_out) { SET_CARRY_FLAG(cpu); } else { CLEAR_CARRY_FLAG(cpu); }
    CLEAR_SUBTRACT_FLAG(cpu);
    CLEAR_HALF_CARRY_FLAG(cpu);

    cpu->regi.pc += 1;
    cpu->cycles += 8;
}