# Configure make.
# ---------------

# - Do not use make's built-in implicit rules and variables.
# - Look for including makefiles relative to root which contains current Makefile.
# - Disable printing of the working directory.
MAKEFLAGS 	= -rR --include-dir=$(CURDIR) # --no-print-directory

# Setup make to run 'all' as default target.
.DEFAULT_GOAL: all


# Target arch.
NATIVE_ARCH := $(shell uname -m | sed -e s/x86_64/x86/)
ARCH 		:= $(NATIVE_ARCH)

# Make build variables.
AS 			 = as
LD 			 = ld
CC 			 = gcc
OBJCOPY		 = objcopy

# Gets necessary paths to obj(build)-files and src-files.
# Each path variable value must be absolute path, without trailing slash.
SRCPATH		 := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

ifeq ("$(origin O)", "command line")
  OBJPATH := $(O)
  $(info :!: Using command line path: '$(OBJPATH)')
else
  OBJPATH := $(shell pwd)
  $(info :!: Auto-evaluation of the path value: '$(OBJPATH)')
endif

# Now I care about dangerous `rm -rf *`.
ifeq ($(abspath $(OBJPATH)), $(abspath $(SRCPATH)))
  $(error :!: Building in source directory. Remember about dangerous 'rm -rf *'.)
endif

export ARCH AS LD CC OBJCOPY
export OBJPATH SRCPATH

obj		:= $(OBJPATH)
src		:= $(SRCPATH)

# Use KERNELINCLUDE when compile kernel itself.
KERNELINCLUDE 	:= \
	-I$(SRCPATH)/include \
	-I$(SRCPATH)/arch/$(ARCH)/include

export KERNELINCLUDE

# CFLAGS, AFLAGS, LDFLAGS specification.
# --------------------------------------

# Specify to use GNU GCC version with C99 standard.
CFLAGS += -std=gnu99

# Specify include directories.
CFLAGS += $(KERNELINCLUDE)
AFLAGS += $(KERNELINCLUDE)

# Enable debug stuff.
CFLAgS += -g3
AFLAGS += -g3

# Don't embed an OS specific CRT.
CFLAGS += -ffreestanding

# Instruct GCC not to use interrupt unsafe 'red zones'.
CFLAGS += -mno-red-zone

# Disable SSE floating point ops. To protect from #UD and #NM exceptions.
CFLAGS += -mno-sse

CFLAGS += -DCONFIG_USE_MULTIBOOT

build-dir  = $(patsubst %/,%,$(dir $<))
target-dir = $(dir $@)

# define x =
# 	$(Q)$(MAKE) $(build)=$(build-dir) $(target-dir)$(notdir $@)
# endef

%.s: %.c prepare
	$(x)
%.s: %.S prepare
	$(x)
%.o: %.c prepare
	$(x)
%.o: %.S prepare
	$(x)


# Major targets.
# --------------

.PHONY: all
all: $(obj)/kernel_boot.bin
	@echo
	@echo $(CC)
	@echo $(LD)
	@echo $(ARCH)
	@echo $(OBJPATH)
	@echo $(SRCPATH)
	@echo


.ONESHELL:
$(obj)/os.iso: $(obj)/kernel_boot.bin
	mkdir -p $(OBJPATH)/iso/boot/grub
	mv $(OBJPATH)/kernel_boot.bin $(OBJPATH)/iso/boot/kernel_boot.bin
	if ! grub-file --is-x86-multiboot $(OBJPATH)/iso/boot/kernel_boot.bin ; then
		echo 'ERROR: >>> ISO file is not multiboot! <<<'
		exit 1
	fi
	cp $(SRCPATH)/grub.cfg $(OBJPATH)/iso/boot/grub/grub.cfg
	grub-mkrescue -o $@ $(OBJPATH)/iso

.PHONY: test
test: $(obj)/os.iso
	# Use x86_64 version fo QEMU to emulate amd64 processor architecture.
	qemu-system-x86_64 -cdrom $(obj)/os.iso

.PHONY: debug
.ONESHELL:
debug: $(obj)/os.iso
	ln -sf $(SRCPATH)/.gdbinit $(OBJPATH)/.gdbinit
	qemu-system-i386 -cdrom $(obj)/os.iso -s -S &
	gdb

.PHONY: clear
clear:
	rm -rfv $(OBJPATH)/*

# .PHONY: $(PHONY)
