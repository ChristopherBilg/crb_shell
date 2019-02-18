#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"
#include "standard.h"

#define BUFFER_SIZE 128
#define DELIMITER " \t\r\n\a"

char *read_input() {
  char *input = NULL;
  // ssize_t input_buffer_size = 0;
  size_t input_buffer_size = 0;
  getline(&input, &input_buffer_size, stdin);
  return input;
}

char **parse_input(char *input) {
  int input_buffer_size = BUFFER_SIZE;
  int index = 0;

  char **all_segments = malloc(input_buffer_size * sizeof(char *));
  char *current_segment;

  current_segment = strtok(input, DELIMITER);
  while (current_segment != NULL) {
    // Add the current segment to the array of all segments
    all_segments[index] = current_segment;
    index++;

    // Reallocate dynamic memory buffer if needed
    if (index >= input_buffer_size) {
      input_buffer_size *= 2;
      void *temp = realloc(all_segments, input_buffer_size * sizeof(char *));

      if (temp == NULL)
        free(temp);
      
      all_segments = temp;
    }

    // Move the current segment to be the next segment with the same delim.
    current_segment = strtok(NULL, DELIMITER);
  }

  // NULL terminated
  all_segments[index] = NULL;
  return all_segments;
}

void interpret_input(char **parsed_input) {
  // First job is to check for internal commands
  char *internal_commands[] = {"cd", "clr", "dir", "environ", "echo", "help", "pause", "quit"};
  
  int index = 0;
  while (internal_commands[index]) {
    if (strcmp(internal_commands[index], parsed_input[0]) == 0)
      run_internal(parsed_input);
    index++;
  }
}

void run_internal(char **parsed_input) {
  if (strcmp(parsed_input[0], "cd") == 0) {
    if (parsed_input[1] == NULL) {
      char buffer[BUFFER_SIZE];
      if (getcwd(buffer, BUFFER_SIZE) == NULL)
        print_error();

      printf("%s%s\n", SHELLNAME, buffer);
    }
    else if (chdir(parsed_input[1]) != 0)
      print_error();
  }
  else if (strcmp(parsed_input[0], "clr")) {
    
  }
  else if (strcmp(parsed_input[0], "dir")) {
    
  }
  else if (strcmp(parsed_input[0], "environ")) {
    
  }
  else if (strcmp(parsed_input[0], "echo")) {
    
  }
  else if (strcmp(parsed_input[0], "help")) {
    
  }
  else if (strcmp(parsed_input[0], "pause")) {
    
  }
  else if (strcmp(parsed_input[0], "quit")) {
    exit(EXIT_SUCCESS);
  }
}
