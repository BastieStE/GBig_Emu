#include <ppu.h>

static ppu_context ppu;

void ppu_init() {
    // Clear VRAM, OAM, and the framebuffer.
    memset(ppu.vram, 0, sizeof(ppu.vram));
    memset(ppu.oam, 0, sizeof(ppu.oam));
    memset(ppu.framebuffer, 0, sizeof(ppu.framebuffer));

    ppu.current_vram_bank = 0;

    // Set default I/O register values (adjust as needed)
    ppu.LCDC = 0x91;  // Example: LCD enabled with BG and window enabled.
    ppu.STAT = 0;
    ppu.SCY  = 0;
    ppu.SCX  = 0;
    ppu.LY   = 0;
    ppu.LYC  = 0;
    ppu.BGP  = 0xFC;  // Typical DMG default palette.
    ppu.OBP0 = 0xFF;
    ppu.OBP1 = 0xFF;
    ppu.WY   = 0;
    ppu.WX   = 0;
    // GBC-specific defaults:
    ppu.VBK   = 0;
    ppu.BGPI  = 0;
    ppu.BGPD  = 0;
    ppu.OBPI  = 0;
    ppu.OBPD  = 0;

    // Start in OAM search mode with a cleared cycle counter.
    ppu.mode = PPU_MODE_OAM;
    ppu.cycleCounter = 0;
}

void triggerVBlankInterrupt(void)
{
    NOT_YET
}

uint8_t getBackgroundPixel(int bgX, int bgY) {
    // If BG disabled (LCDC bit 0 = 0), return color 0 (white).
    if (!(ppu.LCDC & 0x01)) return 0;
    // Select background tile map base offset in VRAM (0x9800 or 0x9C00).
    uint16_t mapOffset = (ppu.LCDC & 0x08) ? 0x1C00 : 0x1800;
    // Compute tile indices (32x32 map, each tile is 8x8).
    int tileX = (bgX / 8) & 0x1F;
    int tileY = (bgY / 8) & 0x1F;
    uint16_t tileIndexAddr = mapOffset + tileY * 32 + tileX;
    uint8_t tileNum = ppu.vram[0][tileIndexAddr];
    // Determine tile data address (two addressing modes).
    uint16_t tileDataAddr;
    if (ppu.LCDC & 0x10) {
        // $8000 method (unsigned)
        tileDataAddr = tileNum * 16;
    } else {
        // $8800 method (signed)
        tileDataAddr = (int8_t)tileNum * 16 + 0x1000;
    }
    // Fetch the two bytes for the scanline within the tile.
    int line = bgY % 8;
    uint8_t byte1 = ppu.vram[ppu.current_vram_bank][tileDataAddr + 2*line];
    uint8_t byte2 = ppu.vram[ppu.current_vram_bank][tileDataAddr + 2*line + 1];
    // Extract the two bits for this pixel.
    int bit = 7 - (bgX % 8);
    uint8_t colorBit0 = (byte1 >> bit) & 1;
    uint8_t colorBit1 = (byte2 >> bit) & 1;
    uint8_t colorIndex = colorBit0 | (colorBit1 << 1);
    return colorIndex;
}

