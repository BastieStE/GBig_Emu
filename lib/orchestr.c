#include <orchestr.h>

visual_t init_SDL(visual_t visual)
{
    visual.window = SDL_CreateWindow(
        "My Game", 
        SDL_WINDOWPOS_CENTERED, // X position
        SDL_WINDOWPOS_CENTERED, // Y position
        (SCREEN_WIDTH *5),                   // width
        (SCREEN_HEIGHT * 5),                    // height
        SDL_WINDOW_SHOWN
    );    
    if (!visual.window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }
    return visual;
}

static void ctx_init(main_context *ctx)
{
    ctx->paused = false;
    ctx->running = true;
    ctx->ticks = 0;
}

bool handle_SDL(visual_t vis)
{
    while (SDL_PollEvent(&vis.event)) {
        if (vis.event.type == SDL_QUIT) {
            return false;
        }
    }
    SDL_SetRenderDrawColor(vis.renderer, 0, 0, 0, 255);
    SDL_RenderClear(vis.renderer);

    // Draw things
    SDL_RenderPresent(vis.renderer);
    return true;
}


void render_screen(visual_t *vis, ppu_context *ppu) {
    // Create the texture once
    static SDL_Texture *texture = NULL;
    if (!texture) {
        texture = SDL_CreateTexture(
            vis->renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_STREAMING,
            SCREEN_WIDTH,
            SCREEN_HEIGHT
        );
    }

    // Upload framebuffer into the texture
    void *pixels;
    int   pitch;
    SDL_LockTexture(texture, NULL, &pixels, &pitch);
    // ppu->framebuffer is uint32_t[SCREEN_WIDTH*SCREEN_HEIGHT] in RGBA8888
    memcpy(pixels, ppu->framebuffer, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint32_t));
    SDL_UnlockTexture(texture);

    // Draw it
    SDL_RenderClear(vis->renderer);
    // Stretch to fill the window (or you can set a dst rect for 1× scale)
    SDL_RenderCopy(vis->renderer, texture, NULL, NULL);
    SDL_RenderPresent(vis->renderer);
    free(pixels);
}

int run(int argc, char **argv) {

    main_context ctx;
    ppu_context ppu;  // Create an instance of PPU
    IO_register_t io_regs;
    visual_t screen;
    cpu_context cpu = cpu_init();
    bus_context *bus = bus_init(&io_regs, &cpu);
    debug_ctx debug = init_debug(argc, argv, bus, &ppu, &cpu);

    // Initialize the main context, CPU, WRAM, and PPU.
    cart_load("Tetris.gb"); // load cart
    ctx_init(&ctx); // init orchestrator context
    cpu_init(); // init cpu
    ppu_init(&ppu); // init PPU state
    screen = init_SDL(screen);
    puts("end of init");


    while(ctx.running) {
        if (ctx.paused) {
            puts("pause not implemented");
        }

        // Execute the next CPU instruction and obtain the number of cycles taken.
        int cycles = cpu_step(&cpu);

        if (cycles < 0) {
            puts("CPU Stopped");
            return -3;
        }

        // Update the PPU with the cycles executed by the CPU.
        //updatePPU(cycles);
        ctx.running = handle_SDL(screen);
        //render_screen(&screen, &ppu);

        if (debug.is_on == false)
            poll_sdl_input(&io_regs.joypad);
        ctx.ticks++;
        
        // debug print and leave if Q pressed
        if (debug_step(&debug, &io_regs)) break;


    }
    return 0;
}

int main(int argc, char **argv) {
    run(argc, argv);
    return 77;
}
