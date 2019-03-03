#ifndef _MICRO_H_
#define _MICRO_H_
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "lua.h"
#include "lauxlib.h"

#define SCREEN_SIZE  128
#define WINDOW_SCALE 4
#define WINDOW_SIZE  SCREEN_SIZE * WINDOW_SCALE

#define TARGET_FPS 60
#define FRAME_TIME (1000 / TARGET_FPS)

const static int micro_palette[][3] = {
    { 255, 0, 0 },
    { 0, 255, 0 },
    { 0, 0, 255 }
};

#define PALETTE_SIZE (sizeof(micro_palette) / sizeof(micro_palette[0]))

const static int micro_button_map[] = {
    [0] = SDLK_z,
    [1] = SDLK_x,
    [2] = SDLK_UP,
    [3] = SDLK_DOWN,
    [4] = SDLK_LEFT,
    [5] = SDLK_RIGHT
};

#define BUTTON_COUNT (sizeof(micro_button_map) / sizeof(micro_button_map[0]))

typedef struct micro {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *scene;
    lua_State *lua;
    int button_state[BUTTON_COUNT];
    int old_button_state[BUTTON_COUNT];
    int running;
    int start_time, end_time, delta, fps;
} micro_t;

int micro_resolve_key(SDL_Keycode key);

micro_t *micro_init(const char *cart);
void micro_free(micro_t *micro);
int micro_load_cart(micro_t *micro, const char *cart);
void micro_run(micro_t *micro);

#endif
