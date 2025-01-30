#include "instruction.h"

void load_immediate(cpu_context *cpu, uint8_t *dst) {
    u8 value = bus_read(cpu->regi.pc + 1);
    *dst = value;
    cpu->regi.pc += 2;
    INCR_CYCLE(8);
}

void load_immediate_16(cpu_context *cpu, uint16_t *dst) {
    uint16_t value = bus_read(cpu->regi.pc + 1) | (bus_read(cpu->regi.pc + 2) << 8);
    *dst = value;
    cpu->regi.pc += 3;
    INCR_CYCLE(12);
}

void load_register(cpu_context *cpu, uint8_t *dst, uint8_t *src) {
    *dst = *src;
    cpu->regi.pc += 1;
    INCR_CYCLE(4);
}

void load_direct(cpu_context *cpu, uint8_t *dst) {
    uint16_t address = bus_read16(cpu->regi.pc + 1);
    *dst = bus_read(address);
    cpu->regi.pc += 3;
    INCR_CYCLE(16);
}

void load_indirect(cpu_context *cpu, uint8_t *dst, uint16_t *address_reg) {
    *dst = bus_read(*address_reg);
    cpu->regi.pc += 1;
    INCR_CYCLE(8);
}

void store_register(cpu_context *cpu, uint8_t *dst, uint8_t *src) {
    *dst = *src;
    cpu->regi.pc += 1;
    INCR_CYCLE(4);
}

void store_direct(cpu_context *cpu, uint8_t *src) {
    uint16_t address = bus_read16(cpu->regi.pc + 1);
    bus_write(address, *src);
    cpu->regi.pc += 3;
    INCR_CYCLE(16);
}

void store_indirect(cpu_context *cpu, uint8_t *src, uint16_t *address_reg) {
    bus_write(*address_reg, *src);
    cpu->regi.pc += 1;
    INCR_CYCLE(8);
}
