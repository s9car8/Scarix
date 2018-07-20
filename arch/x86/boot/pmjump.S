
#include "asm/boot.h"
#include "asm/gdt.h"

.text
#ifdef CONFIG_USE_MULTIBOOT
.code32
#else
.code16
#endif

.global jump_to_protected_mode
jump_to_protected_mode:
    movw    $BOOT_DS, %cx
    movw    $BOOT_TSS, %di

    // Enter to protected mode.
    movl    %cr0, %eax
    orl     $X86_CR0_PE, %eax
    movl    %eax, %cr0

    // Reload CS register.
    pushl   $BOOT_CS
    pushl   $in_pm32
    lretl

.code32
in_pm32:
    // Reload other segment registers.
    movl    %ecx, %ds
    movl    %ecx, %es
    movl    %ecx, %fs
    movl    %ecx, %gs
    movl    %ecx, %ss

    // ltr     %di

    // Jump to protected mode kernel.
    call    kernel_main_pm
    ret
