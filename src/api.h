#ifndef _API_H_
#define _API_H_
#include "micro.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include <SDL2/SDL2_gfxPrimitives.h>

micro_t *micro_get_state(lua_State *lua);
void micro_get_color(lua_State *lua, int color, int *r, int *g, int *b);

void micro_load_api(micro_t *micro);

#endif
