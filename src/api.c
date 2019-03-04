#include "api.h"

micro_t *micro_get_state(lua_State *lua) {
    lua_getglobal(lua, "_MICRO4");
    return (micro_t*)lua_touserdata(lua, -1);
}

void micro_get_color(lua_State *lua, int color, int *r, int *g, int *b) {
    if(color >= PALETTE_SIZE) {
        lua_pushstring(lua, "Invalid palette index.");
        lua_error(lua);
    }

    *r = micro_palette[color][0];
    *g = micro_palette[color][1];
    *b = micro_palette[color][2];
}

int micro_clr(lua_State *lua) {
    micro_t *micro = micro_get_state(lua);

    int r, g, b, color = luaL_checknumber(lua, 1);
    micro_get_color(lua, color, &r, &g, &b);

    SDL_SetRenderDrawColor(micro->renderer, r, g, b, 255);
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

    int r, g, b, color = luaL_checknumber(lua, 5);
    micro_get_color(lua, color, &r, &g, &b);

    SDL_SetRenderDrawColor(micro->renderer, r, g, b, 255);
    SDL_RenderFillRect(micro->renderer, &rect);

    return 0;
}

int micro_rectline(lua_State *lua) {
    micro_t *micro = micro_get_state(lua);

    int x = luaL_checknumber(lua, 1);
    int y = luaL_checknumber(lua, 2);
    int w = luaL_checknumber(lua, 3);
    int h = luaL_checknumber(lua, 4);
    SDL_Rect rect = { .x = x, .y = y, .w = w, .h = h };

    int r, g, b, color = luaL_checknumber(lua, 5);
    micro_get_color(lua, color, &r, &g, &b);

    SDL_SetRenderDrawColor(micro->renderer, r, g, b, 255);
    SDL_RenderDrawRect(micro->renderer, &rect);

    return 0;
}

int micro_circle(lua_State *lua) {
    micro_t *micro = micro_get_state(lua);

    int x = luaL_checknumber(lua, 1);
    int y = luaL_checknumber(lua, 2);
    int radius = luaL_checknumber(lua, 3);

    int r, g, b, color = luaL_checknumber(lua, 4);
    micro_get_color(lua, color, &r, &g, &b);

    filledCircleRGBA(micro->renderer, x, y, radius, r, g, b, 255);

    return 0;
}

int micro_circleline(lua_State *lua) {
    micro_t *micro = micro_get_state(lua);

    int x = luaL_checknumber(lua, 1);
    int y = luaL_checknumber(lua, 2);
    int radius = luaL_checknumber(lua, 3);

    int r, g, b, color = luaL_checknumber(lua, 4);
    micro_get_color(lua, color, &r, &g, &b);

    circleRGBA(micro->renderer, x, y, radius, r, g, b, 255);

    return 0;
}

int micro_btn(lua_State *lua) {
    micro_t *micro = micro_get_state(lua);

    int button = luaL_checknumber(lua, 1);
    lua_pushboolean(lua, micro->button_state[button]);

    return 1;
}

int micro_btnp(lua_State *lua) {
    micro_t *micro = micro_get_state(lua);

    int button = luaL_checknumber(lua, 1);
    lua_pushboolean(lua, micro->button_state[button] && !micro->old_button_state[button]);

    return 1;
}

void micro_load_api(micro_t *micro) {
    luaL_openlibs(micro->lua);

    /* Load the functions into the lua environment. */
    lua_pushcfunction(micro->lua, micro_clr);
    lua_setglobal(micro->lua, "clr");

    lua_pushcfunction(micro->lua, micro_rect);
    lua_setglobal(micro->lua, "rect");

    lua_pushcfunction(micro->lua, micro_rectline);
    lua_setglobal(micro->lua, "rectline");

    lua_pushcfunction(micro->lua, micro_circle);
    lua_setglobal(micro->lua, "circle");

    lua_pushcfunction(micro->lua, micro_circleline);
    lua_setglobal(micro->lua, "circleline");
    
    lua_pushcfunction(micro->lua, micro_btn);
    lua_setglobal(micro->lua, "btn");

    lua_pushcfunction(micro->lua, micro_btnp);
    lua_setglobal(micro->lua, "btnp");
}
