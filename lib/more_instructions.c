#include <instruction.h>

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
    INCR_CYCLE(4);
}

void complement(cpu_context *cpu) {
    cpu->regi.a = ~cpu->regi.a;
    SET_SUBTRACT_FLAG(cpu);
    SET_HALF_CARRY_FLAG(cpu);
    cpu->regi.pc += 1;
    INCR_CYCLE(4);
}

void set_carry(cpu_context *cpu) {
    SET_CARRY_FLAG(cpu);
    CLEAR_SUBTRACT_FLAG(cpu);
    CLEAR_HALF_CARRY_FLAG(cpu);
    cpu->regi.pc += 1;
    INCR_CYCLE(4);
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
    INCR_CYCLE(4);
}

void halt(cpu_context *cpu) {
    cpu->halted = true;
    cpu->regi.pc += 1;
}

void ret_cond(cpu_context *cpu, bool condition) {
    if (condition) {
        cpu->regi.pc = bus_read16(cpu->regi.sp);
        cpu->regi.sp += 2;
        INCR_CYCLE(20);
    } else {
        cpu->regi.pc += 1;
        INCR_CYCLE(8);
    }
}

void pop_register_pair(cpu_context *cpu, uint16_t *reg) {
    *reg = bus_read16(cpu->regi.sp);
    cpu->regi.sp += 2;
    cpu->regi.pc += 1;
    INCR_CYCLE(12);
}

void call_cond(cpu_context *cpu, bool condition) {
    debug_breakpoint = true;
    if (condition) {
        uint16_t address = bus_read16(cpu->regi.pc + 1);
        cpu->regi.sp -= 2;
        bus_write16(cpu->regi.sp, cpu->regi.pc + 3);
        cpu->regi.pc = address;
        INCR_CYCLE(24);
    } else {
        cpu->regi.pc += 3;
        INCR_CYCLE(12);
    }
}

void push_register_pair(cpu_context *cpu, uint16_t reg) {
    cpu->regi.sp -= 2;
    bus_write16(cpu->regi.sp, reg);
    cpu->regi.pc += 1;
    INCR_CYCLE(16);
}

void restart(cpu_context *cpu, uint8_t address) {
    cpu->regi.sp -= 2;
    bus_write16(cpu->regi.sp, cpu->regi.pc + 1);
    cpu->regi.pc = address;
    INCR_CYCLE(16);
}

void ret(cpu_context *cpu) {
    uint16_t ret_addr = bus_read16(cpu->regi.sp);
    printf("[RET] SP=0x%04X, popping return address 0x%04X\n", cpu->regi.sp, ret_addr);

    cpu->regi.pc = ret_addr;
    cpu->regi.sp += 2;

    INCR_CYCLE(16);
}

void call(cpu_context *cpu) {
    uint16_t address = bus_read16(cpu->regi.pc + 1);
    printf("[CALL] PC=0x%04X, call address=0x%04X, SP before=0x%04X\n", cpu->regi.pc, address, cpu->regi.sp);
    printf("[CALL] writing 0x%04X at 0x%04X\n", cpu->regi.pc + 3, cpu->regi.sp);

    cpu->regi.sp -= 2;
    bus_write16(cpu->regi.sp, cpu->regi.pc + 3);  // push return address (after CALL instruction)

    cpu->regi.pc = address;  // jump to called address

    printf("[CALL] address written after=0x%04X : \n",bus_read16(cpu->regi.sp));
    printf("[CALL] SP after=0x%04X, PC set to 0x%04X\n", cpu->regi.sp, cpu->regi.pc);

    INCR_CYCLE(24);
}

void invalid_opcode(cpu_context *cpu) {
    // Handle an invalid opcode gracefully (could be a no-op or crash handling)
    cpu->regi.pc += 1;
}

void reti(cpu_context *cpu) {
    cpu->regi.pc = bus_read16(cpu->regi.sp);
    cpu->regi.sp += 2;
    cpu->IME = true;
    INCR_CYCLE(16);
}

