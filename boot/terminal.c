#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "boot.h"


enum vga_color
{
    VGA_COLOR_BLACK         = 0,
    VGA_COLOR_BLUE          = 1,
    VGA_COLOR_GREEN         = 2,
    VGA_COLOR_CYAN          = 3,
    VGA_COLOR_RED           = 4,
    VGA_COLOR_MAGENTA       = 5,
    VGA_COLOR_BROWN         = 6,
    VGA_COLOR_LIGHT_GREY    = 7,
    VGA_COLOR_DARK_GREY     = 8,
    VGA_COLOR_LIGHT_BLUE    = 9,
    VGA_COLOR_LIGHT_GREEN   = 10,
    VGA_COLOR_LIGHT_CYAN    = 11,
    VGA_COLOR_LIGHT_RED     = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN   = 14,
    VGA_COLOR_WHITE         = 15,
};


static inline
uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

static inline
uint16_t vga_entry(unsigned char uc, uint8_t color)
{
    return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str)
{
    size_t len = 0;

    while (*str++) ++len;
    return len;
}

void reverse(char* str, size_t n)
{
    for (size_t i = 0; i < n / 2; ++i)
        swap(str + i, str + n - 1 - i);
}

void itoa(char* buf, int x, int p)
{
    static const char digits[] =
        {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    size_t n = 0;

    while (1) {
        buf[n++] = digits[x % p];
        x /= p;
        if (x == 0) break;
    }
    reverse(buf, n);
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_col;
uint8_t terminal_color;
volatile uint16_t* terminal_vga_ptr;


void terminal_initialize(void)
{
    terminal_row = terminal_col = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_vga_ptr = (uint16_t*) 0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; ++y) {
        for (size_t x = 0; x < VGA_WIDTH; ++x) {
            __auto_type index = y * VGA_WIDTH + x;
            terminal_vga_ptr[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_shift_buffer(void)
{
    static char buf[10];
    size_t len = (VGA_HEIGHT - 1) * VGA_WIDTH;
    memcpy(terminal_vga_ptr, terminal_vga_ptr + VGA_WIDTH, sizeof (uint16_t[len]));
    for (size_t i = 0; i < VGA_WIDTH; ++i) terminal_vga_ptr[len + i] = vga_entry(' ', terminal_color);
}


void terminal_putentryat(char c, uint8_t color, size_t y, size_t x)
{
    __auto_type index = y * VGA_WIDTH + x;
    terminal_vga_ptr[index] = vga_entry(c, color);
}

void terminal_setcolor(uint8_t color)
{
    terminal_color = color;
}

void terminal_putchar(char c)
{
    if (terminal_col == VGA_WIDTH) {
        terminal_col = 0;
        ++terminal_row;
    }

    if (terminal_row >= VGA_HEIGHT) {
        terminal_row = VGA_HEIGHT - 1;
        terminal_shift_buffer();
    }

    switch (c)
    {
        case '\n':
            terminal_col = 0;
            ++terminal_row;
            break;
        default:
            terminal_putentryat(c, terminal_color, terminal_row, terminal_col);
            ++terminal_col;
            break;
    }
}

void terminal_puts(const char* str)
{
    for (; *str; ++str) terminal_putchar(*str);
}

void terminal_printf(const char* fmt, ...)
{
    static char buf[10];
    va_list args;

    va_start(args, fmt);
    for (; *fmt; ++fmt) {
        int ch = *fmt;

        switch (ch) {
            case '%':
                switch (*++fmt) {
                    case 'i': case 'd':
                        itoa(buf, va_arg(args, int), 10);
                        terminal_puts(buf);
                        break;
                    case 'x':
                        itoa(buf, va_arg(args, int), 16);
                        terminal_puts(buf);
                        break;
                }
                break;
            default: terminal_putchar(ch);
        }
    }
    va_end(args);
}

// __attribute__((force_align_arg_pointer))
void terminal_print_regs(void)
{
    asm("push %eax; push %ecx; push %edx");

    register const uint32_t eax asm("eax");
    register const uint32_t ebx asm("ebx");
    register const uint32_t ecx asm("ecx");
    register const uint32_t edx asm("edx");
    register const uint32_t esi asm("esi");
    register const uint32_t edi asm("edi");
    register const uint32_t ebp asm("ebp");
    register const uint32_t esp asm("esp");

    // asm("movl %%eax, %0" : "=r" (eax));
    // asm("movl %%ebx, %0" : "=r" (ebx));
    // asm("mov %%ecx, %0" : "=r" (ecx));
    // asm("mov %%edx, %0" : "=r" (edx));
    // asm("mov %%esi, %0" : "=r" (esi));
    // asm("mov %%edi, %0" : "=r" (edi));

    terminal_printf("EAX:   0x%x\n", eax);
    terminal_printf("EBX:   0x%x\n", ebx);
    terminal_printf("ECX:   0x%x\n", ecx);
    terminal_printf("EDX:   0x%x\n", edx);
    terminal_printf("ESI:   0x%x\n", esi);
    terminal_printf("EDI:   0x%x\n", edi);
    terminal_printf("EBP:   0x%x\n", ebp);
    terminal_printf("ESP:   0x%x\n", esp);

    asm("pop %edx; pop %ecx; pop %eax");
}
