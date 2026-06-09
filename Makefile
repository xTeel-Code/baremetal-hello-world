CC      = aarch64-elf-gcc
OBJCOPY = aarch64-elf-objcopy
CFLAGS  = -ffreestanding -nostdlib -nostartfiles -O0 -Wall
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
		-monitor none \
		-device loader,file=kernel.bin,addr=0x40100000,cpu-num=0 \
		-serial stdio

clean:
	rm -f *.elf *.bin
