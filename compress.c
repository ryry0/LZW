#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <ring_buffer.h>
#include <hashtable.h>

#define NUM_BUCKETS 10
#define BUFFER_SIZE 3841 //from 2^12 - 2^8 +1

int main(int argc, char ** argv) {

  int input_fd;
  int output_fd;

  int key = 256;
  hash_node* last_match = NULL;

  struct ring_buffer input_buffer;
  hash_table dictionary;

  char current;

  if (argc > 2) {
    input_fd = open(argv[1], O_RDONLY);
    if (input_fd == -1) {
      printf("File not found!");
      return 1;
    }
    output_fd = open(argv[2], O_RDWR | O_CREAT | O_SYNC, 0666);
  }
  else {
    printf("Please provide filenames for");
    printf("input and output files\n");
  }

  //init data structures
  initBuffer(&input_buffer, BUFFER_SIZE);
  initTable(&dictionary, NUM_BUCKETS);

  //read from file

  //priming read
  read(input_fd, &current, 1);
  pushBack(&input_buffer, current);

  while (read(input_fd, &current, 1)) {
    char string_buffer[BUFFER_SIZE] = {0};
    char output_buffer[BUFFER_SIZE] = {0};
    size_t write_len = 0;

    printf("current character read: %c\n", current);
    if (current == EOF)
      break;

    pushBack(&input_buffer, current);

    //get string inside input_buffer
    readBuffer(&input_buffer, string_buffer);
    printf("string buffer before retrieve: %s\n", string_buffer);

    hash_node * node = NULL;
    if ((node = retrieve(&dictionary, string_buffer)) != NULL) {
      last_match = node;
      continue;
    }

    if (key < 4096)
      insert(&dictionary, string_buffer, key++);


    write_len = input_buffer.length - 1;
    popAllButBack(&input_buffer, output_buffer);

    if (last_match == NULL) {
      write(output_fd, output_buffer, write_len);
      printf("output_buffer: %s\n", output_buffer);
    }
    else {

      sprintf(output_buffer, "%d", last_match->data);
      write(output_fd, output_buffer, strlen(output_buffer));
      /*
      write(output_fd, (char *) &itoa(last_match->data), sizeof(int));
      */
      printf("output_buffer(int): %d\n", last_match->data);
    }
  }

  close (input_fd);
  close (output_fd);

  destroyBuffer(&input_buffer);
  destroyTable(&dictionary);

  return 0;
}
