#include <np_buffer.h>

#include <stdlib.h>
#include <string.h>

np_buffer *create_buffer() {
  np_buffer *buf = (np_buffer *)malloc(sizeof(np_buffer));
  buf->ptr = (uint8_t *)malloc(1024);
  buf->size = 0;
  buf->capacity = 1024;
  return buf;
}

np_bool buffer__need_resize(np_buffer *buf, int size) {
  return buf->capacity - buf->size < size ? np_true : np_false;
}

np_status buffer__resize(np_buffer *buf, int size) {
  int new_size = (size + 1023) & ~1024;
  if (new_size < buf->capacity)
    return np_status_ok;

  uint8_t *new_buf = (uint8_t *)malloc(new_size);
  memcpy(new_buf, buf->ptr, buf->size);

  free(buf->ptr);

  buf->capacity = new_size;
  buf->ptr = new_buf;

  return np_status_ok;
}

/**
 * @brief append raw data after current position
 */
void buffer_append_raw(np_buffer *buf, uint8_t *data, int size) {
  if (buffer__need_resize(buf, size))
    buffer__resize(buf, buf->size + size);

  memcpy(buf->ptr + buf->size, data, size);
  buf->size += size;
}

/**
 * @brief append a char after current position
 */
void buffer_append_char(np_buffer *buf, char val) {
  if (buffer__need_resize(buf, sizeof(char)))
    buffer__resize(buf, buf->size + sizeof(char));

  *(buf->ptr + buf->size) = val;
  buf->size += sizeof(char);
}

/**
 * @brief append a string after current position
 */
void buffer_append_string(np_buffer *buf, char *val) {
  size_t s = strlen(val);
  if (buffer__need_resize(buf, s))
    buffer__resize(buf, buf->size + s);

  memcpy(buf->ptr + buf->size, val, s);
  buf->size += s;
}