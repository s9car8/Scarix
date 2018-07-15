CFLAGS := -g -std=gnu99
CFLAGS += -ffreestanding
CFLAGS += $(KERNELINCLUDE)

LDFLAGS	:= -shared -ffreestanding -nostdlib

obj		:= $(OBJPATH)/boot

boot-objs := boot.o kernel.o terminal.o memory.o io_impl.o
boot-objs := $(addprefix $(obj)/, $(boot-objs))

include $(SRCPATH)/scripts/Kbuild.include


OBJCOPYFLAGS_kernel_boot.bin := #-O binary
$(OBJPATH)/kernel_boot.bin: $(OBJPATH)/kernel_boot.elf
	$(call cmd,objcopy)
	# cp $< $@

LDFLAGS_kernel_boot.elf := -T
$(OBJPATH)/kernel_boot.elf: $(SRCPATH)/arch/x86/boot/setup.ld $(boot-objs)
	$(call cmd,ld)

$(obj)/boot.o: $(SRCPATH)/arch/x86/boot/boot.S
	$(call cmd,cc)

$(obj)/kernel.o: $(SRCPATH)/arch/x86/boot/kernel.c
	$(call cmd,cc)

$(obj)/terminal.o: $(SRCPATH)/arch/x86/boot/terminal.c
	$(call cmd,cc)

$(obj)/memory.o: $(SRCPATH)/arch/x86/lib/memory.S
	$(call cmd,cc)

$(obj)/io_impl.o: $(SRCPATH)/lib/io_impl.c
	$(call cmd,cc)





















# CC=~/opt/cross-gcc/bin/i686-elf-gcc
# BUILD_DIR=build
# PREFIX=boot

# build: $(PREFIX)/kernel.ld boot.o kernel.o terminal.o io_impl.o copy.o
# 	$(CC) -g -T $(PREFIX)/kernel.ld -ffreestanding -O2 -nostdlib -o $(BUILD_DIR)/krnl.img $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o $(BUILD_DIR)/terminal.o $(BUILD_DIR)/io_impl.o $(BUILD_DIR)/copy.o -lgcc

# boot.o: $(PREFIX)/boot.S
# 	$(CC) -o $(BUILD_DIR)/$@ -c -g $^

# kernel.o: $(PREFIX)/kernel.c
# 	$(CC) -std=gnu99 -ffreestanding -o $(BUILD_DIR)/$@ -c -g $^

# terminal.o: $(PREFIX)/terminal.c
# 	$(CC) -std=gnu99 -ffreestanding -o $(BUILD_DIR)/$@ -c -g $^

# io_impl.o: $(PREFIX)/io_impl.c
# 	$(CC) -std=gnu99 -ffreestanding -o $(BUILD_DIR)/$@ -c -g $^

# copy.o: $(PREFIX)/copy.S
# 	$(CC) -o $(BUILD_DIR)/$@ -c -g $^

# .PHONY: clean
# clean:
# 	rm -r $(BUILD_DIR)/*.o
