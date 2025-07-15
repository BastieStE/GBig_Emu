#ifndef PPU_H_
#define PPU_H_

#include <common.h>


typedef enum {
    PPU_MODE_HBLANK = 0,
    PPU_MODE_VBLANK = 1,
    PPU_MODE_OAM    = 2,
    PPU_MODE_VRAM   = 3
} PPU_Mode;

typedef struct {

    uint8_t vram[2][8192];
    uint8_t current_vram_bank;
    uint8_t oam[160];

    uint32_t palette[4];
    
    uint32_t framebuffer[144][160]; // 32-bit ARGB or RGBA pixels


    //  I/O Registers 
    uint8_t LCDC;   // LCD Control (0xFF40)
    uint8_t STAT;   // LCD Status (0xFF41)
    uint8_t SCY;    // Scroll Y (0xFF42)
    uint8_t SCX;    // Scroll X (0xFF43)
    uint8_t LY;     // Current scanline (0xFF44) - read-only
    uint8_t LYC;    // LY Compare (0xFF45)
    uint8_t BGP;    // BG Palette Data (0xFF47)
    uint8_t OBP0;   // Object Palette 0 (0xFF48)
    uint8_t OBP1;   // Object Palette 1 (0xFF49)
    uint8_t WY;     // Window Y Position (0xFF4A)
    uint8_t WX;     // Window X Position (0xFF4B)
    // GBC color ones
    uint8_t VBK;    // VRAM Bank (0xFF4F)
    uint8_t BGPI;   // BG Palette Index (0xFF68)
    uint8_t BGPD;   // BG Palette Data (0xFF69)
    uint8_t OBPI;   // Sprite Palette Index (0xFF6A)
    uint8_t OBPD;   // Sprite Palette Data (0xFF6B)

    //  Internal PPU State 
    PPU_Mode mode;       // Current PPU mode (OAM, VRAM, HBlank, VBlank)
    int cycleCounter;    // Accumulated cycles within the current mode
} ppu_context;



ppu_context *ppu_init();
void ppu_tick();
u8 ppu_read(u16 address);
void ppu_write(u16 address, u8 value);
uint8_t ppu_read_register(uint16_t address);
void ppu_write_register(uint16_t addr, uint8_t value);
void updatePPU(int cycles);


#endif /* !PPU_H_ */