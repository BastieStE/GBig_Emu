#include "instruction.h"

void add_register(cpu_context *cpu, uint8_t *dst, uint8_t *src) {
    uint16_t result = *dst + *src;

    if ((uint8_t)result == 0) {
        SET_ZERO_FLAG(cpu);
    } else {
        CLEAR_ZERO_FLAG(cpu);
    }

    if (result > 0xFF) {
        SET_CARRY_FLAG(cpu);
    } else {
        CLEAR_CARRY_FLAG(cpu);
    }

    if (((*dst & 0xF) + (*src & 0xF)) > 0xF) {
        SET_HALF_CARRY_FLAG(cpu);
    } else {
        CLEAR_HALF_CARRY_FLAG(cpu);
    }

    *dst = (uint8_t)result;
    cpu->regi.pc += 1;
    INCR_CYCLE(4);
}

void adc_register(cpu_context *cpu, uint8_t *dst, uint8_t *src) {
    uint8_t carry = (cpu->regi.f & (1 << 4)) ? 1 : 0;
    uint16_t result = *dst + *src + carry;

    if ((uint8_t)result == 0) {
        SET_ZERO_FLAG(cpu);
    } else {
        CLEAR_ZERO_FLAG(cpu);
    }

    if (result > 0xFF) {
        SET_CARRY_FLAG(cpu);
    } else {
        CLEAR_CARRY_FLAG(cpu);
    }

    if (((*dst & 0xF) + (*src & 0xF) + carry) > 0xF) {
        SET_HALF_CARRY_FLAG(cpu);
    } else {
        CLEAR_HALF_CARRY_FLAG(cpu);
    }

    *dst = (uint8_t)result;
    cpu->regi.pc += 1;
    INCR_CYCLE(4);
}

void sub_register(cpu_context *cpu, uint8_t *dst, uint8_t *src) {
    uint16_t result = *dst - *src;

    if ((uint8_t)result == 0) {
        SET_ZERO_FLAG(cpu);
    } else {
        CLEAR_ZERO_FLAG(cpu);
    }
    if (*dst < *src) {
        SET_CARRY_FLAG(cpu);
    } else {
        CLEAR_CARRY_FLAG(cpu);
    }
    if ((*dst & 0xF) < (*src & 0xF)) {
        SET_HALF_CARRY_FLAG(cpu);
    } else {
        CLEAR_HALF_CARRY_FLAG(cpu);
    }
    SET_SUBTRACT_FLAG(cpu);

    *dst = (uint8_t)result;
    cpu->regi.pc += 1;
    INCR_CYCLE(4);
}

void sbc_register(cpu_context *cpu, uint8_t *dst, uint8_t *src) {
    uint8_t carry = (cpu->regi.f & (1 << 4)) ? 1 : 0;
    uint16_t result = *dst - *src - carry;

    if ((uint8_t)result == 0) {
        SET_ZERO_FLAG(cpu);
    } else {
        CLEAR_ZERO_FLAG(cpu);
    }
    if (*dst < *src + carry) {
        SET_CARRY_FLAG(cpu);
    } else {
        CLEAR_CARRY_FLAG(cpu);
    }
    if ((*dst & 0xF) < (*src & 0xF) + carry) {
        SET_HALF_CARRY_FLAG(cpu);
    } else {
        CLEAR_HALF_CARRY_FLAG(cpu);
    }
    SET_SUBTRACT_FLAG(cpu);

    *dst = (uint8_t)result;
    cpu->regi.pc += 1;
    INCR_CYCLE(4);
}

void and_register(cpu_context *cpu, uint8_t *dst, uint8_t *src) {
    *dst &= *src;

    if (*dst == 0) {
        SET_ZERO_FLAG(cpu);
    } else {
        CLEAR_ZERO_FLAG(cpu);
    }
    CLEAR_CARRY_FLAG(cpu);
    CLEAR_HALF_CARRY_FLAG(cpu);
    CLEAR_SUBTRACT_FLAG(cpu);

    cpu->regi.pc += 1;
    INCR_CYCLE(4);
}

void add_indirect(cpu_context *cpu, uint8_t *src) {
    // Fetch the value from memory address HL
    uint8_t hl_value = bus_read(cpu->regi.hl);

    uint16_t result = cpu->regi.a + hl_value;

    // Update flags
    if ((uint8_t)result == 0) {
        SET_ZERO_FLAG(cpu);
    } else {
        CLEAR_ZERO_FLAG(cpu);
    }

    if (result > 0xFF) {
        SET_CARRY_FLAG(cpu);
    } else {
        CLEAR_CARRY_FLAG(cpu);
    }

    if (((cpu->regi.a & 0xF) + (hl_value & 0xF)) > 0xF) {
        SET_HALF_CARRY_FLAG(cpu);
    } else {
        CLEAR_HALF_CARRY_FLAG(cpu);
    }

    cpu->regi.a = (uint8_t)result;

    cpu->regi.pc += 1;
    INCR_CYCLE(8);
}

