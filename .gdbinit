target remote localhost:1234
layout regs
symbol-file ./kernel_boot.elf
b jump_to_protected_mode
