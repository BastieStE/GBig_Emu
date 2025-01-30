#include <common.h>

#define WRAM_BANK_SIZE 0x1000  // 4 KB
#define WRAM_NUM_BANKS 8       // Total banks in CGB mode


u8 read_wram(u16 address);
void write_wram(u16 address, u8 value);
void init_wram();
