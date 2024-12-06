#include "cpu_LD_instructions.h"

void load_immediate(cpu_contex *cpu, uint8_t *dst) { // takes 8 cycles (to implement)
    u8 value = bus_read(cpu->regi.pc + 1);
    *dst = value;
    cpu->regi.pc += 2;
}

void load_register(cpu_contex *cpu, uint8_t *dst, uint8_t *src) { // takes 4 cycles (to implement)
    *dst = *src;
    cpu->regi.pc += 1;
}

void load_direct(cpu_contex *cpu, uint8_t *dst) { // takes 16 cycles (to implement)
    uint16_t address = bus_read16(cpu->regi.pc + 1);
    *dst = bus_read(address);
    cpu->regi.pc += 3;
}

void load_indirect(cpu_contex *cpu, uint8_t *dst, uint16_t *address_reg) { // takes 8 cycles (to implement)
    *dst = bus_read(*address_reg);
    cpu->regi.pc += 1;
}

void store_register(cpu_contex *cpu, uint8_t *dst, uint8_t *src) { // takes 4 cycles (to implement)
    *dst = *src;
    cpu->regi.pc += 1;
}

void store_direct(cpu_contex *cpu, uint8_t *src) { // takes 16 cycles (to implement)
    uint16_t address = bus_read16(cpu->regi.pc + 1);
    bus_write(address, *src);
    cpu->regi.pc += 3;
}

void store_indirect(cpu_contex *cpu, uint8_t *src, uint16_t *address_reg) { // takes 8 cycles (to implement)
    bus_write(*address_reg, *src);
    cpu->regi.pc += 1;
}
