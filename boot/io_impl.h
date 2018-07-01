#include <stdarg.h>


int puts_impl(int (*putchar)(int), const char* str);
int printf_impl(int (*putchar)(int), const char* fmt, va_list args);
