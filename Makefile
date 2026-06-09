CC      = aarch64-elf-gcc
OBJCOPY = aarch64-elf-objcopy
CFLAGS  = -ffreestanding -nostdlib -nostartfiles -O2 -Wall
LDFLAGS = -T linker.ld -nostdlib
all: kernel.bin
kernel.elf: boot.s main.c
	$(CC) $(CFLAGS) $(LDFLAGS) boot.s main.c -o $@

kernel.bin: kernel.elf
	$(OBJCOPY) -O binary $< $@

run: kernel.bin
	qemu-system-aarch64 \
		-machine virt \
		-cpu cortex-a57 \
		-nographic \
		-kernel kernel.bin

clean:
	rm -f *.elf *.bin
