#include <stdint.h>
#include "../include/commands.h"
#include "../include/uart.h"
#define SYSCON_BASE 0x09080000UL
#define SYSCON_POWEROFF (*(volatile uint32_t*)(SYSCON_BASE))
#define POWEROFF_MAGIC 0x5555

#define MAX_ENTRIES 16
#define MAX_KEY_LEN 32
#define MAX_VAL_LEN 128


typedef struct {
  char key[MAX_KEY_LEN];
  char val[MAX_VAL_LEN];
} entry_t;

static entry_t storage[MAX_ENTRIES];
static int store_count = 0;
int main(void){
  uart_puts("Hello World!\n");
  char term_buf[128];
  int max_args = 6;
  int argc;
  char *argv[8];
  while (1){
    uart_puts("> ");
    read_line(term_buf, sizeof(term_buf));
    argc = parse_args(term_buf,argv,max_args);
    run_command(argc, argv);
  }
}
