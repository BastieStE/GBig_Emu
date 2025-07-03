#ifndef EMU_H_
#define EMU_H_

#include <common.h>
#include <io_register.h>
#include <wram.h>
#include <cpu.h>
#include <cart.h>
#include <cpu.h>
#include <ppu.h>

typedef struct {
    SDL_Window *window;
    SDL_Renderer* renderer;
    SDL_Event event;
} visual_t;

typedef struct {
    bool paused;
    bool running;
    int ticks;
} main_context;

typedef struct {
    int speed;
    int step;
    bool is_on;
    bool is_display_register;
} debug_ctx;



#endif /* !EMU_H_ */