#ifndef PARSER_H
#define PARSER_H

#define BUFFER_SIZE 128
#define DELIMITER " \t\r\n\a"

char *read_input();
char **parse_input(char *input);

int run_cd(char **parsed_input);
int run_clr();
int run_dir(char **parsed_input);
int run_environ();
int run_echo(char **parsed_input);
int run_help();
int run_pause();
int run_quit();

#endif
