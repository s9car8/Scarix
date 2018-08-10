
#include "asm/boot.h"


.text
.code32

.global start32
start32:
    call    1f
1:  popl    %ebx
    subl    $1b, %ebx
    jmp     go_to_long_mode

go_to_long_mode:
    // Step 1: Disable paging.
    movl    %cr0, %eax
    andl    $~X86_CR0_PG, %eax
    movl    %eax, %cr0

    // Step 2: Enable physical-address extension.
    movl    $(X86_CR4_PAE), %eax
    movl    %eax, %cr4

    // Step 3: Load PML4
    call    init_pgtable
    leal    pgtable(%ebx), %eax
    movl    %eax, %cr3

    // Step 4: Enable IA-32e.
    movl    $MSR_EFER, %ecx
    rdmsr
    btsl    $_EFER_LME, %eax
    wrmsr

    // Step 5: Enable paging.
    movl    %cr0, %eax
    orl     $(X86_CR0_PG | X86_CR0_PE), %eax
    movl    %eax, %cr0

    call    kernel_main_lm

init_pgtable:
    cld

    # Fill page table by zeros.
    leal    pgtable(%ebx), %edi
    xorl    %eax, %eax
    movl    $6144, %ecx
    rep     stosl

    # Setup PML4.
    leal    pgtable +0x1000(%ebx), %eax
    orl     $0x7, %eax
    movl    %eax, pgtable(%ebx)

    # Setup PDPT.
    leal    pgtable + 0x1000(%ebx), %edi
    leal    pgtable + 0x2000(%ebx), %eax
    orl     $0x7, %eax
    movl    $4, %ecx
1:  movl    %eax, (%edi)
    addl    $0x8, %edi
    addl    $0x00001000, %eax
    decl    %ecx
    jnz     1b

    # Setup PD.
    leal    pgtable + 0x2000(%ebx), %edi
    movl    $0x00000183, %eax // G + USE_2M_PAGES + R/W + PRESENT
    movl    $0x800, %ecx
1:  movl    %eax, (%edi)
    addl    $0x8, %edi
    addl    $0x00200000, %eax
    decl    %ecx
    jnz     1b

    retl


.section .data
/*
 * +----------------------------------+
 * | Table | Entries | Size | Aligned |
 * +----------------------------------+
 * | PML4  | 1       | 8    | 4096    |
 * | PDPT  | 4/256   | 2048 | 4096    |
 * | PD    | 512     | 4096 | 4096    |
 * +----------------------------------+
 * Note: Instead of building PTEs, we're going to use 2M page entries in PD.
 * Thus my OS support up to 16G addressing.
 * Initial paging structure size: 4096 + 4096 + 4*4096 = 24576
 */
.align 0x1000
pgtable: .skip 24576
