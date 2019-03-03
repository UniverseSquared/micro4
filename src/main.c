#include "micro.h"

int main(int argc, char *argv[]) {
    micro_t *micro = micro_init();
    if(!micro) {
        return 1;
    }

    micro_run(micro);

    micro_free(micro);
    return 0;
}
