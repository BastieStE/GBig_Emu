#include "instruction.h"

void increment_indirect(cpu_context *cpu) {
    uint8_t value_at_hl = bus_read(cpu->regi.hl);
    uint8_t result = value_at_hl + 1;

    if (result == 0) {
        SET_ZERO_FLAG(cpu);
    } else {
        CLEAR_ZERO_FLAG(cpu);
    }

    if ((value_at_hl & 0xF) == 0xF) {
        SET_HALF_CARRY_FLAG(cpu);
    } else {
        CLEAR_HALF_CARRY_FLAG(cpu);
    }

    CLEAR_SUBTRACT_FLAG(cpu);

   bus_write(cpu->regi.hl, result);

    cpu->regi.pc += 1;
    INCR_CYCLE(12);
}

void increment_16(cpu_context *cpu, uint16_t *reg) {
    (*reg)++;
    cpu->regi.pc += 1;
    INCR_CYCLE(8);
}

void increment(cpu_context *cpu, uint8_t *reg) {
    uint8_t result = *reg + 1;

    if (result == 0) {
        SET_ZERO_FLAG(cpu);
    } else {
        CLEAR_ZERO_FLAG(cpu);
    }

    if ((*reg & 0xF) == 0xF) {
        SET_HALF_CARRY_FLAG(cpu);
    } else {
        CLEAR_HALF_CARRY_FLAG(cpu);
    }

    CLEAR_SUBTRACT_FLAG(cpu);

    *reg = result;

    cpu->regi.pc += 1;
    INCR_CYCLE(4);
}

void decrement_indirect(cpu_context *cpu) {
    uint8_t value_at_hl = bus_read(cpu->regi.hl);
    uint8_t result = value_at_hl - 1;

    if (result == 0) {
        SET_ZERO_FLAG(cpu);
    } else {
        CLEAR_ZERO_FLAG(cpu);
    }

    if ((value_at_hl & 0xF) == 0x0) {
        SET_HALF_CARRY_FLAG(cpu);
    } else {
        CLEAR_HALF_CARRY_FLAG(cpu);
    }

    SET_SUBTRACT_FLAG(cpu);

    bus_write(cpu->regi.hl, result);

    cpu->regi.pc += 1;
    INCR_CYCLE(12);
}

void decrement_16(cpu_context *cpu, uint16_t *reg) {
    (*reg)--;
    cpu->regi.pc += 1;
    INCR_CYCLE(8);
}

void decrement(cpu_context *cpu, uint8_t *reg) {
    uint8_t result = *reg - 1;

    if (result == 0) {
        SET_ZERO_FLAG(cpu);
    } else {
        CLEAR_ZERO_FLAG(cpu);
    }

    if ((*reg & 0xF) == 0) {
        SET_HALF_CARRY_FLAG(cpu);
    } else {
        CLEAR_HALF_CARRY_FLAG(cpu);
    }

    SET_SUBTRACT_FLAG(cpu);

    *reg = result;

    cpu->regi.pc += 1;
    INCR_CYCLE(4);
}
