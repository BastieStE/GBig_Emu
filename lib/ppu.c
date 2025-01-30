#include <ppu.h>

void ppu_init(ppu_context *ppu) {
    memset(ppu->oam, 0, sizeof(ppu->oam));

    ppu->lcdc = 0x91;  // LCD enabled, BG display enabled (default)
    ppu->stat = 0x85;  // Default status register
    ppu->scy = 0;
    ppu->scx = 0;
    ppu->ly = 0;
    ppu->lyc = 0;
    ppu->bgp = 0xFC;  // Default BG palette (DMG mode)
    ppu->obp0 = 0xFF; // Default sprite palette 0
    ppu->obp1 = 0xFF; // Default sprite palette 1
    ppu->wy = 0;
    ppu->wx = 0;

    ppu->mode = 2;  // PPU starts in OAM search mode
    ppu->mode_clock = 0;
}

void ppu_tick()
{
    return;
}


u8 ppu_vram_read(u16 address)
{
    NOT_YET
}

void ppu_vram_write(u16 address, u8 value)
{
    NOT_YET
}