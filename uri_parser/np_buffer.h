#pragma once

#include <np_base.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint8_t *ptr;
  int size;
  int capacity;
} np_buffer;

np_buffer *create_buffer();

/**
 * @brief append raw data after current position
 */
void buffer_append_raw(np_buffer *buf, uint8_t *data, int size);

/**
 * @brief append a char after current position
 */
void buffer_append_char(np_buffer *buf, char c);

/**
 * @brief append a string after current position
 */
void buffer_append_string(np_buffer *buf, char *str);

#ifdef __cplusplus
}
#endif