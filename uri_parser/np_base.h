#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef null
#define null ((void *)0)
#endif // !null

typedef int np_bool;
typedef int np_status;
typedef void np_void;
typedef uint8_t *np_data;
typedef int np_size;

#define np_null 0

#define np_true 1
#define np_false 0

#define np_status_ok 0
#define np_status_err 1
#define np_status_expect_crlf 12
#define np_status_expect_ws 13
#define np_status_invalid_char 20
#define np_status_invalid_token 21
#define np_status_invalid_header 22
#define np_status_invalid_number 50
#define np_status_invalid_data 51
#define np_status_invalid_status_code 52

#define np_status_overflow 100
#define np_status_lack_memory 101
#define np_status_not_found 102
#define np_status_item_exist 103

#define np_status_more_data 104

char *base_dup_fixed_str(const char *temp, unsigned len);

char *base_dup_str(const char *temp);

uint8_t *base_dup_raw_data(uint8_t *data, size_t s);

#ifdef __cplusplus
}
#endif