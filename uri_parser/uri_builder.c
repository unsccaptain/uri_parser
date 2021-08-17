#include <memory.h>
#include <np_buffer.h>
#include <np_uri.h>
#include <stdlib.h>

#include "uri_internal.h"

void uri_builder_init(uri *uri) { memset(uri, 0, sizeof(*uri)); }

uri_path_seg *uri__new_segment(const char *path_seg) {
  uri_path_seg *seg = (uri_path_seg *)malloc(sizeof(*seg));
  seg->segment = base_dup_str(path_seg);
  seg->next = null;
  return seg;
}

void uri_builder_append_segment(uri *uri, const char *path_seg) {
  uri_path_seg *seg = uri__new_segment(path_seg);

  uri->tail_path->next = seg;
  uri->tail_path = seg;
}

void uri_builder_insert_segment(uri *uri, const char *path_seg,
                                uri_path_seg *after) {
  uri_path_seg *seg = uri__new_segment(path_seg);
  seg->next = after->next;
  after->next = seg;
  if (uri->tail_path == after)
    uri->tail_path = seg;
}

np_status uri_build(uri *uri, char **out) {
  np_buffer *buf = create_buffer();
  np_bool relative = np_true;

  if (uri->scheme) {
    if (uri__valid_scheme(uri) != np_status_ok)
      return np_status_invalid_token;
    relative = np_false;
    buffer_append_string(buf, uri->scheme);
    buffer_append_char(buf, ':');
  }

  // host must be start with double slash
  // regular URI or network-path
  if (uri->host) {
    buffer_append_string(buf, "//");
    buffer_append_string(buf, uri->host);
  }

  // append port
  if (uri->port) {
    if (uri__valid_port(uri) != np_status_ok)
      return np_status_invalid_number;
    buffer_append_char(buf, ':');
    buffer_append_string(buf, uri->port);
  }

  uri_path_seg *path = uri->head_path;
  // if uri doesn't start with scheme but the first path segment has ':'
  // such case should start with ./
  if (relative && path && strchr(path, ':') != np_null)
    buffer_append_char(buf, ".");

  // we didn't consider path-noscheme ABNF here as it is useless
  while (path) {
    buffer_append_char(buf, '/');
    buffer_append_string(buf, path->segment);
    path = path->next;
  }

  if (uri->query) {
    buffer_append_char(buf, '?');
    buffer_append_string(buf, uri->query);
  }

  if (uri->frgment) {
    buffer_append_char(buf, '#');
    buffer_append_string(buf, uri->frgment);
  }

  // zero terminated
  buffer_append_char(buf, 0);

  *out = base_dup_str(buf->ptr);

  return np_status_ok;
}
