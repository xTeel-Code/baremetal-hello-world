#include <stdint.h>
#include "include/commands.h"
#include "include/uart.h"
#include "include/string.h"
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
typedef struct {
    const char name[16];
    void (*handler)(int argc, char **argv);
} command_t;


int parse_args(char *line, char **argv, int max_args) {
    int argc = 0;

    while (*line && argc < max_args) {
        while (*line == ' ' || *line == '\t') {
            line++;
        }

        if (*line == '\0') {
            break;
        }

        argv[argc++] = line;

        while (*line && *line != ' ' && *line != '\t') {
            line++;
        }

        if (*line == '\0') {
            break;
        }

        *line = '\0';
        line++;
    }

    return argc;
}

void read_line(char *term_buf,int max){
  int i = 0;
  for (int j = 0; j < max;j++) term_buf[j] = 0;
  while(1){
    char c = uart_getc();

    if (c == '\r' || c == '\n'){
      term_buf[i] = '\0';
      uart_puts("\r\n");
      return;
    }
    if ((c == 0x7F || c == 0x08) && i > 0){
      i--;
      uart_puts("\b \b");
      continue;
    }
    if (c == 0x03) {
      while (i--) uart_puts("\b \b");
        i = 0;
        continue;
      }

    if (i < max - 1) {
        term_buf[i++] = c;
        uart_putc(c);
    }
  }
}
void echo(int argc, char **argv){
  uart_puts(*argv);
}

static command_t commands[] = {
  {"exit",poweroff},
  {"clear", clear_cmd},
  {"help", help_cmd},
  {"echo", echo}
};
void run_command(int argc,char **argv) {
    if (argc == 0) return;
    int count = sizeof(commands) / sizeof(commands[0]);
    for (int i = 0; i < count; i++) {
            if (str_eq(argv[0], commands[i].name)) {
            commands[i].handler(argc,argv);
            return;
        }
    }
    unknown_cmd(argc,argv);
}
int main(void){
  uart_puts("Hello World!",1);
  char term_buf[128];
  int max_args = 6;
  int argc;
  char *argv[8];
  uart_puts("Press CTRL+C to exit",1);
  while (1){
    uart_puts("> ",0);
    read_line(term_buf, sizeof(term_buf));
    argc = parse_args(term_buf,argv,max_args);
    run_command(argc, argv);
  }
}
