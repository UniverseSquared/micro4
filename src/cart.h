#ifndef _CART_H_
#define _CART_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SECTION_COUNT 2

typedef struct cart {
    int code_size;
    char *code_data;
} cart_t;

cart_t *cart_parse(FILE *file);
char *cart_get_section(cart_t *cart, const char *section);
void cart_free(cart_t *cart);

#endif