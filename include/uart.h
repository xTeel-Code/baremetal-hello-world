#ifndef UART_H
#define UART_H

#include <stdint.h>

#define UART0_BASE 0x09000000UL
#define UART0_DR   (*(volatile uint32_t *)(UART0_BASE + 0x00))
#define UART0_FR   (*(volatile uint32_t *)(UART0_BASE + 0x18))

#define UART0_FR_TXFF (1U << 5)
#define UART0_FR_RXFE (1U << 4)

void uart_putc(char c);
void uart_puts(const char *s);
int uart_getc(void);

#endif
