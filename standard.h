#ifndef STANDARD_H
#define STANDARD_H

#define SHELLNAME "crb_shell$ "
#define INPUT "<"
#define DOUBLE_INPUT "<<"
#define OUTPUT ">"
#define DOUBLE_OUTPUT ">>"
#define PIPE "|"
#define BACKGROUND "&"

void print_error();
int has_io_redirect(char **parsed_input);
int find_io_redirect_position(char **parsed_input);

#endif
