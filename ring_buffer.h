#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_

struct ring_buffer {
  char * buffer;
  size_t buffer_size;
  size_t head;
  size_t tail;
  size_t length;
};


char * initBuffer(struct ring_buffer * r_buffer, size_t size);
void popAllButBack(struct ring_buffer * r_buffer, char * data_buff);
void pushBack(struct ring_buffer * r_buffer, char data);
void printBuffer (struct ring_buffer * r_buffer);
void destroyBuffer (struct ring_buffer * r_buffer);

#endif
