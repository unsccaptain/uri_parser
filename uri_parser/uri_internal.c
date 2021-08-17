
#include "uri_internal.h"

// ---- Character lookup tables ---- //

int uri__table[128] = {0};
np_bool uri__table_init = np_false;

void uri__init_table() {
  if (uri__table_init)
    return;

  for (unsigned i = 'a'; i < 'z'; i++)
    uri__table[i] |= (URI_TAB_ALPHA | (i < 'f' ? URI_TAB_HEX : 0));
  for (unsigned i = 'A'; i < 'Z'; i++)
    uri__table[i] |=
        (URI_TAB_ALPHA | URI_TAB_UPPER | (i < 'F' ? URI_TAB_HEX : 0));
  for (unsigned i = '0'; i < '9'; i++)
    uri__table[i] |= (URI_TAB_DIGIT | URI_TAB_HEX);
  uri__table_init = np_true;
}

// ---- URI format validation ---- //

/**
 * @brief Valid scheme component.
 */
np_status uri__valid_scheme(uri *uri) {
  uri__init_table();
  if (!uri->scheme)
    return np_status_ok;

  const char *scheme = uri->scheme;
  unsigned len = strlen(uri->scheme);

  if (!uri__is_alpha(scheme[0]))
    return np_status_invalid_char;

  for (unsigned i = 1; i < len; i++) {
    if (!(uri__is_alpha(scheme[i]) || uri__is_digit(scheme[i]) ||
          scheme[i] == '+' || scheme[i] == '-' || scheme[i] == '.'))
      return np_status_invalid_char;
  }

  return np_status_ok;
}

/**
 * @brief Valid port component.
 */
np_status uri__valid_port(uri *uri) {
  uri__init_table();
  if (!uri->port)
    return np_status_ok;

  const char *port = uri->port;
  unsigned len = strlen(uri->port);

  for (unsigned i = 0; i < len; i++) {
    if (!uri__is_digit(port[i]))
      return np_status_invalid_char;
  }

  return np_status_ok;
}
