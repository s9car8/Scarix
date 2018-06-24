CC=~/opt/cross-gcc/bin/i686-elf-gcc
BUILD_DIR=build
PREFIX=boot

build: $(PREFIX)/kernel.ld boot.o kernel.o terminal.o copy.o
	$(CC) -g -T $(PREFIX)/kernel.ld -ffreestanding -O2 -nostdlib -o $(BUILD_DIR)/krnl.img $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o $(BUILD_DIR)/terminal.o $(BUILD_DIR)/copy.o -lgcc

boot.o: $(PREFIX)/boot.S
	$(CC) -o $(BUILD_DIR)/$@ -c -g $^

kernel.o: $(PREFIX)/kernel.c
	$(CC) -std=gnu99 -ffreestanding -o $(BUILD_DIR)/$@ -c -g $^

terminal.o: $(PREFIX)/terminal.c
	$(CC) -std=gnu99 -ffreestanding -o $(BUILD_DIR)/$@ -c -g $^

copy.o: $(PREFIX)/copy.S
	$(CC) -o $(BUILD_DIR)/$@ -c -g $^

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)/*.o
