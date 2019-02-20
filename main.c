#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "main.h"
#include "parser.h"
#include "standard.h"

int main() {
  int return_code;

  do {
    char *input;
    char **arguments;
    
    printf("%s", SHELLNAME);

    input = read_input();
    arguments = parse_input(input);

    return_code = run_execution(arguments);
  }
  while (return_code != 0);
  
  return 1;
}

char *internal_commands[] = {"cd", "clr", "dir", "environ", "echo", "help", "pause", "quit"};
int (*internal_functions[]) (char**) = {&run_cd, &run_clr, &run_dir, &run_environ, &run_echo, &run_help, &run_pause, &run_quit};

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

int start_process(char **process_input) {
  pid_t process_id;
  pid_t wait_process_id;

  int continue_ = 0;
  int background = 0;
  int waiting;

  char process_name_to_run[strlen(process_input[0])];
  memset(process_name_to_run, '\0', sizeof(process_name_to_run));

  // Check for background process (background_)
  if (process_input[0][strlen(process_input[0]-1)] == '&') {
    // Shorten the length of the process_input by one element off of the end
    strncpy(process_name_to_run, process_input[0], strlen(process_input[0]-1));
    background = 1;
  }
  else {
    strcpy(process_name_to_run, process_input[0]);
  }

  process_id = fork();
  if (process_id == 0) {
    // Child will run this
    execvp(process_name_to_run, process_input);
  }
  else {
    // Parent will run this
    if (!background) {
      do {
        wait_process_id = waitpid(process_id, &waiting, WUNTRACED);
      }
      while (!WIFEXITED(waiting) && !WIFSIGNALED(waiting));
    }
  }

  return 1;
}

int run_execution(char **process_input) {
  // No command given, or NULL command
  if (process_input[0] == NULL)
    return 1;

  // Find position of io redirect if it exists
  int io_redirect = has_io_redirect(process_input);
  int io_redirect_location = find_io_redirect_position(process_input);

  // No io redirect
  if (io_redirect < 0 || io_redirect_location < 0) {
    // 8 is the number of internal commands to check for
    for (int i=0; i<8; i++) {
      if (strcmp(process_input[0], internal_commands[i]) == 0)
        return (*internal_functions[i])(process_input);
    }

    // If not an internal command, return here
    return start_process(process_input);
  }
  else {
    // There is an io redirect
    int process_input_length = count_arguments(process_input);

    // Make a copy of the arguments on the left of the io redirect
    int index1 = 0;
    char *left_side_arguments[io_redirect_location + 1];
    for (index1=0; index1<io_redirect_location; index1++)
      left_side_arguments[index1] = process_input[index1];
    left_side_arguments[index1++] = NULL; // NULL terminated

    // Make a copy of the arguments on the right of the io redirect
    int index2 = 0;
    char *right_side_arguments[process_input_length - io_redirect_location];
    for (index2=0; process_input[index1] != NULL; index1++, index2++)
      right_side_arguments[index2] = process_input[index1];
    right_side_arguments[index2] = NULL; // NULL terminated

    // Check for io redirects and run command
    if (io_redirect == 0) // < truncate
      return run_io_redirect(left_side_arguments, right_side_arguments, true);
    else if (io_redirect == 1) // << append
      return run_io_redirect(left_side_arguments, right_side_arguments, true);
    else if (io_redirect == 2) // > truncate
      return run_io_redirect(left_side_arguments, right_side_arguments, false);
    else if (io_redirect == 3) // >> append
      return run_io_redirect(left_side_arguments, right_side_arguments, false);
    else if (io_redirect == 4) // | pipe
      return run_io_pipe(left_side_arguments, right_side_arguments);
  }
  
  return 1;
}

int run_io_redirect(char **left_side_arguments,
                    char **right_side_arguments,
                    _Bool input) {
  pid_t process_id;
  int return_code, fd; // return code and file descriptor
  char *filename = right_side_arguments[0];

  if (input == true) {
    // input
    fd = open(filename, O_RDONLY, 0755);
    
    if (fd == -1) {
      print_error();
      return EXIT_FAILURE;
    }

    dup2(fd, STDIN_FILENO);
    run_execution(right_side_arguments);
  }
  else {
    // output
    fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0755);

    if (fd == -1) {
      print_error();
      return EXIT_FAILURE;
    }

    dup2(fd, STDOUT_FILENO);
    run_execution(left_side_arguments);
  }
  
  return 1;
}
int run_io_pipe(char **left_side_arguments, char **right_side_arguments) {
  return 1;
}
