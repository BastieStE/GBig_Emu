#include <ppu.h>

void ppu_init(ppu_context *ppu) {
    // Clear VRAM, OAM, and the framebuffer.
    memset(ppu->vram, 0, sizeof(ppu->vram));
    memset(ppu->oam, 0, sizeof(ppu->oam));
    memset(ppu->framebuffer, 0, sizeof(ppu->framebuffer));

    ppu->current_vram_bank = 0;

    // Set default I/O register values (adjust as needed)
    ppu->LCDC = 0x91;  // Example: LCD enabled with BG and window enabled.
    ppu->STAT = 0;
    ppu->SCY  = 0;
    ppu->SCX  = 0;
    ppu->LY   = 0;
    ppu->LYC  = 0;
    ppu->BGP  = 0xFC;  // Typical DMG default palette.
    ppu->OBP0 = 0xFF;
    ppu->OBP1 = 0xFF;
    ppu->WY   = 0;
    ppu->WX   = 0;
    // GBC-specific defaults:
    ppu->VBK   = 0;
    ppu->BGPI  = 0;
    ppu->BGPD  = 0;
    ppu->OBPI  = 0;
    ppu->OBPD  = 0;

    // Start in OAM search mode with a cleared cycle counter.
    ppu->mode = PPU_MODE_OAM;
    ppu->cycleCounter = 0;
}

void updatePPU(ppu_context *ppu, int cycles) {
    ppu->cycleCounter += cycles;

    switch (ppu->mode) {
        case PPU_MODE_OAM:
            if (ppu->cycleCounter >= 80) {
                ppu->cycleCounter -= 80;
                // Transition from OAM Search to Pixel Transfer
                ppu->mode = PPU_MODE_VRAM;
            }
            break;

        case PPU_MODE_VRAM:
            if (ppu->cycleCounter >= 172) {
                ppu->cycleCounter -= 172;
                // During Pixel Transfer, you render the scanline:
                renderScanline(ppu);
                // Transition to HBlank after rendering the scanline:
                ppu->mode = PPU_MODE_HBLANK;
            }
            break;

        case PPU_MODE_HBLANK:
            if (ppu->cycleCounter >= 204) {
                ppu->cycleCounter -= 204;
                // End of the scanline:
                ppu->LY++;  // Advance to the next scanline

                if (ppu->LY == 144) {
                    // Enter VBlank period and trigger the VBlank interrupt:
                    ppu->mode = PPU_MODE_VBLANK;
                    triggerVBlankInterrupt();
                } else {
                    // Start the next scanline’s OAM search:
                    ppu->mode = PPU_MODE_OAM;
                }
            }
            break;

        case PPU_MODE_VBLANK:
            if (ppu->cycleCounter >= 456) {
                ppu->cycleCounter -= 456;
                ppu->LY++;  // Advance in VBlank lines
                if (ppu->LY > 153) {
                    // VBlank period is over: reset scanline counter and start over.
                    ppu->LY = 0;
                    ppu->mode = PPU_MODE_OAM;
                }
            }
            break;
    }

}

void triggerVBlankInterrupt(void)
{
    NOT_YET
}

uint8_t getBackgroundPixel(ppu_context *ppu, int bgX, int bgY) {

    NOT_YET
}

void drawSprites(ppu_context *ppu, int line) {

    NOT_YET
    for (int i = 0; i < 40; i++) {
        // Each sprite uses 4 bytes in OAM.
        int spriteIndex = i * 4;
        uint8_t spriteY = ppu->oam[spriteIndex] - 16;  // Sprites are offset by 16 in Y.
        uint8_t spriteX = ppu->oam[spriteIndex + 1] - 8; // and by 8 in X.
        uint8_t tileNum = ppu->oam[spriteIndex + 2];
        uint8_t attributes = ppu->oam[spriteIndex + 3];
    }
}

void renderScanline(ppu_context *ppu) {
    int y = ppu->LY;  // Current scanline
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        // Calculate effective background coordinates (wrapping at 256):
        int bgX = (x + ppu->SCX) & 0xFF;
        int bgY = (y + ppu->SCY) & 0xFF;

        // Fetch the color index for the background pixel.
        uint8_t colorIndex = getBackgroundPixel(ppu, bgX, bgY);
        // Write the pixel to the framebuffer using the palette lookup.
        ppu->framebuffer[y * SCREEN_WIDTH + x] = ppu->palette[colorIndex];
    }

    // After drawing the background (and possibly window), draw sprites on top.
    drawSprites(ppu, y);
}

uint8_t ppu_read(ppu_context *ppu, uint16_t address) {
    if (address >= 0x8000 && address <= 0x9FFF) {
        // Reading from VRAM.
        uint16_t offset = address - 0x8000;
        return ppu->vram[ppu->current_vram_bank][offset];
    } else if (address >= 0xFE00 && address <= 0xFE9F) {
        // Reading from OAM memory.
        uint16_t offset = address - 0xFE00;
        return ppu->oam[offset];
    } else if (address >= 0xFF00 && address <= 0xFF7F) {
        // Reading from I/O registers.
        switch (address) {
            case 0xFF40: return ppu->LCDC;
            case 0xFF41: return ppu->STAT;
            case 0xFF42: return ppu->SCY;
            case 0xFF43: return ppu->SCX;
            case 0xFF44: return ppu->LY;   // Read-only; updated internally.
            case 0xFF45: return ppu->LYC;
            case 0xFF47: return ppu->BGP;
            case 0xFF48: return ppu->OBP0;
            case 0xFF49: return ppu->OBP1;
            case 0xFF4A: return ppu->WY;
            case 0xFF4B: return ppu->WX;
            // Add additional I/O registers as needed.
            default:
                // Return a default value or log an error.
                return 0;
        }
    }
    // Addresses not mapped to PPU memory return a default value.
    return 0;
}


void ppu_write(ppu_context *ppu, uint16_t address, uint8_t value) {
    if (address >= 0x8000 && address <= 0x9FFF) {
        // Writing to VRAM; account for active bank.
        uint16_t offset = address - 0x8000;
        ppu->vram[ppu->current_vram_bank][offset] = value;
    } else if (address >= 0xFE00 && address <= 0xFE9F) {
        // Writing to OAM memory.
        uint16_t offset = address - 0xFE00;
        ppu->oam[offset] = value;
    } else if (address >= 0xFF00 && address <= 0xFF7F) {
        // Writing to I/O registers.
        switch (address) {
            case 0xFF40:
                ppu->LCDC = value;
                // Optionally reset or update internal state when toggling LCD.
                break;
            case 0xFF41:
                // Only upper 5 bits writable; lower 3 bits are read-only.
                ppu->STAT = (ppu->STAT & 0x07) | (value & 0xF8);
                break;
            case 0xFF42: ppu->SCY  = value; break;
            case 0xFF43: ppu->SCX  = value; break;
            case 0xFF45: ppu->LYC  = value; break;
            case 0xFF47: ppu->BGP  = value; break;
            case 0xFF48: ppu->OBP0 = value; break;
            case 0xFF49: ppu->OBP1 = value; break;
            case 0xFF4A: ppu->WY   = value; break;
            case 0xFF4B: ppu->WX   = value; break;
            // Add additional I/O registers as needed.
            default:
                // Unhandled I/O register write: could log or ignore.
                break;
        }
    } else {
        // Addresses not handled by the PPU (if any) can be ignored or handled elsewhere.
    }
}

