#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ring_buffer.h>

char * initBuffer(struct ring_buffer * r_buffer, size_t size) {

  r_buffer->buffer = (char *) malloc(sizeof(char) * size);
  memset(r_buffer->buffer, 0, size);
  r_buffer->buffer_size = size;
  r_buffer->head = 0;
  r_buffer->tail = 0;
  r_buffer->length = 0;

  return r_buffer->buffer;

}

void popAllButBack(struct ring_buffer * r_buffer, char * data_buff) {
  for (size_t i = 0; i < r_buffer->length - 1; i++) {
    data_buff[i] = r_buffer->buffer[(r_buffer->head + i) % r_buffer->buffer_size];
  }
  r_buffer->length = 1;
}

void pushBack(struct ring_buffer * r_buffer, char data) {

  r_buffer->buffer[r_buffer->tail] = data;

  r_buffer->tail = (r_buffer->tail + 1) % r_buffer->buffer_size;

  if (r_buffer->length == r_buffer->buffer_size)
    r_buffer->head = (r_buffer->head + 1) % r_buffer->buffer_size;
  else
    r_buffer->length++;

} //end void pushBack

void printBuffer (struct ring_buffer * r_buffer) {
  for (size_t i = 0; i < r_buffer->length; i++) {
    printf("%c", r_buffer->buffer[(r_buffer->head + i) % r_buffer->buffer_size]);
  }
  printf("\t length: %ld\n", r_buffer->length);
  printf("\n");
}

void readBuffer (struct ring_buffer * r_buffer, char * buffer) {
  size_t i = 0;
  for (i = 0; i < r_buffer->length; i++) {
    buffer[i] =
      r_buffer->buffer[ (r_buffer->head + i) %
      r_buffer->buffer_size ];
  }
  buffer[i] = '\0';
}

void destroyBuffer (struct ring_buffer * r_buffer) {
  free(r_buffer->buffer);
}

