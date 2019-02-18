#include <stdio.h>
#include "parser.h"

int main() {
  char *input = read_input();
  char **segments = parse_input(input);

  int index = 0;
  while (segments[index] != NULL) {
    printf("[%d] %s\n", index, segments[index]);
    index++;
  }

  interpret_input(segments);
  
  return 1;
}
