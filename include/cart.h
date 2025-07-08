#pragma once

#include <common.h>

// Cartridge memory ranges
#define ROM_BANK_SIZE 0x4000  // 16 KB per ROM bank
#define RAM_BANK_SIZE 0x2000  // 8 KB per RAM bank


typedef struct {
    u8 entry[4];
    u8 logo[0x30];

    char title[16];
    u16 new_lic_code;
    u8 sgb_flag;
    u8 type;
    u8 rom_size;
    u8 ram_size;
    u8 dest_code;
    u8 lic_code;
    u8 version;
    u8 checksum;
    u16 global_checksum;
} rom_header;


typedef struct {
    char filename[1024];
    uint32_t rom_size;
    uint8_t *rom_data;
    uint8_t *ram_data;
    uint32_t ram_size;
    uint8_t rom_bank;
    uint8_t ram_bank;
    bool ram_enabled;
    rom_header *header;
} cart_context;

bool cart_load(char *cart);
uint8_t cart_read(uint16_t address);
void cart_write(uint16_t address, uint8_t value);
void load_bios(const char *bios_path, u8 *bios_dt);