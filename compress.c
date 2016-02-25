#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "ring_buffer.h"

int main(int argc, char ** argv) {

  int input_fd;
  int output_fd;
  struct ring_buffer input_buffer;

  char current;
  char string_buffer[10] = {0};

  if (argc > 1) {
    input_fd = open(argv[1], O_RDONLY);
    if (input_fd == -1) {
      printf("File not found!");
      return 1;
    }
  }
  else
    printf("Please provide a filename");



  initBuffer(&input_buffer, 10);

  for (int i = 0; i < 20; i++) {
  //while (read(input_fd, &current, 1)) {
    read(input_fd, &current, 1);
    pushBack(&input_buffer, current);
    printBuffer(&input_buffer);
  } //end while (input..

  popAllButBack(&input_buffer, string_buffer);
  printf("before: %s after\n", string_buffer);

  destroyBuffer(&input_buffer);

  return 0;
}
