
all:
	make --file boot/Makefile build
	mv build/krnl.img build/boot/krnl.img
	grub-file --is-x86-multiboot build/boot/krnl.img || echo the file is not multiboot
	cp grub.cfg build/boot/grub/grub.cfg
	grub-mkrescue -o cdrom.iso build
	qemu-system-i386 -cdrom cdrom.iso

.PHONY: clean
clean:
	make clean --file boot/Makefile
