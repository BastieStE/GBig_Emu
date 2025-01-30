#pragma once
#include "common.h"
#include "cpu.h"
#include "bus.h"

void load_immediate(cpu_context *cpu, uint8_t *dst);
void load_register(cpu_context *cpu, uint8_t *dst, uint8_t *src); // takes 4 cycles (to implement)
void load_direct(cpu_context *cpu, uint8_t *dst); // takes 16 cycles (to implement)
void load_indirect(cpu_context *cpu, uint8_t *dst, uint16_t *address_reg); // takes 8 cycles (to implement)
void store_direct(cpu_context *cpu, uint8_t *src);// takes 16 cycles (to implement)