void drawSprites(int line, uint8_t *bgLine) {
    // If OBJ (sprite) disabled (LCDC bit 1=0), do nothing.
    if (!(ppu.LCDC & 0x02)) return;
    int spriteHeight = (ppu.LCDC & 0x04) ? 16 : 8;
    int spritesDrawn = 0;
    static const uint8_t dmgPalette[4] = { 255, 192, 96, 0 };
    for (int i = 0; i < 40 && spritesDrawn < 10; i++) {
        int si = i * 4;
        int spriteY = ppu.oam[si] - 16;
        int spriteX = ppu.oam[si+1] - 8;
        uint8_t tile = ppu.oam[si+2];
        uint8_t attr = ppu.oam[si+3];
        // Check vertical range
        if (line < spriteY || line >= spriteY + spriteHeight) continue;
        // Check horizontal range (skip entirely off-screen sprites)
        if (spriteX >= SCREEN_WIDTH || spriteX + 7 < 0) continue;
        // Which line of the tile to draw
        int yInSprite = line - spriteY;
        if (attr & 0x40) {  // Y-flip
            yInSprite = spriteHeight - 1 - yInSprite;
        }
        // Handle 8x16 mode: use even tile number for top half
        if (spriteHeight == 16) {
            tile &= 0xFE;
            if (yInSprite >= 8) {
                tile++;
                yInSprite -= 8;
            }
        }
        // Fetch sprite tile data (sprites always use $8000 method, bank 0)
        uint16_t tileAddr = tile * 16;
        uint8_t byte1 = ppu.vram[0][tileAddr + 2*yInSprite];
        uint8_t byte2 = ppu.vram[0][tileAddr + 2*yInSprite + 1];
        for (int px = 0; px < 8; px++) {
            int xPix = spriteX + px;
            if (xPix < 0 || xPix >= SCREEN_WIDTH) continue;
            int bit = (attr & 0x20) ? px : (7 - px);  // X-flip?
            uint8_t c0 = (byte1 >> bit) & 1;
            uint8_t c1 = (byte2 >> bit) & 1;
            uint8_t colorIndex = c0 | (c1 << 1);
            if (!colorIndex) continue;  // transparent pixel
            // Sprite priority: if bit7=1 and BG pixel nonzero, skip sprite
            if ((attr & 0x80) && bgLine[xPix]) continue;
            // Choose sprite palette
            uint8_t palette = (attr & 0x10) ? ppu.OBP1 : ppu.OBP0;
            uint8_t finalColor = (palette >> (colorIndex * 2)) & 0x03;
            ppu.framebuffer[line * SCREEN_WIDTH + xPix] = dmgPalette[finalColor];
        }
        spritesDrawn++;
    }
}

void renderScanline() {
    int y = ppu.LY;
    uint8_t bgLine[SCREEN_WIDTH];
    static const uint8_t dmgPalette[4] = { 255, 192, 96, 0 };
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        int bgX = (x + ppu.SCX) & 0xFF;
        int bgY = (y + ppu.SCY) & 0xFF;
        uint8_t colorIndex = getBackgroundPixel(bgX, bgY);
        bgLine[x] = colorIndex;
        // Apply background palette BGP
        uint8_t paletteIndex = (ppu.BGP >> (colorIndex * 2)) & 0x03;
        ppu.framebuffer[y * SCREEN_WIDTH + x] = dmgPalette[paletteIndex];
    }
    // Draw sprites over the background line
    drawSprites(y, bgLine);
}

uint8_t ppu_read_register( uint16_t address) {
 switch (address) {
            case 0xFF40: return ppu.LCDC;
            case 0xFF41: return ppu.STAT;
            case 0xFF42: return ppu.SCY;
            case 0xFF43: return ppu.SCX;
            case 0xFF44: return ppu.LY;   // Read-only; updated internally.
            case 0xFF45: return ppu.LYC;
            case 0xFF47: return ppu.BGP;
            case 0xFF48: return ppu.OBP0;
            case 0xFF49: return ppu.OBP1;
            case 0xFF4A: return ppu.WY;
            case 0xFF4B: return ppu.WX;
            // Add additional I/O registers as needed.
            default:
                // Return a default value or log an error.
                return 0;
        }
}
void ppu_write_register(uint16_t addr, uint8_t value) {
    switch(addr) {
        case 0xFF40: // LCDC
            ppu.LCDC = value;
            // Update mode, enable/disable LCD, etc.
            break;
        case 0xFF41: // STAT
            // Only bits 3-6 writable, bits 0-2 read-only
            ppu.STAT = (ppu.STAT & 0x07) | (value & 0xF8);
            break;
        case 0xFF42: // SCY
            ppu.SCY = value;
            break;
        case 0xFF43: // SCX
            ppu.SCX = value;
            break;
        case 0xFF44: // LY - read-only, ignore writes
            break;
        case 0xFF45: // LYC
            ppu.LYC = value;
            break;
        case 0xFF47: // BGP
            ppu.BGP = value;
            // Optionally recalc BG palette colors
            break;
        case 0xFF48: // OBP0
            ppu.OBP0 = value;
            // Optionally recalc OBJ palette 0 colors
            break;
        case 0xFF49: // OBP1
            ppu.OBP1 = value;
            // Optionally recalc OBJ palette 1 colors
            break;
        case 0xFF4A: // WY
            ppu.WY = value;
            break;
        case 0xFF4B: // WX
            ppu.WX = value;
            break;
        default:
            // Ignore writes to other addresses (GBC only registers)
            break;
    }
}


