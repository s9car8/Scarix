target remote localhost:1234
layout regs
symbol-file build/boot/krnl.img
b memcpy
