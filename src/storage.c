#include "../include/uart.h"
#include "../include/string.h"
typedef struct {
  char key[16];
  char value[256];
}entry_t;
static entry_t entry;
int store_count = 0;
void write(int argc, char **argv) {
    if (argc < 3) {
        uart_puts("usage: save <key> <value>",1);
        return;
    }

    str_cpy(argv[1], entry.key);
    entry.value[0] = '\0';

    for (int i = 2; i < argc; i++) {
        if (i > 2) {
            str_cat(entry.value, " ");
        }
        str_cat(entry.value, argv[i]);
    }
    

   uart_puts("\r\nsaved",1);
}
void read(int argc, char **argv) {
    if (argc < 2) {
        uart_puts("usage: get <key>",0);
        return;
    }

    if (str_eq(argv[1], entry.key)) {
        uart_puts(entry.value,1);
        } else {
        uart_puts("not found",1);
    }
}
