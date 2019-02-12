#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"

#define BUFFER_SIZE 128
#define DELIMITER " "

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
