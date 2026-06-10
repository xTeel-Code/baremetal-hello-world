#ifndef COMMANDS_H
#define COMMANDS_H
#include "uart.h"
struct command_t;
void help_cmd(int argc, char **argv);
void poweroff(int argc, char **argv);
void clear_cmd(int argc, char **argv);
void unknown_cmd(int argc, char **argv);
int parse_args(char *line, char **argv, int max_args);
void read_line(char *term_buf,int max);
void echo(int argc, char **argv);
void run_command(int argc, char **argv);

#endif
