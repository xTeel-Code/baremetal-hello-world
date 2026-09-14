# baremetal-hello-world

A tiny AArch64 bare-metal kernel: it boots on QEMU's `virt` machine, talks to the
PL011 UART, and drops you into a small interactive shell over the serial line.

No bootloader, no libc, no OS — just a linker script, an assembly entry point,
and a handful of C files.

```
Hello World!
Press CTRL+C to exit
> help
==========================
     Bare Metal
==========================
help - shows this table
clear - clears terminal
exit - exits this ???
...
> echo hello there
hello there
> save greeting hello world
saved
> read greeting
hello world
```

## Requirements

The Makefile targets `aarch64-elf-gcc` / `aarch64-elf-objcopy` — this was
developed on macOS with the Homebrew cross toolchain, hence the `aarch64-elf-`
prefix. On most Linux distros the same toolchain is called
`aarch64-linux-gnu-gcc`, so adjust `CC` / `OBJCOPY` at the top of the Makefile
(or override on the command line) to match what you have.

macOS:

```sh
brew install --cask gcc-aarch64-embedded   # or: brew install aarch64-elf-gcc
brew install qemu
```

Debian / Ubuntu:

```sh
sudo apt install gcc-aarch64-linux-gnu qemu-system-arm
make CC=aarch64-linux-gnu-gcc OBJCOPY=aarch64-linux-gnu-objcopy
```

## Build and run

```sh
make          # builds build/kernel.elf and build/kernel.bin
make run      # builds, then boots it under QEMU with serial on stdio
make clean    # removes build/
```

`make run` starts QEMU with:

```
qemu-system-aarch64 -machine virt -cpu cortex-a57 -display none \
  -device loader,file=build/kernel.bin,addr=0x40100000,cpu-num=0 \
  -serial stdio
```

Serial output lands in your terminal and your keystrokes go to the kernel.
Type `exit` to power the machine off via the syscon register; if that ever
fails, `Ctrl-A` then `x` kills QEMU.

## How it boots

1. QEMU loads `kernel.bin` at `0x40100000` and starts the CPU there.
2. `startup/boot.s` (`.text.boot`, placed first by the linker script) sets the
   stack pointer to `0x40180000`, zeroes `.bss` between `__bss_start` and
   `__bss_end`, and branches to `main`.
3. `src/main.c` prints the banner and loops: print prompt → `read_line` →
   `parse_args` → `run_command`.
4. If `main` ever returns, `boot.s` parks the core in a `wfe` loop.

`linker.ld` places everything at `0x40100000` in a 128 MB RAM region, with
`.text.boot` guaranteed to be the first thing in the image.

## Layout

```
startup/boot.s    entry point: stack setup, .bss zeroing, jump to main
linker.ld         memory map, load address, __bss_start/__bss_end symbols
src/main.c        kernel entry, the REPL loop
src/command.c     command table, built-in commands, line editing, arg parsing
src/storage.c     the save/read key-value store
src/string.c      minimal str_cpy / str_len / str_cat / str_eq
src/uart.c        PL011 UART at 0x09000000: putc / puts / getc
include/          headers for the above
main.c            stale duplicate of src/main.c — not built (see TODO.md)
```

## Hardware touched

| What | Address | Used for |
|---|---|---|
| PL011 UART0 data | `0x09000000` | character in/out |
| PL011 UART0 flags | `0x09000018` | TX-full / RX-empty polling |
| syscon | `0x09080000` | write `0x5555` to power off |

Both are the standard QEMU `virt` mappings. UART access is polled — no
interrupts, no vector table yet.

## Shell

| Command | Description |
|---|---|
| `help` | list the commands |
| `clear` | clear the screen (`\033[2J\033[H`) |
| `echo <text...>` | print the arguments back |
| `save <key> <value...>` | store a value |
| `read <key>` | print the stored value |
| `exit` | power off the machine |

`<cmd> -h` prints that command's one-line description. Line editing supports
backspace and `Ctrl-C` to clear the current line.

Storage currently holds exactly one entry, and arguments are capped at 6 per
line with a 128-byte input buffer.

## Status

This is a learning project and a work in progress — see [TODO.md](TODO.md) for
known bugs and what's next.
