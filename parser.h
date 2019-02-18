#ifndef _HEADER_NAME_
#define _HEADER_NAME_
#endif

char *read_input();
char **parse_input(char *input);
void interpret_input(char **parsed_input);
void run_internal(char **parsed_input);
