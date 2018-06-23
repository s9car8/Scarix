
#if defined(__linux__)
#   error "You are not using a cross-compiler, you will most certainly run into trouble."
#endif


#include <stdint.h>


#define MAKE_GDT_ENTRY() \
    ;


extern uint8_t terminal_color;

void kernel_main()
{
    terminal_initialize();
    terminal_putentryat('!', terminal_color, 10, 10);
}
