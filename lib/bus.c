#include <bus.h>
#include <cart.h>

u8 bus_read(u16 address) {
    if (address < 0x8000) {
        //ROM Data
        return cart_read(address);
    }

    printf("UNSUPPORTED bus_read(%04X)\n", address);
}

void bus_write(u16 address, u8 value) {
    if (address < 0x8000) {
        //ROM Data
        cart_write(address, value);
        return;
    }

    printf("UNSUPPORTED bus_write(%04X)\n", address);
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