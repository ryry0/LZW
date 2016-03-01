#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <hashtable.h>

#define NUM_BUCKETS 10
#define BUFFER_SIZE 3841 //from 2^12 - 2^8 +1

int main(int argc, char ** argv) {
  int input_fd;
  int output_fd;
  int key = 256;

  hash_table dictionary;

  if (argc > 2) {
    input_fd = open(argv[1], O_RDONLY);
    if (input_fd == -1) {
      printf("File not found!");
      return 1;
    }
    output_fd = open(argv[2], O_TRUNC | O_RDWR | O_CREAT, 0666);
  }
  else {
    printf("Please provide filenames for");
    printf("input and output files\n");
  }

  //init data structures
  initTable(&dictionary, NUM_BUCKETS);

  //read from file
  char buf[BUFFER_SIZE] = {0};
  int buf_i = 0;
  char current;
  hash_node * last_match= NULL;
  while (read(input_fd, &current, 1)) {
    
    buf[buf_i++] = current;
    buf[buf_i] = 0;
    hash_node * node = NULL;
    printf("checking the dictionary for buf=%s\n", buf);
    node = retrieve(&dictionary, buf);
    if (node == NULL) {
      insert(&dictionary, buf, key++);
      buf[0]=current;
      buf[1]=0;
      buf_i = 1;
      if(last_match != NULL) {
        char temp[1000]={0};
        sprintf(temp, "%d", last_match->data);
        write(output_fd, temp, strlen(temp));
        printf("last match NOT null: %d\n", last_match->data);
      } else {
        write(output_fd, buf, strlen(buf));
        printf("last match null: %s\n", buf);
      }
    } else {
      last_match = node;
    }
  }

  close (input_fd);
  close (output_fd);

  destroyTable(&dictionary);

  return 0;
}
