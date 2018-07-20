
#if defined(__linux__)
#   error "You are not using a cross-compiler, you will most certainly run into trouble."
#endif


#include <stdint.h>

#include "boot.h"



static const char scancode2ascii[256] = {
    [0x02] = '1',
    [0x03] = '2',
    [0x04] = '3',
    [0x05] = '4',
    [0x06] = '5',
    [0x07] = '6',
    [0x08] = '7',
    [0x09] = '8',
    [0x0a] = '9',
    [0x0b] = '0',
    [0x0c] = '-',
    [0x0e] = '\b',
    [0x10] = 'q',
    [0x11] = 'w',
    [0x12] = 'e',
    [0x13] = 'r',
    [0x14] = 't',
    [0x15] = 'y',
    [0x16] = 'u',
    [0x17] = 'i',
    [0x18] = 'o',
    [0x19] = 'p',
    [0x1a] = '[',
    [0x1b] = ']',
    [0x1e] = 'a',
    [0x1f] = 's',
    [0x20] = 'd',
    [0x21] = 'f',
    [0x22] = 'g',
    [0x23] = 'h',
    [0x24] = 'j',
    [0x25] = 'k',
    [0x26] = 'l',
    [0x27] = ';',
    [0x28] = '\'',
    [0x29] = '`',
    // [0x2a] = '3',
    [0x2b] = '\\',
    [0x2c] = 'z',
    [0x2d] = 'x',
    [0x2e] = 'c',
    [0x2f] = 'v',
    [0x30] = 'b',
    [0x31] = 'n',
    [0x32] = 'm',
    [0x33] = ',',
    [0x34] = '.',
    [0x35] = '/',
};


static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}


static inline uint8_t inb(uint16_t port)
{
    uint8_t res;

    asm volatile("inb %1, %0" : "=a"(res) : "Nd"(port));
    return res;
}


char get_scancode()
{
    static char c = 0;

    while (1) {
        if (inb(0x60) != c) {
            c = inb(0x60);
            if (c > 0) return c;
        }
    }
}


char getchar()
{
    return scancode2ascii[get_scancode()];
}


void kernel_main(void)
{
    terminal_initialize();

    // while (1) {
    //     // terminal_printf("%i : %x\n", i++, get_scancode());
    //     terminal_printf("%c", getchar());
    // }

    // @Note: When we disable PE flag in CR0 register, we go to real mode.
    // To access data in this mode everything of them must be located BELOW 1M.
    // asm("mov %cr0, %eax; andl $~0x1, %eax; mov %eax, %cr0");

    go_to_protected_mode();
}


void kernel_main_pm()
{
    while (1) terminal_printf("%c", getchar());
}
