#include "../include/uart.h"
#include "../include/string.h"
#define SYSCON_BASE 0x09080000UL
#define SYSCON_POWEROFF (*(volatile uint32_t*)(SYSCON_BASE))
#define POWEROFF_MAGIC 0x5555

typedef struct {
    const char name[16];
    void (*handler)(int argc, char **argv);
} command_t;



void help_cmd(int argc, char **argv){
  uart_puts("==========================\r\n");
  uart_puts("     Bare Metal\r\n");
  uart_puts("==========================\r\n");
  uart_puts("help - shows this table\r\n");
  uart_puts("clear - clears terminal\r\n");
  uart_puts("exit - exits this ??? (Still don't know how to call it)\r\n");
  return;
}
void poweroff(int argc, char **argv){
  uart_puts("Caught exit prompt, quitting... I hope you enjoyed baremetal program.");
  SYSCON_POWEROFF = POWEROFF_MAGIC;
  while(1){}
}
void clear_cmd(int argc, char **argv){
  uart_puts("\033[2J\033[H");
}
void unknown_cmd(int argc, char **argv){
  uart_puts("I do not recognize this command.\n");
}

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
  for(int i = 1; i < argc; i++){
     uart_puts(argv[i]);
     if (i+1 < argc){
       uart_putc(' ');
     }
  }
  uart_puts("\r\n");
  
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