void adc_indirect(cpu_context *cpu) {
    uint8_t carry = (cpu->regi.f & (1 << 4)) ? 1 : 0;  // Check the carry flag
    uint8_t hl_val = bus_read(cpu->regi.hl);

    uint16_t result = cpu->regi.a + hl_val + carry;

    // Update flags
    if ((uint8_t)result == 0) {
        SET_ZERO_FLAG(cpu);
    } else {
        CLEAR_ZERO_FLAG(cpu);
    }

    if (result > 0xFF) {
        SET_CARRY_FLAG(cpu);
    } else {
        CLEAR_CARRY_FLAG(cpu);
    }

    if (((cpu->regi.a & 0xF) + (hl_val & 0xF) + carry) > 0xF) {
        SET_HALF_CARRY_FLAG(cpu);
    } else {
        CLEAR_HALF_CARRY_FLAG(cpu);
    }

    cpu->regi.a = (uint8_t)result;

    cpu->regi.pc += 1;
    INCR_CYCLE(8);
}

void sub_indirect(cpu_context *cpu) {
    uint8_t hl_val = bus_read(cpu->regi.hl);

    uint16_t result = cpu->regi.a - hl_val;

    // Update flags
    if ((uint8_t)result == 0) {
        SET_ZERO_FLAG(cpu);
    } else {
        CLEAR_ZERO_FLAG(cpu);
    }

    if (cpu->regi.a < hl_val) {
        SET_CARRY_FLAG(cpu);
    } else {
        CLEAR_CARRY_FLAG(cpu);
    }

    if ((cpu->regi.a & 0xF) < (hl_val & 0xF)) {
        SET_HALF_CARRY_FLAG(cpu);
    } else {
        CLEAR_HALF_CARRY_FLAG(cpu);
    }

    SET_SUBTRACT_FLAG(cpu);

    cpu->regi.a = (uint8_t)result;

    cpu->regi.pc += 1;
    INCR_CYCLE(8);
}

void sbc_indirect(cpu_context *cpu) {
    uint8_t carry = (cpu->regi.f & (1 << 4)) ? 1 : 0;  // Check the carry flag
    uint8_t hl_val = bus_read(cpu->regi.hl);

    uint16_t result = cpu->regi.a - hl_val - carry;

    // Update flags
    if ((uint8_t)result == 0) {
        SET_ZERO_FLAG(cpu);
    } else {
        CLEAR_ZERO_FLAG(cpu);
    }

    if (cpu->regi.a < hl_val + carry) {
        SET_CARRY_FLAG(cpu);
    } else {
        CLEAR_CARRY_FLAG(cpu);
    }

    if ((cpu->regi.a & 0xF) < (hl_val & 0xF) + carry) {
        SET_HALF_CARRY_FLAG(cpu);
    } else {
        CLEAR_HALF_CARRY_FLAG(cpu);
    }

    SET_SUBTRACT_FLAG(cpu);

    cpu->regi.a = (uint8_t)result;

    cpu->regi.pc += 1;
    INCR_CYCLE(8);
}

void and_indirect(cpu_context *cpu) {
    uint8_t hl_val = bus_read(cpu->regi.hl);

    cpu->regi.a &= hl_val;

    // Update flags
    if (cpu->regi.a == 0) {
        SET_ZERO_FLAG(cpu);
    } else {
        CLEAR_ZERO_FLAG(cpu);
    }

    CLEAR_CARRY_FLAG(cpu);
    SET_HALF_CARRY_FLAG(cpu);
    CLEAR_SUBTRACT_FLAG(cpu);

    cpu->regi.pc += 1;
    INCR_CYCLE(8);
}

void xor_register(cpu_context *cpu, uint8_t *src) {
    cpu->regi.a ^= *src;

    // Update flags
    if (cpu->regi.a == 0) {
        SET_ZERO_FLAG(cpu);
    } else {
        CLEAR_ZERO_FLAG(cpu);
    }

    CLEAR_CARRY_FLAG(cpu);
    CLEAR_HALF_CARRY_FLAG(cpu);
    CLEAR_SUBTRACT_FLAG(cpu);

    cpu->regi.pc += 1;
    INCR_CYCLE(4);
}

