
#include "boot.h"


void setup_gdt()
{
    static const u64 boot_gdt[]
        __attribute__((section(".initdata")))
        __attribute__((aligned(16))) = {

        [GDT_ENTRY_BOOT_CS]     = GDT_ENTRY(0xc09b, 0x0, 0xfffff),
        [GDT_ENTRY_BOOT_DS]     = GDT_ENTRY(0xc093, 0x0, 0xfffff),
        [GDT_ENTRY_BOOT_TSS]    = GDT_ENTRY(0x0089, 4096, 103),
    };
    struct gdt_ptr gdt = {.ptr = (u32) boot_gdt, .len = sizeof (boot_gdt) - 1};

    // Load GDT ptr.
    asm volatile("lgdtl %0" : : "m" (gdt));
}