void or_immediate(cpu_context *cpu) {
    cpu->regi.a |= bus_read(cpu->regi.pc + 1);
    if (cpu->regi.a == 0) SET_ZERO_FLAG(cpu);
    else CLEAR_ZERO_FLAG(cpu);
    CLEAR_CARRY_FLAG(cpu);
    CLEAR_HALF_CARRY_FLAG(cpu);
    CLEAR_SUBTRACT_FLAG(cpu);
    cpu->regi.pc += 2;
    INCR_CYCLE(8);
}

void enable_interrupts(cpu_context *cpu) {
    cpu->IME = true;
    cpu->regi.pc += 1;
    INCR_CYCLE(4);
}

void disable_interrupts(cpu_context *cpu) {
    cpu->IME = false;
    cpu->regi.pc += 1;
    INCR_CYCLE(4);
}

void execute_cb_instruction(cpu_context *cpu) {
    // Fetch the CB‐prefixed opcode (second byte)
    uint16_t pc = cpu->regi.pc;
    uint8_t cb = bus_read(pc + 1);

    // Decode fields
    uint8_t x      = (cb >> 6) & 0x03;   // top two bits: operation group
    uint8_t y      = (cb >> 3) & 0x07;   // next three bits: sub‐op or bit index
    uint8_t reg_id =  cb       & 0x07;   // low three bits: register operand

    // Helper: read or write the target
    uint8_t val;
    bool  is_mem = (reg_id == 6);
    if (is_mem) {
        val = bus_read(cpu->regi.hl);
    } else {
        switch (reg_id) {
            case 0: val = cpu->regi.b; break;
            case 1: val = cpu->regi.c; break;
            case 2: val = cpu->regi.d; break;
            case 3: val = cpu->regi.e; break;
            case 4: val = cpu->regi.h; break;
            case 5: val = cpu->regi.l; break;
            case 7: val = cpu->regi.a; break;
            default: val = 0;            break;
        }
    }

    uint8_t res = val;
    uint8_t mask;

    // Execute
    switch (x) {
        // 00: Rotate/shift group
        case 0:
            switch (y) {
                case 0: // RLC
                    {
                        uint8_t carry = (val & 0x80) >> 7;
                        res = (val << 1) | carry;
                        CLEAR_SUBTRACT_FLAG(cpu);
                        CLEAR_HALF_CARRY_FLAG(cpu);
                        if (carry) SET_CARRY_FLAG(cpu);
                        else       CLEAR_CARRY_FLAG(cpu);
                        if (res == 0) SET_ZERO_FLAG(cpu);
                        else           CLEAR_ZERO_FLAG(cpu);
                    }
                    break;
                case 1: // RRC
                    {
                        uint8_t carry = val & 0x01;
                        res = (val >> 1) | (carry << 7);
                        CLEAR_SUBTRACT_FLAG(cpu);
                        CLEAR_HALF_CARRY_FLAG(cpu);
                        if (carry) SET_CARRY_FLAG(cpu);
                        else       CLEAR_CARRY_FLAG(cpu);
                        if (res == 0) SET_ZERO_FLAG(cpu);
                        else           CLEAR_ZERO_FLAG(cpu);
                    }
                    break;
                case 2: // RL
                    {
                        uint8_t old_c = (cpu->regi.f & (1 << 4)) >> 4;
                        uint8_t new_c = (val & 0x80) >> 7;
                        res = (val << 1) | old_c;
                        CLEAR_SUBTRACT_FLAG(cpu);
                        CLEAR_HALF_CARRY_FLAG(cpu);
                        if (new_c) SET_CARRY_FLAG(cpu);
                        else       CLEAR_CARRY_FLAG(cpu);
                        if (res == 0) SET_ZERO_FLAG(cpu);
                        else           CLEAR_ZERO_FLAG(cpu);
                    }
                    break;
                case 3: // RR
                    {
                        uint8_t old_c = (cpu->regi.f & (1 << 4)) >> 4;
                        uint8_t new_c = val & 0x01;
                        res = (val >> 1) | (old_c << 7);
                        CLEAR_SUBTRACT_FLAG(cpu);
                        CLEAR_HALF_CARRY_FLAG(cpu);
                        if (new_c) SET_CARRY_FLAG(cpu);
                        else       CLEAR_CARRY_FLAG(cpu);
                        if (res == 0) SET_ZERO_FLAG(cpu);
                        else           CLEAR_ZERO_FLAG(cpu);
                    }
                    break;
                case 4: // SLA
                    {
                        uint8_t new_c = (val & 0x80) >> 7;
                        res = val << 1;
                        CLEAR_SUBTRACT_FLAG(cpu);
                        CLEAR_HALF_CARRY_FLAG(cpu);
                        if (new_c) SET_CARRY_FLAG(cpu);
                        else       CLEAR_CARRY_FLAG(cpu);
                        if (res == 0) SET_ZERO_FLAG(cpu);
                        else           CLEAR_ZERO_FLAG(cpu);
                    }
                    break;
                case 5: // SRA
                    {
                        uint8_t new_c = val & 0x01;
                        uint8_t msb   = val & 0x80;
                        res = (val >> 1) | msb;
                        CLEAR_SUBTRACT_FLAG(cpu);
                        CLEAR_HALF_CARRY_FLAG(cpu);
                        if (new_c) SET_CARRY_FLAG(cpu);
                        else       CLEAR_CARRY_FLAG(cpu);
                        if (res == 0) SET_ZERO_FLAG(cpu);
                        else           CLEAR_ZERO_FLAG(cpu);
                    }
                    break;
                case 6: // SWAP
                    {
                        res = (val << 4) | (val >> 4);
                        CLEAR_SUBTRACT_FLAG(cpu);
                        CLEAR_HALF_CARRY_FLAG(cpu);
                        CLEAR_CARRY_FLAG(cpu);
                        if (res == 0) SET_ZERO_FLAG(cpu);
                        else           CLEAR_ZERO_FLAG(cpu);
                    }
                    break;
                case 7: // SRL
                    {
                        uint8_t new_c = val & 0x01;
                        res = val >> 1;
                        CLEAR_SUBTRACT_FLAG(cpu);
                        CLEAR_HALF_CARRY_FLAG(cpu);
                        if (new_c) SET_CARRY_FLAG(cpu);
                        else       CLEAR_CARRY_FLAG(cpu);
                        if (res == 0) SET_ZERO_FLAG(cpu);
                        else           CLEAR_ZERO_FLAG(cpu);
                    }
                    break;
            }
            break;

        // 01: BIT y, r
        case 1:
            mask = 1 << y;
            CLEAR_SUBTRACT_FLAG(cpu);
            SET_HALF_CARRY_FLAG(cpu);
            if ((val & mask) == 0) SET_ZERO_FLAG(cpu);
            else                   CLEAR_ZERO_FLAG(cpu);
            // Carry flag unaffected
            break;

        // 10: RES y, r  (reset bit y)
        case 2:
            res = val & ~(1 << y);
            break;

        // 11: SET y, r  (set bit y)
        case 3:
            res = val |  (1 << y);
            break;
    }

    // Write back result
    if (is_mem) {
        bus_write(cpu->regi.hl, res);
    } else {
        switch (reg_id) {
            case 0: cpu->regi.b = res; break;
            case 1: cpu->regi.c = res; break;
            case 2: cpu->regi.d = res; break;
            case 3: cpu->regi.e = res; break;
            case 4: cpu->regi.h = res; break;
            case 5: cpu->regi.l = res; break;
            case 7: cpu->regi.a = res; break;
        }
    }

    // Advance PC past the CB prefix and its operand
    cpu->regi.pc += 2;

    // Timing: 8 cycles for register ops, 16 for (HL)
    if (is_mem) INCR_CYCLE(16);
    else        INCR_CYCLE(8);
}
