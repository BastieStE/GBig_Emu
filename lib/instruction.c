#include <instruction.h>
#include <cpu.h>


void execute_instruction(cpu_contex *cpu) {
    switch (cpu->cur_opcode) {
        // Immediate Addressing
        case 0x3E:  // LD A, u8
            load_immediate(cpu, cpu->regi.a);
            break;
        case 0x06:  // LD B, u8
            load_immediate(cpu, cpu->regi.b);
            break;
        case 0x0E:  // LD C, u8
            load_immediate(cpu, cpu->regi.c);
            break;
        case 0x16:  // LD D, u8
            load_immediate(cpu, cpu->regi.d);
            break;
        case 0x1E:  // LD E, u8
            load_immediate(cpu, cpu->regi.e);
            break;
        case 0x26:  // LD H, u8
            load_immediate(cpu, cpu->regi.h);
            break;
        case 0x2E:  // LD L, u8
            load_immediate(cpu, cpu->regi.l);
            break;

        // Register Addressing
        case 0x78:  // LD A, B
            load_register(cpu, cpu->regi.a, cpu->regi.b);
            break;
        case 0x79:  // LD A, C
            load_register(cpu, cpu->regi.a, cpu->regi.c);
            break;
        case 0x7A:  // LD A, D
            load_register(cpu, cpu->regi.a, cpu->regi.d);
            break;
        case 0x7B:  // LD A, E
            load_register(cpu, cpu->regi.a, cpu->regi.e);
            break;
        case 0x7C:  // LD A, H
            load_register(cpu, cpu->regi.a, cpu->regi.h);
            break;
        case 0x7D:  // LD A, L
            load_register(cpu, cpu->regi.a, cpu->regi.l);
            break;
        case 0x7E:  // LD A, (HL)
            load_indirect(cpu, cpu->regi.a, cpu->regi.hl);
            break;
        case 0x47:  // LD B, A
            load_register(cpu, cpu->regi.b, cpu->regi.a);
            break;
        // ... (add other LD register-register combinations as needed)

        // Direct Addressing
        case 0xFA:  // LD A, (a16)
            load_direct(cpu, cpu->regi.a);
            break;
        case 0xEA:  // LD (a16), A
            store_direct(cpu, cpu->regi.a);
            break;

        // Indirect Addressing
        case 0x0A:  // LD A, (BC)
            load_indirect(cpu, cpu->regi.a, cpu->regi.bc);
            break;
        case 0x1A:  // LD A, (DE)
            load_indirect(cpu, cpu->regi.a, cpu->regi.de);
            break;
        case 0x02:  // LD (BC), A
            store_indirect(cpu, cpu->regi.a, cpu->regi.bc);
            break;
        case 0x12:  // LD (DE), A
            store_indirect(cpu, cpu->regi.a, cpu->regi.de);
            break;

        // Special Indirect (HLI and HLD)
        case 0x22:  // LD (HL+), A
            store_indirect(cpu, cpu->regi.a, cpu->regi.hl);
            cpu->regi.hl++;
            break;
        case 0x2A:  // LD A, (HL+)
            load_indirect(cpu, cpu->regi.a, cpu->regi.hl);
            cpu->regi.hl++;
            break;
        case 0x32:  // LD (HL-), A
            store_indirect(cpu, cpu->regi.a, cpu->regi.hl);
            cpu->regi.hl--;
            break;
        case 0x3A:  // LD A, (HL-)
            load_indirect(cpu, cpu->regi.a, cpu->regi.hl);
            cpu->regi.hl--;
            break;

        default:
            printf("Unknown opcode: 0x%02X\n", cpu->cur_opcode);
            break;
    }
}