#ifndef MAIN_H
#define MAIN_H
#include <stdbool.h>

int main();
void interpret_input(char **parsed_input);
void run_internal(char **parsed_input);

int start_process(char **process_input);
int run_execution(char **process_input);
int run_io_redirect(char **left_side_arguments,
                    char **right_side_arguments,
                    _Bool input);
int run_io_pipe(char **left_side_arguments, char **right_side_arguments);
#endif
