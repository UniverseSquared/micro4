#include "micro.h"
#include "api.h"

int micro_resolve_key(SDL_Keycode key) {
    for(int i = 0; i < BUTTON_COUNT; i++)
        if(micro_button_map[i] == key)
            return i;

    return -1;
}

micro_t *micro_init(const char *cart_path) {
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

    micro->scene = SDL_CreateTexture(micro->renderer, SDL_PIXELFORMAT_RGBA8888,
                                     SDL_TEXTUREACCESS_TARGET, SCREEN_SIZE, SCREEN_SIZE);
    if(!micro->scene) {
        fprintf(stderr, "Failed to create scene texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(micro->renderer);
        SDL_DestroyWindow(micro->window);
        SDL_Quit();
        return NULL;
    }

    micro->lua = luaL_newstate();

    /* Load micro4's lua functions into the environment. */
    micro_load_api(micro);

    /* Load the cart from the path provided. */
    FILE *cart_file = fopen(cart_path, "r");
    if(micro_load_cart(micro, cart_file) != 0) {
        return NULL;
    }

    fclose(cart_file);

    /* Initialize button states to 0. */
    for(int i = 0; i < BUTTON_COUNT; i++) {
        micro->button_state[i] = 0;
        micro->old_button_state[i] = 0;
    }

    micro->running = 1;

    micro->start_time = 0;
    micro->end_time = 0;
    micro->delta = 0;
    micro->fps = 60;

    /* Put the address of the micro struct into a variable.
     * This is so that it can be accessed from the lua API functions. */
    lua_pushlightuserdata(micro->lua, micro);
    lua_setglobal(micro->lua, "_MICRO4");

    return micro;
}

void micro_free(micro_t *micro) {
    SDL_DestroyRenderer(micro->renderer);
    SDL_DestroyWindow(micro->window);
    SDL_Quit();
    lua_close(micro->lua);
    free(micro);
}

int micro_load_cart(micro_t *micro, FILE *cart_file) {
    micro->cart = cart_parse(cart_file);

    /* Load the cart's code into a function. */
    int ret;
    if((ret = luaL_loadstring(micro->lua, micro->cart->code_data)) != 0) {
        if(ret == LUA_ERRSYNTAX) {
            fprintf(stderr, "Failed to load cart:\n%s\n",
                    lua_tostring(micro->lua, -1));
            return 1;
        }

    }

    /* Call the loaded function. */
    if((ret = lua_pcall(micro->lua, 0, 0, 0)) != 0) {
        if(ret == LUA_ERRRUN) {
            fprintf(stderr, "Failed to load cart:\n%s\n",
                    lua_tostring(micro->lua, -1));
            return 1;
        }
    }

    /* Invoke the init function, if present. */
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
    micro->start_time = SDL_GetTicks();
    int ret;
    while(micro->running) {
        micro->delta = micro->end_time - micro->start_time;

        if(micro->delta < FRAME_TIME) {
            SDL_Delay(FRAME_TIME - micro->delta);
        }

        if(micro->delta > FRAME_TIME) {
            micro->fps = 1000 / micro->delta;
        }

        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            switch(e.type) {
            case SDL_QUIT:
                micro->running = 0;
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                int button = micro_resolve_key(e.key.keysym.sym);
                if(button != -1) {
                    micro->button_state[button] = e.type == SDL_KEYDOWN;
                }
                break;
            }
            }
        }

        /* Invoke the update function, if present. */
        lua_getglobal(micro->lua, "update");
        if(lua_isfunction(micro->lua, -1)) {
            if((ret = lua_pcall(micro->lua, 0, 0, 0)) != 0) {
                if(ret == LUA_ERRRUN) {
                    fprintf(stderr, "%s\n", lua_tostring(micro->lua, -1));
                    micro->running = 0;
                }
            }
        }

        SDL_SetRenderTarget(micro->renderer, micro->scene);

        /* Invoke the draw function, if present. */
        lua_getglobal(micro->lua, "draw");
        if(lua_isfunction(micro->lua, -1)) {
            if((ret = lua_pcall(micro->lua, 0, 0, 0)) != 0) {
                if(ret == LUA_ERRRUN) {
                    fprintf(stderr, "%s\n", lua_tostring(micro->lua, -1));
                    micro->running = 0;
                }
            }
        }

        SDL_SetRenderTarget(micro->renderer, NULL);
        SDL_RenderCopy(micro->renderer, micro->scene, NULL, NULL);

        SDL_RenderPresent(micro->renderer);

        /* Update the old button state. */
        for(int i = 0; i < BUTTON_COUNT; i++)
            micro->old_button_state[i] = micro->button_state[i];

        micro->start_time = micro->end_time;
        micro->end_time = SDL_GetTicks();
    }
}
