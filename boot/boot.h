
#include <stddef.h>


#define swap(a, b)              \
    ({                          \
        __auto_type tmp = *(a); \
        *(a) = *(b);            \
        *(b) = tmp;             \
    })


// Copy utils.
extern void memcpy(void* dest, void* src, size_t n);
extern void memset(void* dest, char c, size_t n);

// VGA termianl interface.
void terminal_initialize(void);
int terminal_putchar(int);
int terminal_puts(const char* str);
int terminal_printf(const char* fmt, ...);
void terminal_print_regs(void);

#include "io_impl.h"
