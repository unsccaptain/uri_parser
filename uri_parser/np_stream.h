#pragma once

#include <np_base.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint8_t *head_ptr;
  uint8_t *tail_ptr;
  uint8_t *cur_ptr;
} input_stream;

void stream_forward(input_stream *s, unsigned l);

void stream_backward(input_stream *s, unsigned l);

np_status stream_consume_char(input_stream *, char *);

np_status stream_consume_chunk(input_stream *s, uint8_t *buf, int size);

/**
 * @brief read a decimal number
 */
np_status stream_consume_dec_number(input_stream *s, int *number);

/**
 * @brief read a hexadecimal number
 */
np_status stream_consume_hex_number(input_stream *s, int *number);

char stream_look_forward(input_stream *s);

unsigned stream_remain_size(input_stream *s);

void stream_append_data(input_stream *s, uint8_t *data, size_t len);

/**
 * @brief use a string to initialize the stream
 */
input_stream *create_stream_from_string(const char *);

/**
 * @brief use raw buffer to initialize the stream
 */
input_stream *create_stream_from_buffer(uint8_t *data, size_t len);

#ifdef __cplusplus
}
#endif