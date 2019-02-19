#include <string.h>
#include "main.h"
#include "parser.h"
#include "standard.h"

void interpret_input(char **parsed_input) {
  // First job is to check for internal commands
  char *internal_commands[] = {"cd", "clr", "dir", "environ", "echo", "help", "pause", "quit"};
  
  int index = 0;
  while (internal_commands[index] != NULL) {
    if (strcmp(internal_commands[index], parsed_input[0]) == 0)
      run_internal(parsed_input);
    index++;
  }
}

void run_internal(char **parsed_input) {
  if (strcmp(parsed_input[0], "cd") == 0) {
    run_cd(parsed_input);
  }
  else if (strcmp(parsed_input[0], "clr") == 0) {
    run_clr();
  }
  else if (strcmp(parsed_input[0], "dir") == 0) {
    run_dir(parsed_input);
  }
  else if (strcmp(parsed_input[0], "environ") == 0) {
    run_environ(parsed_input);
  }
  else if (strcmp(parsed_input[0], "echo") == 0) {
    run_echo(parsed_input);
  }
  else if (strcmp(parsed_input[0], "help") == 0) {
    run_help();
  }
  else if (strcmp(parsed_input[0], "pause") == 0) {
    run_pause();
  }
  else if (strcmp(parsed_input[0], "quit") == 0) {
    run_quit();
  }
}
