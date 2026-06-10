#include <stdint.h>
#define UART0_BASE 0x09000000UL
#define UART0_DR (*(volatile uint32_t*)(UART0_BASE + 0x00))
#define UART0_FR (*(volatile uint32_t*)(UART0_BASE + 0x18))
#define UART0_FR_TXFF (1<<5)
#define UART0_FR_RXFE (1<<4)
#define SYSCON_BASE 0x09080000UL
#define SYSCON_POWEROFF (*(volatile uint32_t*)(SYSCON_BASE))
#define POWEROFF_MAGIC 0x5555


int uart_getc(void){
  while(UART0_FR & UART0_FR_RXFE);
  return (int)(UART0_DR & 0xFF);
}
void uart_putc(char c){
  while (UART0_FR & UART0_FR_TXFF);
  UART0_DR = (uint32_t) c;
}
void uart_puts(const char *s, int newLineAtEnd){
  while(*s){
    uart_putc(*s++);
  }
  if (newLineAtEnd){
    uart_putc('\r');
    uart_putc('\n');
  }
}
