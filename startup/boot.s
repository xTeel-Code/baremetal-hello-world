.section ".text.boot"
.global _start

_start:
        ldr x0, = 0x40180000
        mov sp, x0

        ldr x0, = __bss_start
        ldr x1, = __bss_end
        mov x2, #0
bss_loop:
        cmp x0,x1
        bge bss_done
        str x2, [x0], #8
        b bss_loop
bss_done:
        bl main
hang:
        wfe
        b hang
