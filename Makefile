CC = aarch64-elf-gcc
OBJCOPY = aarch64-elf-objcopy

CFLAGS = -ffreestanding -nostdlib -nostartfiles -O0 -Wall -Iinclude -MMD -MP
LDFLAGS = -T linker.ld -nostdlib

BUILD_DIR = build
SRC_DIR = src

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS)) $(BUILD_DIR)/boot.o
DEPS = $(OBJS:.o=.d)

TARGET_ELF = $(BUILD_DIR)/kernel.elf
TARGET_BIN = $(BUILD_DIR)/kernel.bin

all: $(TARGET_BIN)

$(TARGET_ELF): $(OBJS) linker.ld | $(BUILD_DIR)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(TARGET_BIN): $(TARGET_ELF)
	$(OBJCOPY) -O binary $< $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/boot.o: startup/boot.s | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: $(TARGET_BIN)
	qemu-system-aarch64 \
		-machine virt \
		-cpu cortex-a57 \
		-display none \
		-device loader,file=$(TARGET_BIN),addr=0x40100000,cpu-num=0 \
		-serial stdio

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)
