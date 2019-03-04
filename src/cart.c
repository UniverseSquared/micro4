#include "cart.h"

char *cart_parse_section(FILE *file, int *size) {
    fseek(file, 1, SEEK_CUR);

    char length_bytes[2];
    fread(length_bytes, 1, 2, file);

    int length = (length_bytes[0] << 4) | length_bytes[1];
    char *buffer = (char*)malloc(length);

    fread(buffer, 1, length, file);

    if(size)
        *size = length;

    return buffer;
}

cart_t *cart_parse(FILE *file) {
    cart_t *cart = (cart_t*)malloc(sizeof(cart_t));

    char header[4];
    fread(header, 1, 4, file);
    if(strcmp(header, "MC4C") != 0) {
        fprintf(stderr, "Input file is not a valid cart: no header.\n");
        return NULL;
    }

    cart->code_data = cart_parse_section(file, &cart->code_size);

    return cart;
}

void cart_free(cart_t *cart) {
    free(cart->code_data);
    free(cart);
}
