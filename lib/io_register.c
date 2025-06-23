#include <io_register.h>

static int map_sdl_key(SDL_Keycode sym) {
    switch (sym) {
        case SDLK_RIGHT:  return JOYP_RIGHT;
        case SDLK_LEFT:   return JOYP_LEFT;
        case SDLK_UP:     return JOYP_UP;
        case SDLK_DOWN:   return JOYP_DOWN;
        case SDLK_d:  return JOYP_RIGHT;
        case SDLK_q:   return JOYP_LEFT;
        case SDLK_z:     return JOYP_UP;
        case SDLK_s:   return JOYP_DOWN;
        case SDLK_j:      return JOYP_A;      // Z = A
        case SDLK_k:      return JOYP_B;      // X = B
        case SDLK_LSHIFT: return JOYP_SELECT; // RShift = Select
        case SDLK_LCTRL: return JOYP_START;  // Enter = Start
        default:          return 0;
    }
}

void poll_sdl_input(joypad_t *joypad) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            exit(0);
        }
        else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
            int bit = map_sdl_key(e.key.keysym.sym);
            if (!bit) continue;  // unmapped key

            if (e.type == SDL_KEYDOWN) {
                // set bit → record "pressed"
                joypad->hard |= bit;
            } else {
                // clear bit → record "released"
                joypad->hard &= ~bit;
            }
        }
    }
}

u8 joypad_read(joypad_t *pad)
{
    u8 res = pad->joyp | 0x0F;   // bits7-4 from joyp, bits3-0 = 1
    if (!(pad->joyp & 0x10))    // Directions
        res &= ~((pad->hard >> 0) & 0x0F);
    if (!(pad->joyp & 0x20))    // Buttons
        res &= ~((pad->hard >> 4) & 0x0F);
    return res;
}

void joypad_write(joypad_t *pad, u8 val) 
{
    pad->joyp = (val & 0x30) | 0xC0;  // store bit 4-5, force bits 7-6 = 1
}