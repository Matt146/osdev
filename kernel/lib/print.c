#include <stdarg.h>
#include <stdint.h>
#include <serial.h>
#include <stddef.h>
#include "print.h"

void printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt != 0x0) {
        switch (*fmt) {
            case 'd':
                uint64_t x = va_args(args, uint64_t);
                serial_write(itoa(x, 10));
                break;
            default:
                serial_write(fmt);
                break;
        }
        fmt++;
    }
}