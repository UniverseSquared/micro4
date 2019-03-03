#include "micro.h"

micro_t *micro_init(const char *cart) {
    micro_t *micro = (micro_t*)malloc(sizeof(micro_t));

    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        return NULL;
    }

    micro->window = SDL_CreateWindow("micro4",
                                     SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                     WINDOW_SIZE, WINDOW_SIZE, SDL_WINDOW_SHOWN);
    if(!micro->window) {
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        SDL_Quit();
        return NULL;
    }

    micro->renderer = SDL_CreateRenderer(micro->window, -1, 0);
    if(!micro->renderer) {
        fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(micro->window);
        SDL_Quit();
        return NULL;
    }

    micro->lua = luaL_newstate();

    if(micro_load_cart(micro, cart) != 0) {
        return NULL;
    }

    micro->running = 1;

    return micro;
}

void micro_free(micro_t *micro) {
    SDL_DestroyRenderer(micro->renderer);
    SDL_DestroyWindow(micro->window);
    SDL_Quit();
    lua_close(micro->lua);
    free(micro);
}

int micro_load_cart(micro_t *micro, const char *cart) {
    if(luaL_loadfile(micro->lua, cart) != 0) {
        fprintf(stderr, "Failed to load cart!\n");
        return 1;
    }

    int ret;
    if((ret = lua_pcall(micro->lua, 0, 0, 0)) != 0) {
        if(ret == LUA_ERRRUN) {
            fprintf(stderr, "Failed to load cart:\n%s\n",
                    lua_tostring(micro->lua, -1));
            return 1;
        }
    }

    lua_getglobal(micro->lua, "init");
    if(lua_isfunction(micro->lua, -1)) {
        if((ret = lua_pcall(micro->lua, 0, 0, 0)) != 0) {
            if(ret == LUA_ERRRUN) {
                fprintf(stderr, "%s\n", lua_tostring(micro->lua, -1));
                return 1;
            }
        }
    }

    return 0;
}

void micro_run(micro_t *micro) {
    while(micro->running) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            switch(e.type) {
            case SDL_QUIT:
                micro->running = 0;
                break;
            }
        }

        SDL_SetRenderDrawColor(micro->renderer, 0, 0, 0, 255);
        SDL_RenderClear(micro->renderer);

        SDL_RenderPresent(micro->renderer);
    }
}
