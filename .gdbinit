target remote localhost:1234
layout regs
symbol-file iso/boot/kernel_boot.bin
b _start
