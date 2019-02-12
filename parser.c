#include <stdio.h>
#include <string.h>
#include "parser.h"

char *read_input() {
  char *input = NULL;
  // ssize_t input_buffer_size = 0;
  size_t input_buffer_size = 0;
  getline(&input, &input_buffer_size, stdin);
  return input;
}

char **parse_input(char *input) {
  int input_buffer_size = 128;
  int position = 0;
}
