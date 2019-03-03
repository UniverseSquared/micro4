#ifndef _MICRO_H_
#define _MICRO_H_
#include <stdlib.h>
#include <SDL2/SDL.h>

#define SCREEN_SIZE  128
#define WINDOW_SCALE 2
#define WINDOW_SIZE  SCREEN_SIZE * WINDOW_SCALE

typedef struct micro {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int running;
} micro_t;

micro_t *micro_init();
void micro_free(micro_t *micro);
void micro_run(micro_t *micro);

#endif
