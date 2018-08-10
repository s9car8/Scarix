
# Local targets flags.
# ---------------------

# Specify to use GNU GCC version with C99 standard.
CFLAGS += -std=gnu99

CFLAGS += -ffreestanding

# Instruct GCC not to use interrupt unsafe 'red zones'.
CFLAGS += -mno-red-zone

# Disable SSE floating point ops. To protect from #UD and #NM exceptions.
CFLAGS += -mno-sse

CFLAGS += $(KERNELINCLUDE)

# Enalbe debug stuff.
CFLAGS += -g3

LDFLAGS	:= -shared -nostdlib -g3

# Don't embed an OS specific CRT.
LDFLAGS += -ffreestanding


obj		:= $(OBJPATH)/boot

boot-objs := boot.o kernel.o gdt.o pm.o pmjump.o pm_boot.o memory.o terminal.o
boot-objs += io_impl.o
boot-objs := $(addprefix $(obj)/, $(boot-objs))

include $(SRCPATH)/scripts/Kbuild.include


src-arch-boot := $(SRCPATH)/arch/x86/boot
src-arch-lib := $(SRCPATH)/arch/x86/lib

OBJCOPYFLAGS_kernel_boot.bin := #-O binary
$(OBJPATH)/kernel_boot.bin: $(OBJPATH)/kernel_boot.elf
	$(call cmd,objcopy)
	# cp $< $@

LDFLAGS_kernel_boot.elf := -T
$(OBJPATH)/kernel_boot.elf: $(src-arch-boot)/setup.ld $(boot-objs)
	$(call cmd,ld)

$(obj)/boot.o: $(src-arch-boot)/boot.S
	$(call cmd,cc)

$(obj)/kernel.o: $(src-arch-boot)/kernel.c
	$(call cmd,cc)

$(obj)/gdt.o: $(src-arch-boot)/gdt.c
	$(call cmd,cc)

$(obj)/pm.o: $(src-arch-boot)/pm.c
	$(call cmd,cc)

$(obj)/pmjump.o: $(src-arch-boot)/pmjump.S
	$(call cmd,cc)

$(obj)/pm_boot.o: $(src-arch-boot)/pm_boot.S
	$(call cmd,cc)

$(obj)/memory.o: $(src-arch-lib)/memory.S
	$(call cmd,cc)

$(obj)/terminal.o: $(src-arch-boot)/terminal.c
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
