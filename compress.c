#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ring_buffer.h>
#include <hashtable.h>

#define NUM_BUCKETS 6000
#define BUFFER_SIZE 3841 //from 2^12 - 2^8 +1

union ben {
  uint16_t num;
  char c[2];
};

int fpeek(FILE *file);
void initDictionary(hash_table *h_table);

int main(int argc, char ** argv) {

  FILE *input_fp;
  FILE *output_fp;

  int key = 256;
  hash_table dictionary;

  char current_char;

  if (argc > 2) {
    input_fp = fopen(argv[1], "rb");
    if (input_fp == NULL) {
      printf("File not found!");
      return 1;
    }
    output_fp = fopen(argv[2], "wb");
  }
  else {
    printf("Please provide filenames for");
    printf("input and output files\n");
  }

  //init data structures
  initTable(&dictionary, NUM_BUCKETS);
  initDictionary(&dictionary);

  char input_buffer[BUFFER_SIZE] = {0};
  size_t buffer_end = 0;
  while (fread(&current_char, 1, 1, input_fp)) {
    char lookup_str[BUFFER_SIZE] = {0};
    //peek the next byte
    int peeked_val = fpeek(input_fp);


    input_buffer[buffer_end++] = current_char;
    strcpy(lookup_str, input_buffer);
    lookup_str[buffer_end] = peeked_val;

    hash_node * node = NULL;
    if (((node = retrieve(&dictionary, input_buffer)) != NULL) &&
      ((retrieve(&dictionary, lookup_str) == NULL) ||
      (peeked_val == EOF))) {

      union ben b;
      b.num = node->data;
      fwrite(b.c, 1, 2, output_fp);

      if (key < 60000) {
        insert(&dictionary, lookup_str, key++);
      } else {
        /*
        printf("Dictionary full, no entry will be made for %s",
            lookup_str);
            */
      } //end else
      memset(input_buffer, 0, BUFFER_SIZE); //clear buffer
      buffer_end = 0;
    } //end if
  } //end while

  fclose (input_fp);
  fclose (output_fp);

  destroyTable(&dictionary);

  return 0;
}

void initDictionary(hash_table *h_table) {
  for (uint16_t i = 0; i < 256; ++i) {
    char c[2] = {0};
    c[0] = i;
    insert(h_table, c, i);
  }
}

int fpeek(FILE *file) {
  int c;
  c = fgetc(file);
  ungetc(c, file);
  return c;
}
