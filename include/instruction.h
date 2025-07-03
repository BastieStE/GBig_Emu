#pragma once

#include <common.h>
#include "cpu.h"
#include "bus.h"
#include <cpu.h>

void execute_instruction();

void load_immediate(cpu_context *cpu, uint8_t *dst);
void load_immediate_16(cpu_context *cpu, uint16_t *dst);
void load_register(cpu_context *cpu, uint8_t *dst, uint8_t *src);
void add_register(cpu_context *cpu, uint8_t *dst, uint8_t *src);
void store_direct(cpu_context *cpu, uint8_t *src);
void store_indirect(cpu_context *cpu, uint8_t *src, uint16_t *address_reg);
void store_register(cpu_context *cpu, uint8_t *dst, uint8_t *src);
void load_indirect(cpu_context *cpu, uint8_t *dst, uint16_t *address_reg);
void load_direct(cpu_context *cpu, uint8_t *dst);
void and_indirect(cpu_context *cpu);
void and_register(cpu_context *cpu, uint8_t *dst, uint8_t *src);
void sbc_register(cpu_context *cpu, uint8_t *dst, uint8_t *src);
void sub_register(cpu_context *cpu, uint8_t *dst, uint8_t *src);
void adc_register(cpu_context *cpu, uint8_t *dst, uint8_t *src); 
void sbc_indirect(cpu_context *cpu);
void sub_indirect(cpu_context *cpu);
void adc_indirect(cpu_context *cpu);
void add_indirect(cpu_context *cpu, uint8_t *src);
void or_indirect(cpu_context *cpu);
void or_register(cpu_context *cpu, uint8_t *src);
void xor_indirect(cpu_context *cpu);
void xor_register(cpu_context *cpu, uint8_t *src);
void ld_absolute(cpu_context *cpu);
void ld_io_offset_store(cpu_context *cpu);
void ld_io_offset_indirect_store(cpu_context *cpu);
void ld_absolute_store(cpu_context *cpu);

void cp_register(cpu_context *cpu, uint8_t *src);
void cp_indirect(cpu_context *cpu);
void cp_immediate(cpu_context *cpu);
void xor_immediate(cpu_context *cpu);
void and_immediate(cpu_context *cpu);
void sbc_immediate(cpu_context *cpu);
void sub_immediate(cpu_context *cpu);
void adc_immediate(cpu_context *cpu);
void add_immediate(cpu_context *cpu);
void store_immediate(cpu_context *cpu);
void store_direct_16(cpu_context *cpu, uint16_t *reg);
void ld_sp_hl(cpu_context *cpu);
void ld_absolute(cpu_context *cpu);
void ld_io_offset_indirect(cpu_context *cpu);
void ld_io_offset(cpu_context *cpu);
void ld_hl_sp_offset(cpu_context *cpu);
void add_sp_offset(cpu_context *cpu);
void add_16(cpu_context *cpu, uint16_t *reg);
void increment_indirect(cpu_context *cpu);
void increment_16(cpu_context *cpu, uint16_t *reg);
void increment(cpu_context *cpu, uint8_t *reg);
void decrement_indirect(cpu_context *cpu);
void decrement_16(cpu_context *cpu, uint16_t *reg);
void decrement(cpu_context *cpu, uint8_t *reg);
void push_register_pair(cpu_context *cpu, uint16_t reg);
void stop(cpu_context *cpu);
void decimal_adjust(cpu_context *cpu);
void complement(cpu_context *cpu);
void set_carry(cpu_context *cpu);
void halt(cpu_context *cpu);
void complement_carry(cpu_context *cpu);
void call_cond(cpu_context *cpu, bool condition);
void pop_register_pair(cpu_context *cpu, uint16_t *reg);
void ret_cond(cpu_context *cpu, bool condition);
void ret(cpu_context *cpu);
void restart(cpu_context *cpu, uint8_t address);
void execute_cb_instruction(cpu_context *cpu);
void call(cpu_context *cpu);
void invalid_opcode(cpu_context *cpu);
void reti(cpu_context *cpu);
void or_immediate(cpu_context *cpu);
void enable_interrupts(cpu_context *cpu);
void disable_interrupts(cpu_context *cpu);
void jump_relative_cond(cpu_context *cpu, bool condition);
void jump_indirect(cpu_context *cpu);
void jump_cond(cpu_context *cpu, bool condition);
void jump_relative(cpu_context *cpu);
void jump(cpu_context *cpu);
void rotate_left_carry(cpu_context *cpu, uint8_t *reg);
void rotate_right_carry(cpu_context *cpu, uint8_t *reg);
void rotate_left(cpu_context *cpu, uint8_t *reg);
void rotate_right(cpu_context *cpu, uint8_t *reg);