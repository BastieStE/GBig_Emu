#include <common.h>

typedef struct {
    uint8_t oam[0xA0];     // Object Attribute Memory (OAM)
    
    uint8_t lcdc;  // LCD Control Register (0xFF40)
    uint8_t stat;  // LCD Status Register (0xFF41)
    uint8_t scy;   // Scroll Y (0xFF42)
    uint8_t scx;   // Scroll X (0xFF43)
    uint8_t ly;    // LCD Y-Coordinate (0xFF44)
    uint8_t lyc;   // LY Compare (0xFF45)
    uint8_t bgp;   // BG Palette (DMG mode, 0xFF47)
    uint8_t obp0;  // Sprite Palette 0 (0xFF48)
    uint8_t obp1;  // Sprite Palette 1 (0xFF49)
    uint8_t wy;    // Window Y (0xFF4A)
    uint8_t wx;    // Window X (0xFF4B)

    int mode;      // LCD Mode (0: H-Blank, 1: V-Blank, 2: OAM Search, 3: Drawing)
    int mode_clock; // Timing for mode switching
} ppu_context;

void ppu_init();
void ppu_tick();
u8 ppu_vram_read(u16 address);
void ppu_vram_write(u16 address, u8 value);