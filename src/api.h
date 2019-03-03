#ifndef _API_H_
#define _API_H_
#include "micro.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

micro_t *micro_get_state(lua_State *lua);

int micro_clr(lua_State *lua);
int micro_btn(lua_State *lua);

void micro_load_api(micro_t *micro);

#endif
