CC=~/opt/cross-gcc/bin/i686-elf-gcc
BUILD_DIR=build
PREFIX=boot

build: boot.o kernel.o terminal.o $(PREFIX)/kernel.ld
	$(CC) -T $(PREFIX)/kernel.ld -ffreestanding -O2 -nostdlib -o $(BUILD_DIR)/krnl.img $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o $(BUILD_DIR)/terminal.o -lgcc

boot.o: $(PREFIX)/boot.S
	~/opt/cross-gcc/bin/i686-elf-as -o $(BUILD_DIR)/$@ $^

kernel.o: $(PREFIX)/kernel.c
	$(CC) -std=gnu99 -ffreestanding -o $(BUILD_DIR)/$@ -c $^

terminal.o: $(PREFIX)/terminal.c
	$(CC) -std=gnu99 -ffreestanding -o $(BUILD_DIR)/$@ -c $^

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)/*.o
