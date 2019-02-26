#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "standard.h"

void print_error() {
  char error_message[30] = "An error has occurred\n";
  write(STDERR_FILENO, error_message, strlen(error_message));
}

// Returns an int representing the type of io redirect found
// -1 error
// 0 input
// 1 double_input
// 2 output
// 3 double_output
// 4 pipe
int has_io_redirect(char **parsed_input) {
  if (parsed_input[0] == NULL)
    return -1;

  for (int i=0; parsed_input[i] != NULL; i++) {
    if (strcmp(parsed_input[i], INPUT) == 0)
      return 0;
    if (strcmp(parsed_input[i], DOUBLE_INPUT) == 0)
      return 1;
    if (strcmp(parsed_input[i], OUTPUT) == 0)
      return 2;
    if (strcmp(parsed_input[i], DOUBLE_OUTPUT) == 0)
      return 3;
    if (strcmp(parsed_input[i], PIPE) == 0)
      return 4;
  }
  
  return -1;
}

// Returns the position in the parsed_input array of the io redirect
int find_io_redirect_position(char **parsed_input) {
  if (parsed_input[0] == NULL)
    return -1;

  for (int i=0; parsed_input[i] != NULL; i++) {
    if (strcmp(parsed_input[i], INPUT) == 0)
      return i;
    if (strcmp(parsed_input[i], DOUBLE_INPUT) == 0)
      return i;
    if (strcmp(parsed_input[i], OUTPUT) == 0)
      return i;
    if (strcmp(parsed_input[i], DOUBLE_OUTPUT) == 0)
      return i;
    if (strcmp(parsed_input[i], PIPE) == 0)
      return i;
  }
  
  return -1;
}
