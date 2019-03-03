#include "api.h"

void micro_load_api(micro_t *micro) {
    luaL_openlibs(micro->lua);
}
