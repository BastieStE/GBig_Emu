#include <bus.h>
#include <cart.h>
#include <wram.h>

// 0x0000 - 0x3FFF : ROM Bank 0 (fixed)
// 0x4000 - 0x7FFF : ROM Bank 1 (Switchable)
// 0x8000 - 0x97FF : Video RAM (VRAM)
// 0x9800 - 0x9BFF : BG Map 1
// 0x9C00 - 0x9FFF : BG Map 2
// 0xA000 - 0xBFFF : External RAM (cartridge)
// 0xC000 - 0xCFFF : RAM Bank 0
// 0xD000 - 0xDFFF : RAM Bank 1-7 - switchable - Color only
// 0xE000 - 0xFDFF : Reserved - Echo RAM
// 0xFE00 - 0xFE9F : Object Attribute Memory
// 0xFEA0 - 0xFEFF : Reserved - Unusable
// 0xFF00 - 0xFF7F : I/O Registers
// 0xFF80 - 0xFFFE : Zero Page

u8 bus_read(u16 address) {
    if (address < 0x8000) {
        return cart_read(address);         //ROM Data
    } else if (address < 0xA000) {
     //   return ppu_vram_read(address);     //Char/Map Data
    } else if (address < 0xC000) {
        return cart_read(address);         //Cartridge RAM
    } else if (address < 0xE000) {
        return read_wram(address);         //WRAM (Working RAM)
    } else if (address < 0xFE00) {
        return 0;
    } else if (address < 0xFEA0) {
        fprintf(stderr, "not ready\n"); return(255);        //OAM
    } else if (address < 0xFF00) {
        return 0;
    } else if (address < 0xFF80) {
     //   return io_read(address);        //IO Registers...
    } else if (address == 0xFFFF) {
     //   return cpu_get_ie_register();        //CPU ENABLE REGISTER...
    }
   // return hram_read(address);
}

/*la mémoire est gérée indépendament par chaque composant en interne, a voir si je préfere faire une 
grosse classe mémoire et faire passer tout les composants par cette classe pour accéder a la mémoire*/


void bus_write(u16 address, u8 value) {
    if (address < 0x8000) {
       cart_write(address, value);        //ROM Data
    } else if (address < 0xA000) {
     //   ppu_vram_write(address, value);    //Char/Map Data
    } else if (address < 0xC000) {
       cart_write(address, value);        //EXT-RAM
    } else if (address < 0xE000) {
       write_wram(address, value);        //WRAM
    } else if (address < 0xFE00) {
        return;                            //reserved echo ram
    } else if (address < 0xFEA0) {
     //   ppu_oam_write(address, value);     //PPU
    } else if (address < 0xFF00) {
        return;                            // reserved
    } else if (address < 0xFF80) {
      //  io_write(address, value);          //IO Registers
    } else if (address == 0xFFFF) {
      //  cpu_set_ie_register(value);        //CPU SET ENABLE REGISTER
    } else {
      //  hram_write(address, value);
    }
}

u16 bus_read16(u16 address) {
    u16 lo = bus_read(address);
    u16 hi = bus_read(address + 1);

    return lo | (hi << 8);
}

void bus_write16(u16 address, u16 value) {
    bus_write(address + 1, (value >> 8) & 0xFF);
    bus_write(address, value & 0xFF);
}