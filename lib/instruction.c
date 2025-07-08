#include <instruction.h>
#include <cpu.h>

void execute_instruction(cpu_context *cpu) {
    printf("executed instruction %02X \n" , cpu->cur_opcode);
    switch (cpu->cur_opcode) {
        case 0x00:
            cpu->regi.pc += 1;
            INCR_CYCLE(4);
            break;
        case 0x01:
            load_immediate_16(cpu, &cpu->regi.bc);
            break;
        case 0x02:
            store_indirect(cpu, &cpu->regi.a, &cpu->regi.bc);
            break;
        case 0x03:
            increment_16(cpu, &cpu->regi.bc);
            break;
        case 0x04:
            increment(cpu, &cpu->regi.b);
            break;
        case 0x05:
            decrement(cpu, &cpu->regi.b);
            break;
        case 0x06:
            load_immediate(cpu, &cpu->regi.b);
            break;
        case 0x07:
            rotate_left_carry(cpu, &cpu->regi.a);
            break;
        case 0x08:
            store_direct_16(cpu, &cpu->regi.sp);
            break;
        case 0x09:
            add_16(cpu, &cpu->regi.bc);
            break;
        case 0x0A:
            load_indirect(cpu, &cpu->regi.a, &cpu->regi.bc);
            break;
        case 0x0B:
            decrement_16(cpu, &cpu->regi.bc);
            break;
        case 0x0C:
            increment(cpu, &cpu->regi.c);
            break;
        case 0x0D:
            decrement(cpu, &cpu->regi.c);
            break;
        case 0x0E:
            load_immediate(cpu, &cpu->regi.c);
            break;
        case 0x0F:
            rotate_right_carry(cpu, &cpu->regi.a);
            break;
        case 0x10:
            stop(cpu);
            break;
        case 0x11:
            load_immediate_16(cpu, &cpu->regi.de);
            break;
        case 0x12:
            store_indirect(cpu, &cpu->regi.a, &cpu->regi.de);
            break;
        case 0x13:
            increment_16(cpu, &cpu->regi.de);
            break;
        case 0x14:
            increment(cpu, &cpu->regi.d);
            break;
        case 0x15:
            decrement(cpu, &cpu->regi.d);
            break;
        case 0x16:
            load_immediate(cpu, &cpu->regi.d);
            break;
        case 0x17:
            rotate_left(cpu, &cpu->regi.a);
            break;
        case 0x18:
            jump_relative(cpu);
            break;
        case 0x19:
            add_16(cpu, &cpu->regi.de);
            break;
        case 0x1A:
            load_indirect(cpu, &cpu->regi.a, &cpu->regi.de);
            break;
        case 0x1B:
            decrement_16(cpu, &cpu->regi.de);
            break;
        case 0x1C:
            increment(cpu, &cpu->regi.e);
            break;
        case 0x1D:
            decrement(cpu, &cpu->regi.e);
            break;
        case 0x1E:
            load_immediate(cpu, &cpu->regi.e);
            break;
        case 0x1F:
            rotate_right(cpu, &cpu->regi.a);
            break;
        case 0x20:
            jump_relative_cond(cpu, cpu->regi.f & ZERO_FLAG);
            break;
        case 0x21:
            load_immediate_16(cpu, &cpu->regi.hl);
            break;
        case 0x22:
            store_indirect(cpu, &cpu->regi.a, &cpu->regi.hl);
            cpu->regi.hl++;
            break;
        case 0x23:
            increment_16(cpu, &cpu->regi.hl);
            break;
        case 0x24:
            increment(cpu, &cpu->regi.h);
            break;
        case 0x25:
            decrement(cpu, &cpu->regi.h);
            break;
        case 0x26:
            load_immediate(cpu, &cpu->regi.h);
            break;
        case 0x27:
            decimal_adjust(cpu);
            break;
        case 0x28:
            jump_relative_cond(cpu, !(cpu->regi.f & ZERO_FLAG));
            break;
        case 0x29:
            add_16(cpu, &cpu->regi.hl);
            break;
        case 0x2A:
            load_indirect(cpu, &cpu->regi.a, &cpu->regi.hl);
            cpu->regi.hl++;
            break;
        case 0x2B:
            decrement_16(cpu, &cpu->regi.hl);
            break;
        case 0x2C:
            increment(cpu, &cpu->regi.l);
            break;
        case 0x2D:
            decrement(cpu, &cpu->regi.l);
            break;
        case 0x2E:
            load_immediate(cpu, &cpu->regi.l);
            break;
        case 0x2F:
            complement(cpu);
            break;
        case 0x30:
            jump_relative_cond(cpu, cpu->regi.f & CARRY_FLAG);
            break;
        case 0x31:
            load_immediate_16(cpu, &cpu->regi.sp);
            break;
        case 0x32:
            store_indirect(cpu, &cpu->regi.a, &cpu->regi.hl);
            cpu->regi.hl--;
            break;
        case 0x33:
            increment_16(cpu, &cpu->regi.sp);
            break;
        case 0x34:
            increment_indirect(cpu);
            break;
        case 0x35:
            decrement_indirect(cpu);
            break;
        case 0x36:
            store_immediate(cpu);
            break;
        case 0x37:
            set_carry(cpu);
            break;
        case 0x38:
            jump_relative_cond(cpu, !(cpu->regi.f & CARRY_FLAG));
            break;
        case 0x39:
            add_16(cpu, &cpu->regi.sp);
            break;
        case 0x3A:
            load_indirect(cpu, &cpu->regi.a, &cpu->regi.hl);
            cpu->regi.hl--;
            break;
        case 0x3B:
            decrement_16(cpu, &cpu->regi.sp);
            break;
        case 0x3C:
            increment(cpu, &cpu->regi.a);
            break;
        case 0x3D:
            decrement(cpu, &cpu->regi.a);
            break;
        case 0x3E:
            load_immediate(cpu, &cpu->regi.a);
            break;
        case 0x3F:
            complement_carry(cpu);
            break;
        case 0x40:
            load_register(cpu, &cpu->regi.b, &cpu->regi.b);
            break;
        case 0x41:
            load_register(cpu, &cpu->regi.b, &cpu->regi.c);
            break;
        case 0x42:
            load_register(cpu, &cpu->regi.b, &cpu->regi.d);
            break;
        case 0x43:
            load_register(cpu, &cpu->regi.b, &cpu->regi.e);
            break;
        case 0x44:
            load_register(cpu, &cpu->regi.b, &cpu->regi.h);
            break;
        case 0x45:
            load_register(cpu, &cpu->regi.b, &cpu->regi.l);
            break;
        case 0x46:
            load_indirect(cpu, &cpu->regi.b, &cpu->regi.hl);
            break;
        case 0x47:
            load_register(cpu, &cpu->regi.b, &cpu->regi.a);
            break;
        case 0x48:
            load_register(cpu, &cpu->regi.c, &cpu->regi.b);
            break;
        case 0x49:
            load_register(cpu, &cpu->regi.c, &cpu->regi.c);
            break;
        case 0x4A:
            load_register(cpu, &cpu->regi.c, &cpu->regi.d);
            break;
        case 0x4B:
            load_register(cpu, &cpu->regi.c, &cpu->regi.e);
            break;
        case 0x4C:
            load_register(cpu, &cpu->regi.c, &cpu->regi.h);
            break;
        case 0x4D:
            load_register(cpu, &cpu->regi.c, &cpu->regi.l);
            break;
        case 0x4E:
            load_indirect(cpu, &cpu->regi.c, &cpu->regi.hl);
            break;
        case 0x4F:
            load_register(cpu, &cpu->regi.c, &cpu->regi.a);
            break;
        case 0x50:
            load_register(cpu, &cpu->regi.d, &cpu->regi.b);
            break;
        case 0x51:
            load_register(cpu, &cpu->regi.d, &cpu->regi.c);
            break;
        case 0x52:
            load_register(cpu, &cpu->regi.d, &cpu->regi.d);
            break;
        case 0x53:
            load_register(cpu, &cpu->regi.d, &cpu->regi.e);
            break;
        case 0x54:
            load_register(cpu, &cpu->regi.d, &cpu->regi.h);
            break;
        case 0x55:
            load_register(cpu, &cpu->regi.d, &cpu->regi.l);
            break;
        case 0x56:
            load_indirect(cpu, &cpu->regi.d, &cpu->regi.hl);
            break;
        case 0x57:
            load_register(cpu, &cpu->regi.d, &cpu->regi.a);
            break;
        case 0x58:
            load_register(cpu, &cpu->regi.e, &cpu->regi.b);
            break;
        case 0x59:
            load_register(cpu, &cpu->regi.e, &cpu->regi.c);
            break;
        case 0x5A:
            load_register(cpu, &cpu->regi.e, &cpu->regi.d);
            break;
        case 0x5B:
            load_register(cpu, &cpu->regi.e, &cpu->regi.e);
            break;
        case 0x5C:
            load_register(cpu, &cpu->regi.e, &cpu->regi.h);
            break;
        case 0x5D:
            load_register(cpu, &cpu->regi.e, &cpu->regi.l);
            break;
        case 0x5E:
            load_indirect(cpu, &cpu->regi.e, &cpu->regi.hl);
            break;
        case 0x5F:
            load_register(cpu, &cpu->regi.e, &cpu->regi.a);
            break;
        case 0x60:
            load_register(cpu, &cpu->regi.h, &cpu->regi.b);
            break;
        case 0x61:
            load_register(cpu, &cpu->regi.h, &cpu->regi.c);
            break;
        case 0x62:
            load_register(cpu, &cpu->regi.h, &cpu->regi.d);
            break;
        case 0x63:
            load_register(cpu, &cpu->regi.h, &cpu->regi.e);
            break;
        case 0x64:
            load_register(cpu, &cpu->regi.h, &cpu->regi.h);
            break;
        case 0x65:
            load_register(cpu, &cpu->regi.h, &cpu->regi.l);
            break;
        case 0x66:
            load_indirect(cpu, &cpu->regi.h, &cpu->regi.hl);
            break;
        case 0x67:
            load_register(cpu, &cpu->regi.h, &cpu->regi.a);
            break;
        case 0x68:
            load_register(cpu, &cpu->regi.l, &cpu->regi.b);
            break;
        case 0x69:
            load_register(cpu, &cpu->regi.l, &cpu->regi.c);
            break;
        case 0x6A:
            load_register(cpu, &cpu->regi.l, &cpu->regi.d);
            break;
        case 0x6B:
            load_register(cpu, &cpu->regi.l, &cpu->regi.e);
            break;
        case 0x6C:
            load_register(cpu, &cpu->regi.l, &cpu->regi.h);
            break;
        case 0x6D:
            load_register(cpu, &cpu->regi.l, &cpu->regi.l);
            break;
        case 0x6E:
            load_indirect(cpu, &cpu->regi.l, &cpu->regi.hl);
            break;
        case 0x6F:
            load_register(cpu, &cpu->regi.l, &cpu->regi.a);
            break;
        case 0x70:
            store_indirect(cpu, &cpu->regi.b, &cpu->regi.hl);
            break;
        case 0x71:
            store_indirect(cpu, &cpu->regi.c, &cpu->regi.hl);
            break;
        case 0x72:
            store_indirect(cpu, &cpu->regi.d, &cpu->regi.hl);
            break;
        case 0x73:
            store_indirect(cpu, &cpu->regi.e, &cpu->regi.hl);
            break;
        case 0x74:
            store_indirect(cpu, &cpu->regi.h, &cpu->regi.hl);
            break;
        case 0x75:
            store_indirect(cpu, &cpu->regi.l, &cpu->regi.hl);
            break;
        case 0x76:
            halt(cpu);
            break;
        case 0x77:
            store_indirect(cpu, &cpu->regi.a, &cpu->regi.hl);
            break;
        case 0x78:
            load_register(cpu, &cpu->regi.a, &cpu->regi.b);
            break;
        case 0x79:
            load_register(cpu, &cpu->regi.a, &cpu->regi.c);
            break;
        case 0x7A:
            load_register(cpu, &cpu->regi.a, &cpu->regi.d);
            break;
        case 0x7B:
            load_register(cpu, &cpu->regi.a, &cpu->regi.e);
            break;
        case 0x7C:
            load_register(cpu, &cpu->regi.a, &cpu->regi.h);
            break;
        case 0x7D:
            load_register(cpu, &cpu->regi.a, &cpu->regi.l);
            break;
        case 0x7E:
            load_indirect(cpu, &cpu->regi.a, &cpu->regi.hl);
            break;
        case 0x7F:
            load_register(cpu, &cpu->regi.a, &cpu->regi.a);
            break;
        case 0x80:
            add_register(cpu, &cpu->regi.a, &cpu->regi.b);
            break;
        case 0x81:
            add_register(cpu, &cpu->regi.a, &cpu->regi.c);
            break;
        case 0x82:
            add_register(cpu, &cpu->regi.a, &cpu->regi.d);
            break;
        case 0x83:
            add_register(cpu, &cpu->regi.a, &cpu->regi.e);
            break;
        case 0x84:
            add_register(cpu, &cpu->regi.a, &cpu->regi.h);
            break;
        case 0x85:
            add_register(cpu, &cpu->regi.a, &cpu->regi.l);
            break;
        case 0x86:
            add_indirect(cpu);
            break;
        case 0x87:
            add_register(cpu, &cpu->regi.a, &cpu->regi.a);
            break;
        case 0x88:
            adc_register(cpu, &cpu->regi.a, &cpu->regi.b);
            break;
        case 0x89:
            adc_register(cpu, &cpu->regi.a, &cpu->regi.c);
            break;
        case 0x8A:
            adc_register(cpu, &cpu->regi.a, &cpu->regi.d);
            break;
        case 0x8B:
            adc_register(cpu, &cpu->regi.a, &cpu->regi.e);
            break;
        case 0x8C:
            adc_register(cpu, &cpu->regi.a, &cpu->regi.h);
            break;
        case 0x8D:
            adc_register(cpu, &cpu->regi.a, &cpu->regi.l);
            break;
        case 0x8E:
            adc_indirect(cpu);
            break;
        case 0x8F:
            adc_register(cpu, &cpu->regi.a, &cpu->regi.a);
            break;
        case 0x90:
            sub_register(cpu, &cpu->regi.a, &cpu->regi.b);
            break;
        case 0x91:
            sub_register(cpu, &cpu->regi.a, &cpu->regi.c);
            break;
        case 0x92:
            sub_register(cpu, &cpu->regi.a, &cpu->regi.d);
            break;
        case 0x93:
            sub_register(cpu, &cpu->regi.a, &cpu->regi.e);
            break;
        case 0x94:
            sub_register(cpu, &cpu->regi.a, &cpu->regi.h);
            break;
        case 0x95:
            sub_register(cpu, &cpu->regi.a, &cpu->regi.l);
            break;
        case 0x96:
            sub_indirect(cpu);
            break;
        case 0x97:
            sub_register(cpu, &cpu->regi.a, &cpu->regi.a);
            break;
        case 0x98:
            sbc_register(cpu, &cpu->regi.a, &cpu->regi.b);
            break;
        case 0x99:
            sbc_register(cpu, &cpu->regi.a, &cpu->regi.c);
            break;
        case 0x9A:
            sbc_register(cpu, &cpu->regi.a, &cpu->regi.d);
            break;
        case 0x9B:
            sbc_register(cpu, &cpu->regi.a, &cpu->regi.e);
            break;
        case 0x9C:
            sbc_register(cpu, &cpu->regi.a, &cpu->regi.h);
            break;
        case 0x9D:
            sbc_register(cpu, &cpu->regi.a, &cpu->regi.l);
            break;
        case 0x9E:
            sbc_indirect(cpu);
            break;
        case 0x9F:
            sbc_register(cpu, &cpu->regi.a, &cpu->regi.a);
            break;
        case 0xA0:
            and_register(cpu, &cpu->regi.a, &cpu->regi.b);
            break;
        case 0xA1:
            and_register(cpu, &cpu->regi.a, &cpu->regi.c);
            break;
        case 0xA2:
            and_register(cpu, &cpu->regi.a, &cpu->regi.d);
            break;
        case 0xA3:
            and_register(cpu, &cpu->regi.a, &cpu->regi.e);
            break;
        case 0xA4:
            and_register(cpu, &cpu->regi.a, &cpu->regi.h);
            break;
        case 0xA5:
            and_register(cpu, &cpu->regi.a, &cpu->regi.l);
            break;
        case 0xA6:
            and_indirect(cpu);
            break;
        case 0xA7:
            and_register(cpu, &cpu->regi.a, &cpu->regi.a);
            break;
        case 0xA8:
            xor_register(cpu, &cpu->regi.b);
            break;
        case 0xA9:
            xor_register(cpu, &cpu->regi.c);
            break;
        case 0xAA:
            xor_register(cpu, &cpu->regi.d);
            break;
        case 0xAB:
            xor_register(cpu, &cpu->regi.e);
            break;
        case 0xAC:
            xor_register(cpu, &cpu->regi.h);
            break;
        case 0xAD:
            xor_register(cpu, &cpu->regi.l);
            break;
        case 0xAE:
            xor_indirect(cpu);
            break;
        case 0xAF:
            xor_register(cpu, &cpu->regi.a);
            break;
        case 0xB0:
            or_register(cpu, &cpu->regi.b);
            break;
        case 0xB1:
            or_register(cpu, &cpu->regi.c);
            break;
        case 0xB2:
            or_register(cpu, &cpu->regi.d);
            break;
        case 0xB3:
            or_register(cpu, &cpu->regi.e);
            break;
        case 0xB4:
            or_register(cpu, &cpu->regi.h);
            break;
        case 0xB5:
            or_register(cpu, &cpu->regi.l);
            break;
        case 0xB6:
            or_indirect(cpu);
            break;
        case 0xB7:
            or_register(cpu, &cpu->regi.a);
            break;
        case 0xB8:
            cp_register(cpu, &cpu->regi.b);
            break;
        case 0xB9:
            cp_register(cpu, &cpu->regi.c);
            break;
        case 0xBA:
            cp_register(cpu, &cpu->regi.d);
            break;
        case 0xBB:
            cp_register(cpu, &cpu->regi.e);
            break;
        case 0xBC:
            cp_register(cpu, &cpu->regi.h);
            break;
        case 0xBD:
            cp_register(cpu, &cpu->regi.l);
            break;
        case 0xBE:
            cp_indirect(cpu);
            break;
        case 0xBF:
            cp_register(cpu, &cpu->regi.a);
            break;
        case 0xC0:
            ret_cond(cpu, !(cpu->regi.f & ZERO_FLAG));
            break;
        case 0xC1:
            pop_register_pair(cpu, &cpu->regi.bc);
            break;
        case 0xC2:
            jump_cond(cpu, !(cpu->regi.f & ZERO_FLAG));
            break;
        case 0xC3:
            jump(cpu);
            break;
        case 0xC4:
            call_cond(cpu, !(cpu->regi.f & ZERO_FLAG));
            break;
        case 0xC5:
            push_register_pair(cpu, cpu->regi.bc);
            break;
        case 0xC6:
            add_immediate(cpu);
            break;
        case 0xC7:
            restart(cpu, 0x00);
            break;
        case 0xC8:
            ret_cond(cpu, cpu->regi.f & ZERO_FLAG);
            break;
        case 0xC9:
            ret(cpu);
            break;
        case 0xCA:
            jump_cond(cpu, cpu->regi.f & ZERO_FLAG);
            break;
        case 0xCB:
            execute_cb_instruction(cpu);
            break;
        case 0xCC:
            call_cond(cpu, cpu->regi.f & ZERO_FLAG);
            break;
        case 0xCD:
            call(cpu);
            break;
        case 0xCE:
            adc_immediate(cpu);
            break;
        case 0xCF:
            restart(cpu, 0x08);
            break;
        case 0xD0:
            ret_cond(cpu, !(cpu->regi.f & CARRY_FLAG));
            break;
        case 0xD1:
            pop_register_pair(cpu, &cpu->regi.de);
            break;
        case 0xD2:
            jump_cond(cpu, !(cpu->regi.f & CARRY_FLAG));
            break;
        case 0xD3:
            invalid_opcode(cpu);
            break;
        case 0xD4:
            call_cond(cpu, !(cpu->regi.f & CARRY_FLAG));
            break;
        case 0xD5:
            push_register_pair(cpu, cpu->regi.de);
            break;
        case 0xD6:
            sub_immediate(cpu);
            break;
        case 0xD7:
            restart(cpu, 0x10);
            break;
        case 0xD8:
            ret_cond(cpu, cpu->regi.f & CARRY_FLAG);
            break;
        case 0xD9:
            reti(cpu);
            break;
        case 0xDA:
            jump_cond(cpu, cpu->regi.f & CARRY_FLAG);
            break;
        case 0xDB:
            invalid_opcode(cpu);
            break;
        case 0xDC:
            call_cond(cpu, cpu->regi.f & CARRY_FLAG);
            break;
        case 0xDD:
            invalid_opcode(cpu);
            break;
        case 0xDE:
            sbc_immediate(cpu);
            break;
        case 0xDF:
            restart(cpu, 0x18);
            break;
        case 0xE0:
            ld_io_offset_store(cpu);
            break;
        case 0xE1:
            pop_register_pair(cpu, &cpu->regi.hl);
            break;
        case 0xE2:
            ld_io_offset_indirect_store(cpu);
            break;
        case 0xE3:
            invalid_opcode(cpu);
            break;
        case 0xE4:
            invalid_opcode(cpu);
            break;
        case 0xE5:
            push_register_pair(cpu, cpu->regi.hl);
            break;
        case 0xE6:
            and_immediate(cpu);
            break;
        case 0xE7:
            restart(cpu, 0x20);
            break;
        case 0xE8:
            add_sp_offset(cpu);
            break;
        case 0xE9:
            jump_indirect(cpu);
            break;
        case 0xEA:
            ld_absolute_store(cpu);
            break;
        case 0xEB:
            invalid_opcode(cpu);
            break;
        case 0xEC:
            invalid_opcode(cpu);
            break;
        case 0xED:
            invalid_opcode(cpu);
            break;
        case 0xEE:
            xor_immediate(cpu);
            break;
        case 0xEF:
            restart(cpu, 0x28);
            break;
        case 0xF0:
            ld_io_offset(cpu);
            break;
        case 0xF1:
            pop_register_pair(cpu, &cpu->regi.af);
            break;
        case 0xF2:
            ld_io_offset_indirect(cpu);
            break;
        case 0xF3:
            disable_interrupts(cpu);
            break;
        case 0xF4:
            invalid_opcode(cpu);
            break;
        case 0xF5:
            push_register_pair(cpu, cpu->regi.af);
            break;
        case 0xF6:
            or_immediate(cpu);
            break;
        case 0xF7:
            restart(cpu, 0x30);
            break;
        case 0xF8:
            ld_hl_sp_offset(cpu);
            break;
        case 0xF9:
            ld_sp_hl(cpu);
            break;
        case 0xFA:
            ld_absolute(cpu);
            break;
        case 0xFB:
            enable_interrupts(cpu);
            break;
        case 0xFC:
            invalid_opcode(cpu);
            break;
        case 0xFD:
            invalid_opcode(cpu);
            break;
        case 0xFE:
            cp_immediate(cpu);
            break;
        case 0xFF:
            restart(cpu, 0x38);
            break;
        default:
            printf("Unknown opcode: 0x%02X\n", cpu->cur_opcode);
            break;
    }
}