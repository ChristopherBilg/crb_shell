#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "standard.h"

int main() {
  printf("Enter some input: ");
  char *test_input = read_input();
  printf("Test Input: %s", test_input);

  char **parsed_input = parse_input(test_input);
  int parsed_input_size = count_arguments(parsed_input);
  for (int i=0; i<parsed_input_size; i++)
    printf("[%s] ", parsed_input[i]);
  printf("\n");

  print_error();

  int io_redirect = has_io_redirect(parsed_input);
  printf("I/O Redirect? %d\n", io_redirect);
  int io_redirect_position = find_io_redirect_position(parsed_input);
  printf("I/O Redirect Position: %d\n", io_redirect_position);
  
  return EXIT_SUCCESS;
}
