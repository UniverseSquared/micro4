#ifndef _MICRO_H_
#define _MICRO_H_
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "lua.h"
#include "lauxlib.h"

#define SCREEN_SIZE  128
#define WINDOW_SCALE 2
#define WINDOW_SIZE  SCREEN_SIZE * WINDOW_SCALE

const static int micro_palette[][3] = {
    { 255, 0, 0 },
    { 0, 255, 0 },
    { 0, 0, 255 }
};

#define PALETTE_SIZE (sizeof(micro_palette) / sizeof(micro_palette[0]))

typedef struct micro {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *scene;
    lua_State *lua;
    int running;
} micro_t;

micro_t *micro_init(const char *cart);
void micro_free(micro_t *micro);
int micro_load_cart(micro_t *micro, const char *cart);
void micro_run(micro_t *micro);

#endif
