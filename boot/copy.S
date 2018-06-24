
// .code16
.section .text

#define GLOBAL(name) \
.global name; \
.type name, @function; \
name:


GLOBAL(memcpy)
    pushl   %esi
    pushl   %edi
    // call    terminal_print_regs
    // pushl   20(%esp)
    // call    terminal_puts
    // add     $0x4, %esp
    // jmp     .
    movl    12(%esp), %edi
    movl    16(%esp), %esi
    movw    20(%esp), %cx
    // call    terminal_print_regs
    // jmp     .
    pushw   %cx
    shrw    $2, %cx
    rep     movsl
    popw    %cx
    andw    $3, %cx
    rep     movsb
    popl    %edi
    popl    %esi
    retl

GLOBAL(memset)
    pushl   %edi
    movw    8(%esp), %edi
    movzbl  12(%esp), %eax
    movw    16(%esp), %cx
    imull   $0x01010101, %eax
    pushw   %cx
    shrw    $2, %cx
    rep     stosl
    popw    %cx
    andw    $3, %cx
    rep     stosb
    popl    %edi
    retl
