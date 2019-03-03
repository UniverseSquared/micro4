#include "micro.h"

int main(int argc, char *argv[]) {
    if(argc < 2) {
        fprintf(stderr, "Usage: %s <cart>\n", argv[0]);
        return 1;
    }

    micro_t *micro = micro_init(argv[1]);
    if(!micro) {
        return 1;
    }

    micro_run(micro);

    micro_free(micro);
    return 0;
}
