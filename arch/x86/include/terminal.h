
// VGA terminal interface.
void terminal_initialize(void);
int terminal_putchar(int);
int terminal_puts(const char* str);
int terminal_printf(const char* fmt, ...);
void terminal_print_regs(void);
