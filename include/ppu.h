#include <common.h>

void ppu_init();
void ppu_tick();
u8 ppu_vram_read(u16 address);
void ppu_vram_write(u16 address, u8 value);