#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stddef.h>

static char characters[] = "0123456789ABCDEF";

const char* itoa(uint64_t x, uint64_t base) {
    static char buff[500];
    size_t idx = 0;
    do {
        buff[idx] = characters[(x % base)];
        x /= base;
        idx++;
    } while (x > 0);
    buff[idx] = 0x0;

    for (size_t i = 0; i < idx/2; i++) {
        char tmp = buff[i];
        buff[i] = buff[idx-i-1];
        buff[idx-i-1] = tmp;
    }

    return buff;
}

#endif //STRING_H