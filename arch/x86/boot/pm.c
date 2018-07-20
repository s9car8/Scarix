
#include "boot.h"

void disable_nmi(void) { /* ... */ }
void enable_a20(void) { /* ... */ }


void go_to_protected_mode(void)
{
#ifndef CONFIG_USE_MULTIBOOT
#   warning "go_to_protected_mode():: \'multiboot not definded\'"
    disable_interrupts();

    // Disable NMI as suggested in Intel Developers Manual.
    disable_nmi();

    enable_a20();
#endif

    // Whether Multiboot-compilant bootloader setup it's own GDT or not,
    // we gonna to fill and load our own GDT.
    setup_gdt();

    jump_to_protected_mode();
}
