#include <stdint.h>


#define UART0_BASE 0x09000000UL
#define UART0_DR (*(volatile uint32_t*)(UART0_BASE + 0x00))
#define UART0_FR (*(volatile uint32_t*)(UART0_BASE + 0x18))
#define UART0_FR_TXFF (1<<5)
#define UART0_FR_RXFE (1<<4)

#define SYSCON_BASE 0x09080000UL
#define SYSCON_POWEROFF (*(volatile uint32_t*)(SYSCON_BASE))
#define POWEROFF_MAGIC 0x5555

typedef struct {
  const char name[16];
  void (*handler)(void);
}command_table;

int uart_getc(void){
  while(UART0_FR & UART0_FR_RXFE);
  return (int)(UART0_DR & 0xFF);
}
void uart_putc(char c){
  while (UART0_FR & UART0_FR_TXFF);
  UART0_DR = (uint32_t) c;
}
void uart_puts(const char *s){
  while(*s){
    uart_putc(*s++);
  }
}

int str_eq(const char *a, const char *b) {
    while(*a && * b && *a == *b){
      a++;
      b++;  
    }
    return *a == '\0' && *b == '\0';
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
void help_cmd(void){
  uart_puts("help - shows this table\r\n");
  uart_puts("clear - clears terminal\r\n");
  uart_puts("exit - exits this ??? (Still don't know how to call it)\r\n");
  return;
}
void poweroff(void){
  uart_puts("Caught exit prompt, quitting... I hope you enjoyed baremetal program.");
  SYSCON_POWEROFF = POWEROFF_MAGIC;
  while(1){}
}
void clear_cmd(void){
  uart_puts("\033[2J\033[H");
}
void unknow_cmd(void){
  uart_puts("I do not recognize this command.\n");
}
static command_table commands[] = {
  {"exit",poweroff},
  {"clear", clear_cmd},
  {"help", help_cmd}
};
void run_command(const char *prompt) {
    if (str_eq(prompt, "")) return;
    int count = sizeof(commands) / sizeof(commands[0]);
    for (int i = 0; i < count; i++) {
            if (str_eq(prompt, commands[i].name)) {
            commands[i].handler();
            return;
        }
    }
    unknow_cmd();
}
int main(void){
  uart_puts("Hello World!\n");
  char term_buf[128];
  uart_puts("Press CTRL+C to exit\n");
  while (1){
    uart_puts("> ");
    read_line(term_buf, sizeof(term_buf));
    run_command(term_buf);
  }
}
