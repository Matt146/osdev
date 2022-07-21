#ifndef E9_H
#define E9_H

#include <x86.h>

static inline void e9_write(const char* str) {
    while (*str != 0x0) {
        outb(0xe9, *str);
        str++;
    }
}

#endif // E9_H