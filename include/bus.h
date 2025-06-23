#ifndef BUS_H_
#define BUS_H_

#include <common.h>
#include <cart.h>
#include <wram.h>
#include <orchestr.h>

u8 bus_read(u16 address);
void bus_write(u16 address, u8 value);

u16 bus_read16(u16 address);
void bus_write16(u16 address, u16 value);
void bus_init(IO_register_t *io_regs);

typedef struct
{
    u8 bios_enabled;
    IO_register_t *io_regs
} bus_context;

#endif /* !BUS_H_ */
