#ifndef EMU_H_
#define EMU_H_

#include <common.h>

typedef struct {
    bool paused;
    bool running;
    int ticks;
} emu_context;

#endif /* !EMU_H_ */