#include <bus.h>

static bus_context ctx;

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

uint8_t bios_mem[0x100];

bus_context *bus_init(IO_register_t *io_regs, cpu_context *cpu_ctx)
{

    load_bios("gb_bios.bin", bios_mem);
    ctx.io_regs = io_regs;
    ctx.cpu = cpu_ctx;
    return &ctx;
}

static uint8_t io_read(uint16_t address) 
{
    if (address == 0xFF00) {
        return joypad_read(&ctx.io_regs->joypad);
    }
    else if (address >= 0xFF01 && address <= 0xFF02) {
      //  return serial_read(address);
    }
    else if (address >= 0xFF04 && address <= 0xFF07) {
      //  return timer_read(address);
    }
    else if (address == 0xFF0F) {
       return interrupt_flag_read(ctx.cpu);
    }
    // Audio (APU)
    else if (address >= 0xFF10 && address <= 0xFF3F) {
     //   return apu_read(address);
    }
    else if (address >= 0xFF40 && address <= 0xFF4B) {
       return ppu_read_register(address);
    }
    else if (address == 0xFF46) {
        return 0xFF;
    }
    return 0xFF;
}

static void io_write(uint16_t address, uint8_t value) 
{
    if (address == 0xFF00) {
       joypad_write(&ctx.io_regs->joypad, value);
    }
    else if (address >= 0xFF01 && address <= 0xFF02) {
      //  serial_write(address, value);
    }
    else if (address >= 0xFF04 && address <= 0xFF07) {
       // timer_write(address, value);
    }
    else if (address == 0xFF0F) {
       interrupt_flag_write(ctx.cpu, value);
    }
    // Audio (APU)
    else if (address >= 0xFF10 && address <= 0xFF3F) {
      //  apu_write(address, value);
    }
    else if (address >= 0xFF40 && address <= 0xFF4B) {
       ppu_write_register(address, value);
    }
    else if (address == 0xFF46) {
      //  dma_transfer(value);
    }
}

u8 bus_read(u16 address) 
{
    if (address < 0x900 && ctx.bios_enabled == 0x00) {
        printf("bus read in bios : adress %d \n", address);
        return bios_mem[address];
    } ///  Bios Managment
    
    if (address < 0x8000) {
        printf("bus read in cart : adress %d \n", address);
        return cart_read(address);         //ROM Data
    } else if (address < 0xA000) {
        return ppu_read(address);     //Char/Map Data
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
       return io_read(address);        //IO Registers...
    } else if (address == 0xFFFF) {
     //   return cpu_get_ie_register();        //CPU ENABLE REGISTER...
    }
    return ctx.hram[address - 0xFF80];
    return (0);
}

void bus_write(u16 address, u8 value) 
{
    if (address == 0xFF50) {
        debug_breakpoint = true;
        ctx.bios_enabled = value;  // BIOS disable control register
        if (value == 0x01) {
            printf("BIOS disabled, switching to cartridge ROM.\n");
        }
        return;
    } /// Bios Handling

    if (address < 0x900 && bios_mem[0xFF50] == 0x00) {
       bios_mem[address] = value;
    } ///  Bios Managment
    if (address < 0x8000) {
        cart_write(address, value);        //ROM Data
    } else if (address < 0xA000) {
        ppu_write(address, value);    //Char/Map Data
    } else if (address < 0xC000) {
        cart_write(address, value);        //EXT-RAM
    } else if (address < 0xE000) {
        write_wram(address, value);        //WRAM
    } else if (address < 0xFE00) {
        return;                            //reserved echo ram
    } else if (address < 0xFEA0) {
        ppu_write(address, value);     //PPU
    } else if (address < 0xFF00) {
        return;                            // reserved
    } else if (address < 0xFF80) {
       io_write(address, value);          //IO Registers
    } else if (address == 0xFFFF) {
       cpu_set_ie_register(ctx.cpu, value);        //CPU SET ENABLE REGISTER
    } else {
        ctx.hram[address - 0xFF80] = value;
        return;
    }
}

u16 bus_read16(u16 address) {
    u16 lo = bus_read(address);
    u16 hi = bus_read(address + 1);
    return lo | (hi << 8);
}


void bus_write16(u16 address, u16 value) {
    bus_write(address, value & 0xFF);              // Low byte at addr
    bus_write(address + 1, (value >> 8) & 0xFF);    // High byte at addr+1
}
