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

char *cart_parse_section(FILE *file, int *size);
cart_t *cart_parse(FILE *file);
void cart_free(cart_t *cart);

#endif