uint8_t ppu_read( uint16_t address) {
    if (address >= 0x8000 && address <= 0x9FFF) {
        // Reading from VRAM.
        uint16_t offset = address - 0x8000;
        return ppu.vram[ppu.current_vram_bank][offset];
    } else if (address >= 0xFE00 && address <= 0xFE9F) {
        // Reading from OAM memory.
        uint16_t offset = address - 0xFE00;
        return ppu.oam[offset];
    } else if (address >= 0xFF00 && address <= 0xFF7F) {
        // Reading from I/O registers.
            return ppu_read_register(address);
        }
    // return a default value.
    return 0;
}


void ppu_write( uint16_t address, uint8_t value) {
    if (address >= 0x8000 && address <= 0x9FFF) {
        // Writing to VRAM; account for active bank.
        uint16_t offset = address - 0x8000;
        ppu.vram[ppu.current_vram_bank][offset] = value;
    } else if (address >= 0xFE00 && address <= 0xFE9F) {
        // Writing to OAM memory.
        uint16_t offset = address - 0xFE00;
        ppu.oam[offset] = value;
    } else if (address >= 0xFF00 && address <= 0xFF7F) {
        // Writing to I/O registers.
        ppu_write_register(address, value);
    } else {
        // Addresses not handled by the PPU (if any) can be ignored or handled elsewhere.
    }
}



void updatePPU(int cycles) {
    puts("UPDATE PPU");
    ppu.cycleCounter += cycles;
    switch (ppu.mode) {
        case PPU_MODE_OAM:
            if (ppu.cycleCounter >= 80) {
                ppu.cycleCounter -= 80;
                // Transition from OAM Search to Pixel Transfer
                ppu.mode = PPU_MODE_VRAM;
            }
            break;

        case PPU_MODE_VRAM:
            if (ppu.cycleCounter >= 172) {
                ppu.cycleCounter -= 172;
                renderScanline(ppu);
                ppu.mode = PPU_MODE_HBLANK;
            }
            break;

        case PPU_MODE_HBLANK:
            if (ppu.cycleCounter >= 204) {
                ppu.cycleCounter -= 204;
                // End of the scanline:
                ppu.LY++;  // Advance to the next scanline

                if (ppu.LY == 144) {
                    // Enter VBlank period and trigger the VBlank interrupt:
                    ppu.mode = PPU_MODE_VBLANK;
                    triggerVBlankInterrupt();
                } else {
                    ppu.mode = PPU_MODE_OAM;
                }
            }
            break;

        case PPU_MODE_VBLANK:
            if (ppu.cycleCounter >= 456) {
                ppu.cycleCounter -= 456;
                ppu.LY++;  // Advance in VBlank lines
                if (ppu.LY > 153) {
                    // VBlank period is over: reset scanline counter and start over.
                    ppu.LY = 0;
                    ppu.mode = PPU_MODE_OAM;
                }
            }
            break;
    }

}