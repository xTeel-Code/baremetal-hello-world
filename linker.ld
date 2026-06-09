ENTRY(_start)
MEMORY {
  RAM (rwx) : ORIGIN = 0x40000000, LENGTH = 128M 
}
SECTIONS {
  . = 0x40000000;
  .text : {
    *(.text.boot)
    *(.text*)
    *(.rodata*)
  } > RAM
  .data : {
    *(.data*)
  } > RAM
  .bss : {
    __bss_start = .;
    *(.bss*)
    *(COMMON)
    __bss_end = .;
  } > RAM
}
