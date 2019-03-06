#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"
#include "standard.h"

// This function will read the input from the user and return it as a char* (array)
char *read_input() {
  char *input = NULL;
  size_t input_buffer_size = 0;
  getline(&input, &input_buffer_size, stdin);
  return input;
}

// The will take the users input and parse it into individual
// tokens, ie: the command, arguments, redirect, pipe, etc.
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

// Simple function to sum up the number of arguments given by
// a parsed user's input. (including the command name)
int count_arguments(char **parsed_input) {
  int counter = 0;
  for (int i=0; parsed_input[i] != NULL; i++)
    counter++;
  return counter;
}

// Runs the internal command: cd (change directory)
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

// Runs the internal command: clr (clear screen)
int run_clr() {
  system("clear");
  return 1;
}

// Runs the internal command: dir (list's the current directory contents)
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

// Runs the internal command: environ (list's environment variables)
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

// Runs the internal command: echo (prints all arguments out)
int run_echo(char **parsed_input) {
  int index = 1;
  while (parsed_input[index]) {
    printf("%s ", parsed_input[index]);
    index++;
  }
  printf("\n");

  return 1;
}

// Runs the internal command: help
// (gives the user a helpful guide to the crb_shell program)
int run_help() {
  printf("crb_shell User Manual:\n");
  printf("  Internal Commands:\n");
  printf("\tcd <directory>: Change the directory to the given <directory>\n");
  printf("\tclr: clear the screen\n");
  printf("\tdir: list the contents of the current working directory\n");
  printf("\tenviron: list some of the local system environment variables\n");
  printf("\techo <arguments...>: print out to given arguments\n");
  printf("\thelp: list helpful user information for the crb_shell program\n");
  printf("\tpause: pause the shell until the 'enter' key is pressed\n");
  printf("\tquit: quit the crb_shell program, and return to calling shell program\n");
  printf("  External Commands:\n");
  printf("\tAll external commands are supported.\n");
  printf("  Input redirects: < or <<\n");
  printf("\tUsage: com1 args... < filename: This will use the contents of filename as the input for com1.\n");
  printf("\tUsage: com1 args... << filename: This will use the contents of filename as the input for com1.\n");
  printf("  Output redirects: > or >>\n");
  printf("\tUsage: com1 args... > filename: This will append the output of com1 to filename.\n");
  printf("\tUsage: com1 args... >> filename: This will truncate the output of com1 to filename.\n");
  printf("  Command piping: |\n");
  printf("\tUsage: com1 args... | com2 args...: This will link the output of com1 to the input of com2.\n");
  return 1;
}

// Runs the internal command: pause
// (pauses the crb_shell until the 'enter' key is pressed)
int run_pause() {
  printf("Press 'enter' to continue");
  while (getchar() != '\n');
  return 1;
}

// Runs the internal command: quit (quits the crb_shell program)
int run_quit() {
  exit(EXIT_SUCCESS);
  return 0;
}
