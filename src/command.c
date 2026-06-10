#include "../include/uart.h"
#include "../include/string.h"
#include "../include/storage.h"
#define SYSCON_BASE 0x09080000UL
#define SYSCON_POWEROFF (*(volatile uint32_t*)(SYSCON_BASE))
#define POWEROFF_MAGIC 0x5555

typedef struct {
    const char name[16];
    void (*handler)(int argc, char **argv);
    const char desc[256];
} command_t;



void help_cmd(int argc, char **argv){
  uart_puts("==========================",1);
  uart_puts("     Bare Metal",1);
  uart_puts("==========================",1);
  uart_puts("help - shows this table",1);
  uart_puts("clear - clears terminal",1);
  uart_puts("exit - exits this ??? (Still don't know how to call it)",1);
  return;
}
void poweroff(int argc, char **argv){
  uart_puts("Caught exit prompt, quitting... I hope you enjoyed baremetal program.",1);
  SYSCON_POWEROFF = POWEROFF_MAGIC;
  while(1){}
}
void clear_cmd(int argc, char **argv){
  uart_puts("\033[2J\033[H",0);
}
void unknown_cmd(int argc, char **argv){
  uart_puts("I do not recognize this command.",1);
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
      uart_puts("\r\n",0);
      return;
    }
    if ((c == 0x7F || c == 0x08) && i > 0){
      i--;
      uart_puts("\b \b",0);
      continue;
    }
    if (c == 0x03) {
      while (i--) uart_puts("\b \b",0);
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
     uart_puts(argv[i],0);
     if (i+1 < argc){
       uart_putc(' ');
     }
  }
  uart_puts("\r\n",0);
  
}

static command_t commands[] = {
  {"exit",poweroff, "Turns Off this."},
  {"clear", clear_cmd, "Clears Terminal"},
  {"help", help_cmd, "Shows Detailed Help"},
  {"echo", echo, "Prints Output"},
  {"save", write, "Writes to memory to store data"},
  {"read", read, "Reads From Memory"}
};
void run_command(int argc,char **argv) {
    if (argc == 0) return;
    int count = sizeof(commands) / sizeof(commands[0]);
    for (int i = 0; i < count; i++) {
            if (str_eq(argv[0], commands[i].name) ) {
            if (str_eq(argv[1], "-h")){
               uart_puts(commands[i].desc,1);
              return;
            }
            commands[i].handler(argc,argv);
            return;
        }
    }
    unknown_cmd(argc,argv);
}
