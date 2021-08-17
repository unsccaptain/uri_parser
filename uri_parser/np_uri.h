#pragma once

#include <np_stream.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  fsm_init,
  fsm_scheme,
  fsm_first_slash,
  fsm_begin_host,
  fsm_begin_port,
  fsm_begin_segment,
  fsm_begin_query,
  fsm_begin_frag,
  fsm_finish
} uri_fsm;

typedef struct _uri_path_seg {
  struct _uri_path_seg *next;
  char *segment;
} uri_path_seg;

typedef struct {
  char *scheme;
  char *host;
  char *port;
  char *query;
  char *frgment;

  uri_path_seg *head_path;
  uri_path_seg *tail_path;

  np_bool relative_ref;
  int ref_kind;

  uri_fsm fsm;
} uri;

// ---- URI Parser ---- //

void uri_parser_init(uri *uri);

int uri_parse(input_stream *s, uri *uri);

// ---- URI Creator ---- //

void uri_builder_init(uri *uri);

void uri_builder_append_segment(uri *uri, const char *seg);

void uri_builder_insert_segment(uri *uri, const char *seg, uri_path_seg *after);

np_status uri_build(uri *uri, char **out);

#ifdef __cplusplus
}
#endif