void xor_indirect(cpu_context *cpu) {
    uint8_t hl_val = bus_read(cpu->regi.hl);

    cpu->regi.a ^= hl_val;

    // Update flags
    if (cpu->regi.a == 0) {
        SET_ZERO_FLAG(cpu);
    } else {
        CLEAR_ZERO_FLAG(cpu);
    }

    CLEAR_CARRY_FLAG(cpu);
    CLEAR_HALF_CARRY_FLAG(cpu);
    CLEAR_SUBTRACT_FLAG(cpu);

    cpu->regi.pc += 1;
    INCR_CYCLE(8);
}

void or_register(cpu_context *cpu, uint8_t *src) {
    cpu->regi.a |= *src;

    // Update flags
    if (cpu->regi.a == 0) {
        SET_ZERO_FLAG(cpu);
    } else {
        CLEAR_ZERO_FLAG(cpu);
    }

    CLEAR_CARRY_FLAG(cpu);
    CLEAR_HALF_CARRY_FLAG(cpu);
    CLEAR_SUBTRACT_FLAG(cpu);

    cpu->regi.pc += 1;
    INCR_CYCLE(4);
}

void or_indirect(cpu_context *cpu) {
    uint8_t hl_val = bus_read(cpu->regi.hl);

    cpu->regi.a |= hl_val;

    // Update flags
    if (cpu->regi.a == 0) {
        SET_ZERO_FLAG(cpu);
    } else {
        CLEAR_ZERO_FLAG(cpu);
    }

    CLEAR_CARRY_FLAG(cpu);
    CLEAR_HALF_CARRY_FLAG(cpu);
    CLEAR_SUBTRACT_FLAG(cpu);

    cpu->regi.pc += 1;
    INCR_CYCLE(8);
}

void cp_register(cpu_context *cpu, uint8_t *src) {
    uint16_t result = cpu->regi.a - *src;

    // Update flags
    if ((uint8_t)result == 0) {
        SET_ZERO_FLAG(cpu);
    } else {
        CLEAR_ZERO_FLAG(cpu);
    }

    if (cpu->regi.a < *src) {
        SET_CARRY_FLAG(cpu);
    } else {
        CLEAR_CARRY_FLAG(cpu);
    }

    if ((cpu->regi.a & 0xF) < (*src & 0xF)) {
        SET_HALF_CARRY_FLAG(cpu);
    } else {
        CLEAR_HALF_CARRY_FLAG(cpu);
    }

    SET_SUBTRACT_FLAG(cpu);

    cpu->regi.pc += 1;
    INCR_CYCLE(4);
}

void cp_indirect(cpu_context *cpu) {
    uint8_t hl_val = bus_read(cpu->regi.hl);

    uint16_t result = cpu->regi.a - hl_val;

    // Update flags
    if ((uint8_t)result == 0) {
        SET_ZERO_FLAG(cpu);
    } else {
        CLEAR_ZERO_FLAG(cpu);
    }

    if (cpu->regi.a < hl_val) {
        SET_CARRY_FLAG(cpu);
    } else {
        CLEAR_CARRY_FLAG(cpu);
    }

    if ((cpu->regi.a & 0xF) < (hl_val & 0xF)) {
        SET_HALF_CARRY_FLAG(cpu);
    } else {
        CLEAR_HALF_CARRY_FLAG(cpu);
    }

    SET_SUBTRACT_FLAG(cpu);

    cpu->regi.pc += 1;
    INCR_CYCLE(8);
}

void cp_immediate(cpu_context *cpu) {
    uint8_t value = bus_read(cpu->regi.pc+ 1);  
    uint16_t result = cpu->regi.a - value;

    // Update flags
    if ((uint8_t)result == 0) SET_ZERO_FLAG(cpu);
    else CLEAR_ZERO_FLAG(cpu);

    if (cpu->regi.a < value) SET_CARRY_FLAG(cpu);
    else CLEAR_CARRY_FLAG(cpu);

    if ((cpu->regi.a & 0xF) < (value & 0xF)) SET_HALF_CARRY_FLAG(cpu);
    else CLEAR_HALF_CARRY_FLAG(cpu);

    SET_SUBTRACT_FLAG(cpu);

    cpu->regi.pc += 2;
    INCR_CYCLE(8);
}

