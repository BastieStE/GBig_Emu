#include "wram.h"

static u8 wram[0x2000];  // 8 banks of 4 KB each

u8 read_wram(u16 address) {
    if (address >= 0xC000 && address <= 0xDFFF) {
        return wram[address - 0xC000];
    } else if (address >= 0xE000 && address <= 0xFDFF) {
        return wram[address - 0xE000];  //echo ram
    }
    return 0xFF; // Invalid address
}

void write_wram(u16 address, u8 value) {
    if (address >= 0xC000 && address <= 0xDFFF) {
        wram[address - 0xC000] = value;
    } else if (address >= 0xE000 && address <= 0xFDFF) {
        wram[address - 0xE000] = value; // echo ram just in case
    }
}
