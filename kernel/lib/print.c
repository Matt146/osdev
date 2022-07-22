#include <stdarg.h>
#include <stdint.h>
#include <serial.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "print.h"

void printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    const char* start = fmt;
    while (*fmt != 0x0) {
        if (fmt > start && *(fmt-1) == '%') {
            if (*fmt == 'd') {
                uint64_t num = va_arg(args, uint64_t);
                serial_write(uint_to_str(num, 10));
            }
        } else {
            if (*fmt != '%') {
                char c[2] = {*fmt, '\0'};
                serial_write(&c);
            }
        }

        fmt++;
    }
    va_end(args);
}