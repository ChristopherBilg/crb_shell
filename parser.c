#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"
#include "standard.h"

char *read_input() {
  char *input = NULL;
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

int run_cd(char **parsed_input) {
  if (parsed_input[1] == NULL) {
      char buffer[BUFFER_SIZE];
      if (getcwd(buffer, BUFFER_SIZE) == NULL)
        print_error();

      printf("%s%s\n", SHELLNAME, buffer);
  }
  else if (chdir(parsed_input[1]) != 0) {
    print_error();
  }

  return 1;
}

int run_clr() {
  system("clear");
  return 1;
}

int run_dir(char **parsed_input) {
  DIR *d;
  if (parsed_input[1] == NULL)
    d = opendir(".");
  else
    d = opendir(parsed_input[1]);

  if (d == NULL) {
    print_error();
    return 1;
  }

  struct dirent *dir;
  while ((dir = readdir(d)) != NULL) {
    printf("\t%s\n", dir->d_name);
  }

  return 1;
}

int run_environ() {
  const char* env_variables[BUFFER_SIZE];
  int size = 0;
  env_variables[size++] = "USER";
  env_variables[size++] = getenv("USER");
  env_variables[size++] = "HOME";
  env_variables[size++] = getenv("HOME");
  env_variables[size++] = "SHELL";
  env_variables[size++] = getenv("SHELL");
  env_variables[size++] = "OS";
  env_variables[size++] = getenv("DESKTOP_SESSION");
  env_variables[size++] = "PWD";
  env_variables[size++] = getenv("PWD");
  env_variables[size++] = "USERNAME";
  env_variables[size++] = getenv("USERNAME");
  env_variables[size++] = "LANG";
  env_variables[size++] = getenv("LANG");

  int index = 0;
  while (index < size) {
    printf("\t%s: %s\n", env_variables[index], env_variables[index+1]);
    index += 2;
  }

  return 1;
}

int run_echo(char **parsed_input) {
  int index = 1;
  while (parsed_input[index]) {
    printf("%s ", parsed_input[index]);
    index++;
  }
  printf("\n");

  return 1;
}

int run_help() {
  // NEEDS TO BE IMPLEMENTED
  return 1;
}

int run_pause() {
  printf("Press 'enter' to continue");
  while (getchar() != '\n');
  return 1;
}

int run_quit() {
  return 0;
}
