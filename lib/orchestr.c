#include <orchestr.h>

visual_t init_SDL(visual_t visual)
{
    visual.window = SDL_CreateWindow(
        "My Game", 
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        (SCREEN_WIDTH * 5),
        (SCREEN_HEIGHT * 5),
        SDL_WINDOW_SHOWN
    );    
    if (!visual.window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    visual.renderer = SDL_CreateRenderer(visual.window, -1, SDL_RENDERER_ACCELERATED);
    if (!visual.renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(visual.window);
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

bool handle_SDL(visual_t *vis)
{
    while (SDL_PollEvent(&vis->event)) {
        if (vis->event.type == SDL_QUIT) {
            return false;
        }
    }
    SDL_SetRenderDrawColor(vis->renderer, 0, 0, 0, 255);

    // Draw things
    SDL_RenderPresent(vis->renderer);
    return true;
}

void render_screen(visual_t *vis, ppu_context *ppu) {
    static SDL_Texture *texture = NULL;
    if (!texture) {
        texture = SDL_CreateTexture(vis->renderer,
                                   SDL_PIXELFORMAT_RGBA8888,
                                   SDL_TEXTUREACCESS_STREAMING,
                                   SCREEN_WIDTH, SCREEN_HEIGHT);
        if (!texture) {
            fprintf(stderr, "SDL_CreateTexture Error: %s\n", SDL_GetError());
            return;
        }
    }

    // Update the texture with the framebuffer pixels
    // (pitch = SCREEN_WIDTH * sizeof(uint32_t) bytes)
    SDL_UpdateTexture(texture, NULL, ppu->framebuffer,
                      SCREEN_WIDTH * sizeof(uint32_t));

    // Draw the texture to the window
    SDL_RenderClear(vis->renderer);
    SDL_RenderCopy(vis->renderer, texture, NULL, NULL);
    SDL_RenderPresent(vis->renderer);
}


int run(int argc, char **argv) {

    main_context ctx;
    ppu_context *ppu = ppu_init();  // Create an instance of PPU and init PPU state
    IO_register_t io_regs;
    visual_t screen;
    cpu_context cpu = cpu_init();
    bus_context *bus = bus_init(&io_regs, &cpu);
    debug_ctx debug = init_debug(argc, argv, bus, ppu, &cpu);

    // Initialize the main context, CPU, WRAM, and PPU.
    cart_load("Tetris.gb"); // load cart
    ctx_init(&ctx); // init orchestrator context
    cpu_init(); // init cpu 
    screen = init_SDL(screen);
    puts("end of init");


    while(ctx.running) {
        if (ctx.paused) {
            puts("pause not implemented");
        }
        if (debug_step(&debug, &io_regs)) break;
        // Execute the next CPU instruction and obtain the number of cycles taken.
        cpu_step(&cpu);

        // Update the PPU with the cycles executed by the CPU.
        updatePPU(1);
        
        ctx.running = handle_SDL(&screen);
        render_screen(&screen, ppu);

        if (debug.is_on == false)
            poll_sdl_input(&io_regs.joypad);
        ctx.ticks++;
        


    }
    return 0;
}

int main(int argc, char **argv) {
    run(argc, argv);
    return 77;
}
