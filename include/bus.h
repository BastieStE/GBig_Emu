#ifndef BUS_H_
#define BUS_H_

#include <common.h>
#include <cart.h>
#include <wram.h>

u8 bus_read(u16 address);
void bus_write(u16 address, u8 value);

u16 bus_read16(u16 address);
void bus_write16(u16 address, u16 value);

typedef struct
{
    u8 bios_enabled;
} bus_context;

#endif /* !BUS_H_ */
