ENTRY(_start)
MEMORY {
  RAM (rwx) : ORIGIN = 0x40100000, LENGTH = 128M 
}
SECTIONS {
  . = 0x40100000;
  .text : {
    *(.text.boot)
    *(.text*)
  } > RAM
  .rodata :{
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
