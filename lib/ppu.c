#include <ppu.h>

static ppu_context ppu;

ppu_context *ppu_init() {
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
    return &ppu;
}

void triggerVBlankInterrupt(void)
{
    NOT_YET
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
        puts("writing on VRAM");
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


uint8_t get_tile_pixel_color(uint8_t tile_index, uint8_t x, uint8_t y) {

    uint16_t tile_addr = 0x8000 + (tile_index * 16);
    uint8_t low_byte  = ppu_read(tile_addr + y * 2);
    uint8_t high_byte = ppu_read(tile_addr + y * 2 + 1);

    uint8_t bit = 7 - x;
    uint8_t lo = (low_byte >> bit) & 0x01;
    uint8_t hi = (high_byte >> bit) & 0x01;

    return (hi << 1) | lo; // 2-bit color index (0–3)
}

uint32_t get_color_from_palette(uint8_t color_index, uint8_t BGP) {
    uint8_t palette_bits = (BGP >> (color_index * 2)) & 0x03;

    // Map to actual grayscale colors (ARGB format here)
    switch (palette_bits) {
        case 0: return 0xFFFFFFFF; // White
        case 1: return 0xAAAAAAFF; // Light Gray
        case 2: return 0x555555FF; // Dark Gray
        case 3: return 0x000000FF; // Black
        default: return 0xFF00FFFF; // Magenta = error
    }
}

void draw_background_scanline(uint8_t line) {
    uint8_t SCX = ppu_read(0xFF43); // Scroll X
    uint8_t SCY = ppu_read(0xFF42); // Scroll Y
    uint8_t BGP = ppu_read(0xFF47); // Background palette

    uint16_t tilemap_base = 0x9800;

    for (uint8_t x = 0; x < 160; x++) {
        // Actual BG pixel position
        uint8_t bg_x = (x + SCX) & 0xFF;
        uint8_t bg_y = (line + SCY) & 0xFF;

        // Which tile we're in
        uint8_t tile_col = bg_x / 8;
        uint8_t tile_row = bg_y / 8;

        // Pixel within tile
        uint8_t tile_x = bg_x % 8;
        uint8_t tile_y = bg_y % 8;

        // Read tile index from tile map
        uint16_t tilemap_index = tilemap_base + (tile_row * 32) + tile_col;
        uint8_t tile_index = ppu_read(tilemap_index); // Unsigned (BIOS uses 0x8000)

        // Get color index (0–3) from tile data
        uint8_t color_index = get_tile_pixel_color(tile_index, tile_x, tile_y);

        // Map color index via palette
        uint32_t color = get_color_from_palette(color_index, BGP);

        ppu.framebuffer[line][x] = color;
    }
}


void updatePPU(int cycles) {
    
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
                draw_background_scanline(ppu.LY);
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