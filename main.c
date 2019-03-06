#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "main.h"
#include "parser.h"
#include "standard.h"

#define LINE_SIZE 1024

// The main function that is called when the program is started
int main(int argc, char **argv) {
  // Check for arguments (batch file)
  if (argc > 1) {
    FILE *file;
    if ((file = fopen(argv[1], "r")) == 0) {
      print_error();
      return EXIT_FAILURE;
    }

    char line[LINE_SIZE];
    while (1) {
      fgets(line, LINE_SIZE, file);

      if (feof(file))
        break;

      int str_len = strlen(line);
      if (line[str_len-1] == '\n')
        line[str_len-1] = 0;

      printf("%s\"%s\"\n", SHELLNAME, line);
      char **args = parse_input(line);
      run_execution(args);
    }
    
    fclose(file);
  }
  // No batch file arguments
  else {
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

    return EXIT_SUCCESS;
  }
}

// Placeholders for internal commands and their associated functions (ORDER MATTERS)
char *internal_commands[] = {"cd", "clr", "dir", "environ", "echo", "help", "pause", "quit"};
int (*internal_functions[]) (char**) = {&run_cd, &run_clr, &run_dir, &run_environ,
                                        &run_echo, &run_help, &run_pause, &run_quit};

int start_process(char **process_input, int input, int filedesc) {
  pid_t process_id;
  int background = 0;

  char process_name_to_run[strlen(process_input[0])];
  memset(process_name_to_run, '\0', sizeof(process_name_to_run));

  // Check for background process (background_)
  if (strcmp(process_input[count_arguments(process_input)-1], "&") == 0) {
    // Shorten the length of the process_input by one element off of the end
    process_input[count_arguments(process_input)-1] = NULL;
    background = 1;
  }

  strcpy(process_name_to_run, process_input[0]);

  process_id = fork();
  if (process_id == 0) {
    // Child will run this
    if (background) {
      int devNull = open("/dev/null", O_WRONLY);
      dup2(devNull, STDOUT_FILENO);
    }
    else if (filedesc != -1) {
      if (input == 0) // output
        dup2(filedesc, 1);
      else
        dup2(filedesc, 0);
    }
    execvp(process_name_to_run, process_input);
  }
  else {
    // Parent will run this
    if (!background)
      wait(NULL);
  }

  return 1;
}

// Executes a given command (with arguments) by passing it to
// the above start_process(...) function
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
    return start_process(process_input, -1, -1);
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
      return run_io_redirect(left_side_arguments, right_side_arguments, true, false);
    else if (io_redirect == 1) // << append
      return run_io_redirect(left_side_arguments, right_side_arguments, true, true);
    else if (io_redirect == 2) // > truncate
      return run_io_redirect(left_side_arguments, right_side_arguments, false, false);
    else if (io_redirect == 3) // >> append
      return run_io_redirect(left_side_arguments, right_side_arguments, false, true);
    else if (io_redirect == 4) // | pipe
      return run_io_pipe(left_side_arguments, right_side_arguments);
  }

  return 1;
}

// Takes in the parsed aguments for io redirects (input and output)
// and then startts the program by passing the correct arguments
// to the start_process(...) function. This is all after the correct
// file descriptor is opened.
int run_io_redirect(char **left_side_arguments,
                    char **right_side_arguments,
                    _Bool input, _Bool append) {
  int fd;
  int error = 1;
  // The IO redirect is an input (< or <<)
  if (input == true) {
    // Append the file contents
    if (append == true) {
      if ((fd = open(*right_side_arguments, O_CREAT | O_RDWR | O_APPEND)) < 0) {
        print_error();
        return error;
      }

      return start_process(left_side_arguments, 1, fd);
    }
    // Truncate the file contents
    else {
      if ((fd = open(*right_side_arguments, O_CREAT | O_RDWR | O_TRUNC)) < 0) {
        print_error();
        return error;
      }

      return start_process(left_side_arguments, 1, fd);
    }
  }
  // The IO redirect is an output (> or >>)
  else {
    // Append the file contents
    if (append == true) {
      if ((fd = open(*right_side_arguments, O_CREAT | O_RDWR | O_APPEND, S_IRWXU)) < 0) {
        print_error();
        return error;
      }

      return start_process(left_side_arguments, 0, fd);
    }
    // Truncate the file contents
    else {
      if ((fd = open(*right_side_arguments, O_CREAT | O_RDWR | O_TRUNC, S_IRWXU)) < 0) {
        print_error();
        return error;
      }

      return start_process(left_side_arguments, 0, fd);
    }
  }
  
  return 1;
}

// Handled the piping of multiple commands together by linking the output
// of the first command to the input of the second command.
// After this, the start_process(...) function is called to execute.
int run_io_pipe(char **left_side_arguments, char **right_side_arguments) {
  int error = 1;
  int child_pid = fork();
  
  if (child_pid < 0) {
    print_error();
    return error;
  }
  else if (child_pid == 0) {
    // Inside the child process now
    int pipe_fds[2];
    if (pipe(pipe_fds) != 0) {
      print_error();
      return error;
    }
    
    int pipe_pid = fork();
    if (pipe_pid < 0) {
      print_error();
      return error;
    }
    else if (pipe_pid == 0) {
      // left side command arguments to be run
      close(STDOUT_FILENO);
      dup2(pipe_fds[1], 1);
      close(pipe_fds[0]);
      execvp(left_side_arguments[0], left_side_arguments);
    }
    else {
      // right side command arguments to be run
      close(STDIN_FILENO);
      dup2(pipe_fds[0], 0);
      close(pipe_fds[1]);
      execvp(right_side_arguments[0], right_side_arguments);
    }
  }
  else {
    // Inside the parent process still
    wait(NULL);
  }
  
  return 1;
}