void xor_immediate(cpu_context *cpu) {
    uint8_t value = bus_read(cpu->regi.pc+ 1);

    cpu->regi.a ^= value;

    // Update flags
    if (cpu->regi.a == 0) SET_ZERO_FLAG(cpu);
    else CLEAR_ZERO_FLAG(cpu);

    CLEAR_CARRY_FLAG(cpu);
    CLEAR_HALF_CARRY_FLAG(cpu);
    CLEAR_SUBTRACT_FLAG(cpu);

    cpu->regi.pc += 2;
    INCR_CYCLE(8);
}

void and_immediate(cpu_context *cpu) {
    uint8_t value = bus_read(cpu->regi.pc+ 1);

    cpu->regi.a &= value;

    // Update flags
    if (cpu->regi.a == 0) SET_ZERO_FLAG(cpu);
    else CLEAR_ZERO_FLAG(cpu);

    CLEAR_CARRY_FLAG(cpu);
    SET_HALF_CARRY_FLAG(cpu);
    CLEAR_SUBTRACT_FLAG(cpu);

    cpu->regi.pc += 2;
    INCR_CYCLE(8);
}

void sbc_immediate(cpu_context *cpu) {
    uint8_t value = bus_read(cpu->regi.pc+ 1);
    uint8_t carry = (cpu->regi.f & (1 << 4)) ? 1 : 0;
    uint16_t result = cpu->regi.a - value - carry;

    // Update flags
    if ((uint8_t)result == 0) SET_ZERO_FLAG(cpu);
    else CLEAR_ZERO_FLAG(cpu);

    if (cpu->regi.a < (value + carry)) SET_CARRY_FLAG(cpu);
    else CLEAR_CARRY_FLAG(cpu);

    if ((cpu->regi.a & 0xF) < ((value & 0xF) + carry)) SET_HALF_CARRY_FLAG(cpu);
    else CLEAR_HALF_CARRY_FLAG(cpu);

    SET_SUBTRACT_FLAG(cpu);

    cpu->regi.a = (uint8_t)result;
    cpu->regi.pc += 2;
    INCR_CYCLE(8);
}

void sub_immediate(cpu_context *cpu) {
    uint8_t value = bus_read(cpu->regi.pc+ 1);
    uint16_t result = cpu->regi.a - value;

    // Update flags
    if ((uint8_t)result == 0) SET_ZERO_FLAG(cpu);
    else CLEAR_ZERO_FLAG(cpu);

    if (cpu->regi.a < value) SET_CARRY_FLAG(cpu);
    else CLEAR_CARRY_FLAG(cpu);

    if ((cpu->regi.a & 0xF) < (value & 0xF)) SET_HALF_CARRY_FLAG(cpu);
    else CLEAR_HALF_CARRY_FLAG(cpu);

    SET_SUBTRACT_FLAG(cpu);

    cpu->regi.a = (uint8_t)result;
    cpu->regi.pc += 2;
    INCR_CYCLE(8);
}

void adc_immediate(cpu_context *cpu) {
    uint8_t value = bus_read(cpu->regi.pc+ 1);
    uint8_t carry = (cpu->regi.f & (1 << 4)) ? 1 : 0;
    uint16_t result = cpu->regi.a + value + carry;

    // Update flags
    if ((uint8_t)result == 0) SET_ZERO_FLAG(cpu);
    else CLEAR_ZERO_FLAG(cpu);

    if (result > 0xFF) SET_CARRY_FLAG(cpu);
    else CLEAR_CARRY_FLAG(cpu);

    if (((cpu->regi.a & 0xF) + (value & 0xF) + carry) > 0xF) SET_HALF_CARRY_FLAG(cpu);
    else CLEAR_HALF_CARRY_FLAG(cpu);

    CLEAR_SUBTRACT_FLAG(cpu);

    cpu->regi.a = (uint8_t)result;
    cpu->regi.pc += 2;
    INCR_CYCLE(8);
}

void add_immediate(cpu_context *cpu) {
    uint8_t value = bus_read(cpu->regi.pc+ 1);
    uint16_t result = cpu->regi.a + value;

    // Update flags
    if ((uint8_t)result == 0) SET_ZERO_FLAG(cpu);
    else CLEAR_ZERO_FLAG(cpu);

    if (result > 0xFF) SET_CARRY_FLAG(cpu);
    else CLEAR_CARRY_FLAG(cpu);

    if (((cpu->regi.a & 0xF) + (value & 0xF)) > 0xF) SET_HALF_CARRY_FLAG(cpu);
    else CLEAR_HALF_CARRY_FLAG(cpu);

    CLEAR_SUBTRACT_FLAG(cpu);

    cpu->regi.a = (uint8_t)result;
    cpu->regi.pc += 2;
    INCR_CYCLE(8);
}

