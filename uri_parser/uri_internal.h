#pragma once

#include <np_base.h>
#include <np_uri.h>

#define URI_INVALID_CHAR 0x01
#define URI_OK 0x0

#define URI_TAB_ALPHA 0x1
#define URI_TAB_DIGIT 0x2
#define URI_TAB_HEX 0x4
#define URI_TAB_UPPER 0x8

// ---- Character lookup tables ---- //

extern int uri__table[128];
extern int uri__table_init;

void uri__init_table();

int __forceinline uri__is_alpha(char c) {
  return uri__table[c] & URI_TAB_ALPHA;
}

int __forceinline uri__is_digit(char c) {
  return uri__table[c] & URI_TAB_DIGIT;
}

// ---- URI format validation ---- //

/**
 * @brief Valid scheme component.
 */
np_status uri__valid_scheme(uri *uri);

/**
 * @brief Valid port component.
 */
np_status uri__valid_port(uri *uri);