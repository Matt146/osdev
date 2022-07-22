#ifndef MEM_H
#define MEM_H

#include <stdint.h>
#include <stddef.h>

static void* memset(void *s, int c, size_t len) {
    unsigned char *dst = (unsigned char*)s;
    while (len > 0) {
        *dst = (unsigned char) c;
        dst++;
        len--;
    }
    return s;
}

#endif // MEM_H