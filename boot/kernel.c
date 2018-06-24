
#if defined(__linux__)
#   error "You are not using a cross-compiler, you will most certainly run into trouble."
#endif


#include <stdint.h>
#include "boot.h"


#define MAKE_GDT_ENTRY() \
    ;


void kernel_main()
{
    char buf[50];

    terminal_initialize();

    // asm("xor %ax, %ax");
    memcpy(buf, "Hello, kernel world! #%i\n", 26);
    terminal_puts("================================================================================");
    // const char* xxx = "XXX";
    // asm("pushl $3; pushl %0; pushl $1; call memcpy" :: "r" (xxx), "r" (buf));
    for (int i = 0; i < 30; ++i)
        terminal_printf(buf, i);
}
