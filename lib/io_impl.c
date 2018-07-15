
#include <stddef.h>
#include <stdarg.h>

#include "global.h"


void reverse(char* str, size_t n)
{
    for (size_t i = 0; i < n / 2; ++i)
        swap(str + i, str + n - 1 - i);
}


void itoa(char* buf, unsigned x, int p)
{
    static const char digits[] =
        {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    size_t n = 0;

    while (1) {
        buf[n++] = digits[x % p];
        x /= p;
        if (x == 0) break;
    }
    buf[n] = '\0';
    reverse(buf, n);
}


int puts_impl(int (*putchar)(int), const char* str)
{
    for (; *str; ++str)
        putchar(*str);
}


int printf_impl(int (*putchar)(int), const char* fmt, va_list args)
{
    static char buf[10];
    int n = 0;

    for (; *fmt; ++fmt) {
        switch (*fmt) {
            case '%':
                ++n;
                switch (*++fmt) {
                    case 'i': case 'd':
                        itoa(buf, va_arg(args, int), 10);
                        puts_impl(putchar, buf);
                        break;
                    case 'x':
                        itoa(buf, va_arg(args, unsigned), 16);
                        puts_impl(putchar, "0x");
                        puts_impl(putchar, buf);
                        break;
                    case 'c':
                        putchar(va_arg(args, int));
                        break;
                    case 's':
                        puts_impl(putchar, va_arg(args, char*));
                        break;
                    default: --n; // This is only one case for undoing of the n incrementing.
                }
                break;
            default: putchar(*fmt);
        }
    }
    return n;
}
