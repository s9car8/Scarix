#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#include "boot.h"
#include "lib/io_impl.h"


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

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_col;
uint8_t terminal_color;
uint16_t* terminal_vga_ptr;


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

int terminal_putchar(int c)
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
        case '\b':
            if (terminal_col != 0) --terminal_col;
            break;
        default:
            terminal_putentryat(c, terminal_color, terminal_row, terminal_col);
            ++terminal_col;
            break;
    }
    return c;
}

int terminal_puts(const char* str)
{
    puts_impl(terminal_putchar, str);
}

int terminal_printf(const char* fmt, ...)
{
    va_list args;
    int res;

    va_start(args, fmt);
    res = printf_impl(terminal_putchar, fmt, args);
    va_end(args);
    return res;
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

void terminal_print_sregs()
{
    register uint32_t ebx asm("ebx");

    asm("movw %%cs, %0" : "=r" (ebx));
    terminal_printf("CS: 0x%x\n", ebx);

    asm("movw %%ds, %0" : "=r" (ebx));
    terminal_printf("DS: 0x%x\n", ebx);

    asm("movw %%es, %0" : "=r" (ebx));
    terminal_printf("ES: 0x%x\n", ebx);

    asm("movw %%fs, %0" : "=r" (ebx));
    terminal_printf("FS: 0x%x\n", ebx);

    asm("movw %%gs, %0" : "=r" (ebx));
    terminal_printf("GS: 0x%x\n", ebx);

    asm("movw %%ss, %0" : "=r" (ebx));
    terminal_printf("SS: 0x%x\n", ebx);
}

void terminal_print_cr()
{
    u32 cr0;
    // u32 cr1;
    // u32 cr2;
    // u32 cr3;

    asm volatile("movl %%cr0, %%eax; movl %%eax, %0" : "=r"(cr0));
    // asm volatile("movl %%cr1, %%eax; movl %%eax, %0" : "=r"(cr1));
    // asm volatile("movl %%cr2, %%eax; movl %%eax, %0" : "=r"(cr2));
    // asm volatile("movl %%cr3, %%eax; movl %%eax, %0" : "=r"(cr3));
    terminal_printf("CR0: 0x%x\n", cr0);
    // terminal_printf("CR1: 0x%x\n", cr1);
    // terminal_printf("CR2: 0x%x\n", cr2);
    // terminal_printf("CR3: 0x%x\n", cr3);
}
