#include "cart.h"

cart_t *cart_parse(FILE *file) {
    cart_t *cart = (cart_t*)malloc(sizeof(cart_t));

    char header[4];
    fread(header, 1, 4, file);
    if(strcmp(header, "MC4C") != 0) {
        fprintf(stderr, "Input file is not a valid cart: no header.\n");
        return NULL;
    }

    /* TODO: Move section parser to it's own function. (cart_parse_section) */
    fseek(file, 1, SEEK_CUR);

    char code_length_bytes[2];
    fread(code_length_bytes, 1, 2, file);

    int code_length = (code_length_bytes[0] << 4) | code_length_bytes[1];
    printf("code is %0x bytes long\n", code_length);

    cart->code_size = code_length;
    cart->code_data = (char*)malloc(code_length);
    fread(cart->code_data, 1, code_length, file);

    printf("%s\n", cart->code_data);

    return cart;
}

char *cart_get_section(cart_t *cart, const char *section) {
}

void cart_free(cart_t *cart) {
    free(cart->code_data);
    free(cart);
}
