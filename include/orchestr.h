#ifndef EMU_H_
#define EMU_H_

#include <common.h>
#include <io_register.h>

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