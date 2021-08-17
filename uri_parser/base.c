
#include <np_base.h>

#include <assert.h>
#include <string.h>

#include <stdlib.h>

uint8_t *base_dup_raw_data(uint8_t *data, unsigned s) {
  uint8_t *dup = (uint8_t *)malloc(s);
  assert(dup != np_null);
  memcpy(dup, data, s);
  return dup;
}

char *base_dup_fixed_str(const char *temp, unsigned len) {
  if (len == 0)
    return np_null;

  char *cpnt = (char *)malloc(len + 1);
  assert(cpnt != null);
  memcpy(cpnt, temp, len);
  cpnt[len] = 0;
  return cpnt;
}

char* base_dup_str(const char* temp) {
  size_t len = strlen(temp);

  char *cpnt = (char *)malloc(len + 1);
  assert(cpnt != null);
  memcpy(cpnt, temp, len);
  cpnt[len] = 0;
  return cpnt;
}