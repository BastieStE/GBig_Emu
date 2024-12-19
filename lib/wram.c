#include "wram.h"

void init_wram() {
    for (int bank = 0; bank < WRAM_NUM_BANKS; bank++) {
        for (int i = 0; i < WRAM_BANK_SIZE; i++) {
            wram[bank][i] = 0x00;  // Default initialization
        }
    }
    wram_bank = 1;  // Default bank is 1 for CGB
}

uint8_t read_wram(uint16_t address) {
    if (address >= 0xC000 && address <= 0xCFFF) {
        // Bank 0 (Always accessible)
        return wram[0][address - 0xC000];
    } else if (address >= 0xD000 && address <= 0xDFFF) {
        // Bank 1-7 (Switchable)
        return wram[wram_bank][address - 0xD000];
    } else if (address == 0xFF70) {
        // Return the current WRAM bank value
        return wram_bank | 0xF8; // Unused bits are set to 1
    }
    // Invalid address
    return 0xFF;
}

void write_wram(uint16_t address, uint8_t value) {
    if (address >= 0xC000 && address <= 0xCFFF) {
        // Bank 0 (Always accessible)
        wram[0][address - 0xC000] = value;
    } else if (address >= 0xD000 && address <= 0xDFFF) {
        // Bank 1-7 (Switchable)
        wram[wram_bank][address - 0xD000] = value;
    } else if (address == 0xFF70) {
        // Set the WRAM bank (0 maps to 1 in CGB mode)
        uint8_t new_bank = value & 0x07; // Mask to valid range (0-7)
        wram_bank = (new_bank == 0) ? 1 : new_bank;
    }
}
