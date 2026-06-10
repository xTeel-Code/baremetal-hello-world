#include <stdint.h>
#include "../include/commands.h"
#include "../include/uart.h"
#define SYSCON_BASE 0x09080000UL
#define SYSCON_POWEROFF (*(volatile uint32_t*)(SYSCON_BASE))
#define POWEROFF_MAGIC 0x5555
int main(void){
  uart_puts("Hello World!",1);
  char term_buf[128];
  int max_args = 6;
  int argc;
  char *argv[8];
  while (1){
    uart_puts("> ",0);
    read_line(term_buf, sizeof(term_buf));
    argc = parse_args(term_buf,argv,max_args);
    run_command(argc, argv);
  }
}
