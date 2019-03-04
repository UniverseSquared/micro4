#include "micro.h"

int main(int argc, char *argv[]) {
    if(argc < 2) {
        fprintf(stderr, "Usage: %s <cart>\n", argv[0]);
        return 1;
    }

    /* Initialize the micro4. */
    micro_t *micro = micro_init(argv[1]);
    if(!micro) {
        return 1;
    }

    /* Run the loaded cart. */
    micro_run(micro);

    /* Cleanup. */
    micro_free(micro);
    return 0;
}
