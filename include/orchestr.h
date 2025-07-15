#ifndef EMU_H_
#define EMU_H_

#include <common.h>
#include <debug.h>
#include <wram.h>
#include <cpu.h>
#include <cart.h>

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
} visual_t;

typedef struct {
    bool paused;
    bool running;
    int ticks;
} main_context;


#endif /* !EMU_H_ */