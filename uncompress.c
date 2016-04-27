#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
//#include <ring_buffer.h>
//#include <hashtable.h>

//#define NUM_BUCKETS 6000
#define ARRAY_SIZE 70000 //int16 is less than this

//shoud change this for 16 bits
#define BUFFER_SIZE 3841 //from 2^12 - 2^8 +1

union ben {
  uint16_t num;
  char c[2];
};

int fpeek(FILE *file);
int fpeekInt16(FILE *file);

int main(int argc, char ** argv) {

  FILE *input_fp;
  FILE *output_fp;

  int key = 256;

  char **string_arr = calloc(ARRAY_SIZE, sizeof(char*));
  if (string_arr == NULL) {
    printf("Allocate string array failed\n");
    return 1;
  }

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
  for (size_t i = 0; i < 256; ++i) {
    string_arr[i] = calloc(2, sizeof(char));
    if (string_arr[i] == NULL) {
      printf("Allocate string failed\n");
      return 1;
    }
    string_arr[i][0] = (char)i;
  }
  string_arr[0x0a00] = calloc(2, sizeof(char));
  string_arr[0x0a00][0] = '\n';

  char input_buffer[BUFFER_SIZE] = {0};
  size_t buffer_end = 0;
  union ben current_int_ben;

  char *current_string;
  while (fread(&current_int_ben.c[0], 1, 1, input_fp) &&
        fread(&current_int_ben.c[1], 1, 1, input_fp)) {

    uint16_t current_int = current_int_ben.num;
    if(string_arr[current_int] == NULL) {
      printf("Number %d does not exist in dictionary.\n", current_int);
      break;
    }

    uint16_t peeked_int;
    if ((peeked_int = fpeekInt16(input_fp)) != EOF) {
      if (key < 60000) {
        if (string_arr[peeked_int] != NULL) {
          char buff[2] = {0};
          buff[0] = (char)string_arr[peeked_int][0];

          string_arr[key] =
            malloc((strlen(string_arr[current_int])+1)*sizeof(char));
          strcpy(string_arr[key], string_arr[current_int]);

          string_arr[key] = strcat(string_arr[key], buff);

          key++;
        }
        else {
          char buff[2] = {0};
          buff[0] = (char)string_arr[current_int][0];

          string_arr[key] =
            malloc((strlen(string_arr[current_int])+1)* sizeof(char));
          strcpy(string_arr[key], string_arr[current_int]);

          string_arr[key] = strcat(string_arr[key], buff);

          key++;
        }
      }
    } //end if(peeked_int

    current_string = string_arr[current_int];
    fprintf(output_fp, "%s", current_string);
  } //end while

  fclose (input_fp);
  fclose (output_fp);

  for (size_t i = 0; i < ARRAY_SIZE; ++i) {
    if (string_arr[i] != NULL)
      free(string_arr[i]);
  }
  free(string_arr);

  return 0;
}

int fpeek(FILE *file) {
  int c;
  c = fgetc(file);
  ungetc(c, file);
  return c;
}

int fpeekInt16(FILE *file) {
  int c, d;
  union ben bb;
  c = fgetc(file);
  if (c == EOF) {
    return EOF;
  }
  d = fgetc(file);
  if (d == EOF) {
    printf("Compression is uneven\n");
    return EOF;
  }

  ungetc(d, file);
  ungetc(c, file);

  bb.c[0] = c;
  bb.c[1] = d;

  return bb.num;
}

