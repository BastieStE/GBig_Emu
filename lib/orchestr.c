#include <orchestr.h>


static void init_debug(int argc, char **argv, debug_ctx *ctx)
{
    if (argc < 2) { ctx->is_on = false; return;}

    if (argv[1][0] == '-' && argv[1][1] == 'd') {
        printf("debug mode activated\n");
        ctx->is_on = true;
    }
    if (argc > 3)
        if(ctx->speed = atoi(argv[2]) == 0)
            ctx->speed = 1;
    ctx->step = 0;
}

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
    
}


static int debug_step(debug_ctx *debug, IO_register_t *io_regs)
{
    if (debug->is_on == true) {
        puts("debug :");
        debug->step++;
        if (debug->step >= debug->speed) {
            
            char cmd[5];      // pause and get command
            char c;
            for (int i = 0; (c = getchar()) != '\n' && c != EOF && i < sizeof(cmd) - 1; i++) {
                cmd[i] = c;
            }
            poll_sdl_input(io_regs->joypad);
            
            if (cmd[0] == 's')  // exit debug
                debug->is_on = false;
            else if (cmd[0] == '+')
                debug->speed++; // incresse speed
            else if (cmd[0] == '-')
                debug->speed--; // decresse speed
            else if (cmd[0] == 'q')
                return 1; // leave loop
            debug->step = 0;

            print_cpu_registers();
            printf("the IO registers : \n\tjoypad hard: %02X\n\tjoypad joyp: %02X\n", io_regs->joypad.hard, io_regs->joypad.joyp);
        }
    }
    return 0;
}

static int ctx_init(main_context *ctx)
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
}

int run(int argc, char **argv) {
    main_context ctx;
    cpu_context ;
    ppu_context ppu;  // Create an instance of your PPU
    debug_ctx debug;
    IO_register_t io_regs;
    visual_t screen;

    // Initialize the main context, CPU, WRAM, and PPU.
    cart_load("../Tetris.gb"); // load cart
    init_debug(argc, argv, &debug);
    ctx_init(&ctx); // init orchestrator context
    cpu_init(); // init cpu
    bus_init(&io_regs); // init bus and bios
    ppu_init(&ppu); // init PPU state
    screen = init_SDL(screen);
    puts("end of init");

    while(ctx.running) {
        printf("%d ticks\n", ctx.ticks);
        if (ctx.paused) {
            puts("pause not implemented");
        }

        // Execute the next CPU instruction and obtain the number of cycles taken.
        int cycles = cpu_step();
        puts("step");

        if (cycles < 0) {
            puts("CPU Stopped");
            return -3;
        }

        // Update the PPU with the cycles executed by the CPU.
        updatePPU(&ppu, cycles);
        ctx.running = handle_SDL(screen);

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
