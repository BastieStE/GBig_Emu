#ifndef IOREG_H_
#define IOREG_H_

#include <common.h>
#include <SDL2/SDL.h>

#define JOYP_RIGHT  (1 << 0)
#define JOYP_LEFT   (1 << 1)
#define JOYP_UP     (1 << 2)
#define JOYP_DOWN   (1 << 3)
#define JOYP_A      (1 << 4)
#define JOYP_B      (1 << 5)
#define JOYP_SELECT (1 << 6)
#define JOYP_START  (1 << 7)

typedef struct {
    u8 joyp;       // select bits
    u8 hard;      // pressed inputs
} joypad_t;

typedef struct IO_register
{
    joypad_t joypad;
}IO_register_t;

u8 joypad_read(joypad_t *pad);
void joypad_write(joypad_t *pad, u8 val) ;


#endif /* !IOREG_H_ */