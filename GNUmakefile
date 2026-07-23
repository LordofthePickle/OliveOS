PROJECT := oliveos
BUILD_DIR := build
KERNEL := $(BUILD_DIR)/$(PROJECT).elf
LINKER_SCRIPT := kernel/arch/x86_64/linker.ld

ISO := $(BUILD_DIR)/$(PROJECT).iso
ISO_ROOT := $(BUILD_DIR)/iso_root

LIMINE_VERSION := 12.3.2
LIMINE_ARCHIVE := $(BUILD_DIR)/limine-binary-$(LIMINE_VERSION).tar.gz
LIMINE_DIR := $(BUILD_DIR)/limine-binary
LIMINE_URL := https://github.com/Limine-Bootloader/Limine/releases/download/v$(LIMINE_VERSION)/limine-binary.tar.gz

XORRISO := /usr/local/bin/xorriso
QEMU := /opt/homebrew/bin/qemu-system-x86_64
OVMF := /opt/homebrew/opt/qemu/share/qemu/edk2-x86_64-code.fd

CXX := /opt/homebrew/opt/llvm/bin/clang++
LD := /opt/homebrew/bin/ld.lld

CPPFLAGS := \
	-I kernel/include \
	-I third_party/limine-protocol/include

CXXFLAGS := \
	-target x86_64-unknown-none-elf \
	-std=gnu++20 \
	-g \
	-O2 \
	-Wall \
	-Wextra \
	-ffreestanding \
	-fno-rtti \
	-fno-exceptions \
	-fno-stack-protector \
	-fno-stack-check \
	-fno-lto \
	-fno-PIC \
	-ffunction-sections \
	-fdata-sections \
	-m64 \
	-march=x86-64 \
	-mabi=sysv \
	-mno-80387 \
	-mno-mmx \
	-mno-sse \
	-mno-sse2 \
	-mno-red-zone \
	-mcmodel=kernel

LDFLAGS := \
	-m elf_x86_64 \
	-nostdlib \
	-static \
	-z max-page-size=0x1000 \
	--gc-sections \
	-T $(LINKER_SCRIPT)

SOURCES := $(shell find kernel/src kernel/arch/x86_64 -type f -name '*.cpp' | sort)
OBJECTS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
DEPENDENCIES := $(OBJECTS:.o=.d)

.PHONY: all clean iso run

all: $(KERNEL)

$(KERNEL): $(OBJECTS) $(LINKER_SCRIPT)
	@mkdir -p $(dir $@)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(DEPENDENCIES)

iso: $(ISO)

run: $(ISO)
	$(QEMU) \
		-M q35 \
		-m 256M \
		-drive if=pflash,unit=0,format=raw,file=$(OVMF),readonly=on \
		-cdrom $(ISO) \
		-no-reboot \
		-no-shutdown

$(LIMINE_DIR)/limine-uefi-cd.bin:
	@mkdir -p $(BUILD_DIR)
	/usr/bin/curl -L --fail \
		--output $(LIMINE_ARCHIVE) \
		$(LIMINE_URL)
	/usr/bin/tar -xzf $(LIMINE_ARCHIVE) -C $(BUILD_DIR)

$(ISO): $(KERNEL) boot/limine.conf $(LIMINE_DIR)/limine-uefi-cd.bin
	rm -rf $(ISO_ROOT)
	mkdir -p $(ISO_ROOT)/boot/limine
	mkdir -p $(ISO_ROOT)/EFI/BOOT
	cp $(KERNEL) $(ISO_ROOT)/boot/oliveos.elf
	cp boot/limine.conf $(ISO_ROOT)/boot/limine/limine.conf
	cp $(LIMINE_DIR)/limine-uefi-cd.bin $(ISO_ROOT)/boot/limine/
	cp $(LIMINE_DIR)/BOOTX64.EFI $(ISO_ROOT)/EFI/BOOT/
	$(XORRISO) -as mkisofs \
		-R -r -J \
		-hfsplus \
		-apm-block-size 2048 \
		--efi-boot boot/limine/limine-uefi-cd.bin \
		-efi-boot-part \
		--efi-boot-image \
		--protective-msdos-label \
		$(ISO_ROOT) \
		-o $(ISO)

clean:
	rm -rf build