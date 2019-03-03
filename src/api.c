#include "api.h"

micro_t *micro_get_state(lua_State *lua) {
    lua_getglobal(lua, "_MICRO4");
    return (micro_t*)lua_touserdata(lua, -1);
}

int micro_clr(lua_State *lua) {
    micro_t *micro = micro_get_state(lua);

    SDL_SetRenderDrawColor(micro->renderer, 0, 0, 0, 255);
    SDL_RenderClear(micro->renderer);

    return 0;
}

int micro_rect(lua_State *lua) {
    micro_t *micro = micro_get_state(lua);

    int x = luaL_checknumber(lua, 1);
    int y = luaL_checknumber(lua, 2);
    int w = luaL_checknumber(lua, 3);
    int h = luaL_checknumber(lua, 4);
    SDL_Rect rect = { .x = x, .y = y, .w = w, .h = h };

    int color = luaL_checknumber(lua, 5);
    if(color >= PALETTE_SIZE) {
        lua_pushstring(lua, "Invalid palette index.");
        lua_error(lua);
    }

    int r = micro_palette[color][0];
    int g = micro_palette[color][1];
    int b = micro_palette[color][2];

    SDL_SetRenderDrawColor(micro->renderer, r, g, b, 255);
    SDL_RenderFillRect(micro->renderer, &rect);

    return 0;
}

void micro_load_api(micro_t *micro) {
    luaL_openlibs(micro->lua);

    lua_pushcfunction(micro->lua, micro_clr);
    lua_setglobal(micro->lua, "clr");

    lua_pushcfunction(micro->lua, micro_rect);
    lua_setglobal(micro->lua, "rect");
}
