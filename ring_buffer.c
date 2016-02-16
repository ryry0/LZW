#include <stdlib.h>
#include <string.h>
#include <ring_buffer.h>

char * initBuffer(struct ring_buffer * r_buffer, size_t size) {

  r_buffer->buffer = (char *) malloc(sizeof(char) * size);
  memset(r_buffer->buffer, 0, size);
  r_buffer->buffer_size = size;
  r_buffer->head = 0;
  r_buffer->tail = 0;

  return r_buffer->buffer;

}

void popAllButBack(struct ring_buffer * r_buffer, char * data_buff) {

  int j = 0;
  if (r_buffer->head < r_buffer->tail) {
    for (int i = r_buffer->head; i < r_buffer->tail; i++) {
      data_buff[j++] = r_buffer->buffer[i];
    }
  }
  else {
    for (int i = r_buffer->head; i < r_buffer->buffer_size; i++) {
      data_buff[j++] = r_buffer->buffer[i];
    }
    for (int i = 0; i < r_buffer->tail; i++) {
      data_buff[j++] = r_buffer->buffer[i];
    }
  } //end else
  r_buffer->head = r_buffer->tail - 1;
}

void pushBack(struct ring_buffer * r_buffer, char data) {
  if (r_buffer->tail == r_buffer->buffer_size) {
    r_buffer->tail = 0;
  }
  r_buffer->buffer[r_buffer->tail++] = data;
}

void destroyBuffer (struct ring_buffer * r_buffer) {
  free(r_buffer->buffer);
}

